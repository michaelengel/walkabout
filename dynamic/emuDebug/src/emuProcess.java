/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// General packages
import java.util.*;
import java.io.*;
import java.lang.*;
import java.net.*;

public class emuProcess {


  private static native void setBufferNull();
  /******************************************************
    The following are socket port numbers to the emulator
    process and the the factory process. 
    ******************************************************/
  private static final int emulatorPort = 4445;

  /******************************************************
    The following are the native methods necessary to 
    communicate with the emulator.

    returnReg             -> Returns the current register 
    state of the emulator
    returnPC              -> Returns the current pc of the 
    emulator
    secondMain            -> Used to setup and init the 
    emulator
    executeOneInstruction -> Tell the emulator to execute
    one instruction	     
    *******************************************************/	
  private static native RegFile returnReg(); 
  private static native int returnPC();
  private static native int secondMain(String[] arg);
  private static native void executeOneInstruction();


  public static Socket createServerConnection(ServerSocket serverSocket) {  
	    	
    
    System.out.println("Server connection created successfully!");

    Socket clientSocket = null;		
    try {
      clientSocket = serverSocket.accept();
    } catch (IOException socketE) {
      System.out.println("Accept failed :" + socketE);
      System.exit(-1);
    }   
    return clientSocket;   
  }


  public static void main(String[] args) {

    // Must first load the library emulib so that we can 
    // use ours native functions
    System.loadLibrary("emulib");

    // No longer used
    // emuProcess.setBufferNull();

    // In emultor process
    // Create a socketServer and wait till
    // main process connects to it	    	    
			    
    System.err.println("Emulator Process starting up.");
		
    ServerSocket serverSocket = null;
    try {
      serverSocket = new ServerSocket(emuProcess.emulatorPort);
    } catch (IOException e) {
      System.out.println("Could not listen on emulatorPort: " + e);
      System.exit(-1);
    }		
		
    // Wait till the main GUI process connects to this process
    Socket inSocket = emuProcess.createServerConnection(serverSocket);
				
    // Native call to start up the emulator
    int memAddress = emuProcess.secondMain(args);
		
    // Use for later disassemble (of one PC at a time)
    sparcdisdebug sparcDisasm = new sparcdisdebug(memAddress);		
			    		
    try {
		
      // Create object streams to the GUI process				
      System.err.println("Emulator opening object stream...");
		    
      ObjectInputStream objInStream = new 
	ObjectInputStream(inSocket.getInputStream());
      ObjectOutputStream objOutStream = new
	ObjectOutputStream(inSocket.getOutputStream());

      System.err.println("...object stream opened successfully");
					
      while (true) {
	String inString = objInStream.readUTF();		
	if (inString.equals("restart")) {
	  objInStream.close();
	  objOutStream.close();
	  inSocket.close();
	  serverSocket.close();    
	  System.exit(0);
	}
	else if (inString.equals("quit")) {
	  objInStream.close();
	  objOutStream.close();
	  inSocket.close();
	  serverSocket.close();    
	  System.exit(0);			    			
	}
	else if (inString.equals("pc")) {			
	  objOutStream.writeInt(emuProcess.returnPC());
	  objOutStream.flush();
	}
	else if (inString.equals("reg")){
	  objOutStream.writeObject(emuProcess.returnReg());
	  objOutStream.flush();
	}
			
	else if (inString.equals("next")){
	  emuProcess.executeOneInstruction();
	}
	else if (inString.equals("disasm")){
	  int thisPC = objInStream.readInt();    			    
	  sparcDisasm.disassembleRegion(thisPC, thisPC + 1);
	  objOutStream.writeUTF(sparcDisasm.returnDisasm());			    			
	  objOutStream.flush();
	}
	else if (inString.equals("executeUntilBreak")) {
	  int setSize = objInStream.readInt();
	  HashSet breakSet = new HashSet();
			    	    		    
			    
	  for (int i = 0; i < setSize; i++) {
	    Integer tempInt = (Integer) objInStream.readObject();
	    System.out.println("Objects in set is " + tempInt);
	    breakSet.add(tempInt);							    
	  }
			    
	  int thisPC = emuProcess.returnPC();			    
	  System.out.println("Checking for PC");
			    			   

	  while (!breakSet.contains(new Integer(thisPC))) {
	    // Native methods			       
	    emuProcess.executeOneInstruction();	
	    thisPC = emuProcess.returnPC();
	  }			    
	  System.out.println("Finished!");
	}
      }
    } catch (IOException serverE) {
      System.err.println("IOException serverE:" + serverE);
      System.exit(-1);
		
    } catch (ClassNotFoundException classE) {
      System.err.println("ClassNotFoundException classE:" + classE);
      System.exit(-1);		
    }
				    	       
  }	    
}

