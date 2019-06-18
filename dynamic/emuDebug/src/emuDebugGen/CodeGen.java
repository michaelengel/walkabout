import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.*;
import java.util.*;

/*****************************************************************************
   This Main Purpose of the class is to generates the final emuDebug.java 
   file given a particular machine specification
   
   This class works by parsing through the machine specification file and
   creating code fragments from the specification file.
   
   It then parses through the skeleton files for special symbols that
   start with a @ symbol.
   
   It will then replace each symbol with the appropriate code fragment
   
   Current implementation :
           
        First parameter is the name of the machine specification file
	
        Second parameter is the name of the source skeleton file for
	emuDebug.java
	
	Third parameter is the name of the source skeleton file for
	emuLib.cc
   
        Completed emuDebug.java aand emuLib.cc files are written 
	out to the current directory
          
        RegFile.java is automatically generated from the machine
        specification file

******************************************************************************/
class CodeGen
{  

  private static final String regFileName = "RegFile.java";
  private static final String emuDebugName = "emuDebug.java";
  private static final String emuLibName = "emuLib.cc";
  
  private TokenReader tReader;
  public CodeGen(TokenReader inReader) {
      tReader = inReader;  
  }
  
  public Hashtable returnBlockTable() {
      // blockTable is a hashtable that holds the keyString (the string 
      // after the @ sign) and the associated ReplacementStruct object 
      // (which holds all the code fragment)      
      Hashtable blockTable = new Hashtable();                 
                
      while (true) {
        // CodeBlock is the class of the specification parser
        // It can take the toker reader and then be able to 
        // parse through and create the code fragments for
        // one set of code block
        CodeBlock testBlock = new CodeBlock(tReader);
        
        if (testBlock.syntaxError()){
          System.err.println("Syntax error! Parser stage");
          break;
        }
        if (testBlock.finished()){
          break;
        }
                                                        
        String blockName = testBlock.returnLabel();
        ReplacementStruct replacementStruct =
                                  testBlock.returnStruct();                        
                                  
        // blockTable is basically storing each of the code blocks
        // and associate the blockName as its key to the block                          
        blockTable.put(blockName, replacementStruct);
      }
      return blockTable;
  }
  
  public String codeReplacement(BufferedReader skeletonReader, Hashtable blockTable) {
      // A string buffer used to store the code as it is generated or 
      // passed through. Eventually, we will send this string to stdout
      // where it will be the final code for emuDebug.java                 
      StringBuffer codeBuffer = new StringBuffer();
                 
      String tempString = null;
      try {   
	  while ((tempString = skeletonReader.readLine()) != null) {
            // Read a line from the skeleton file. If the line contains
            // a @ sign, stop and analyse. IndexString holds the index
            // to where the @ sign is.
            //
            // Current limitation: Can only have one @ symbol per line
            int indexString = tempString.indexOf("@");

            // indexString equals -1 if an @ sign is not in the line
            if (indexString != -1) {

              // Once we know that there is an @ sign, find the next non-letter
              // character that follows the @ sign. We know that in between
              // comprises the key string. Save the position of the next
              // non-letter character in secondIndexString

	      if (tempString.length() <= indexString + 1) {
		  // We see a @ but no string behind it
        	  System.err.println("Parser Error. String has @ but no string behind");
        	  System.exit(1);
	      }	      
	
	      // Read the String character by character until we find 
	      // the next non-letter character after the @ symbol      
	      StringReader tempStringReader = new StringReader(tempString.substring(indexString + 1));
	      int secondIndexString = 0;
	      while ((secondIndexString = tempStringReader.read()) != -1) {
	      	if (!(Character.isLetter((char) secondIndexString) || ((char) secondIndexString) == '_')) {
		    break;
		}
	      }	      	      	      	      
	      	      	           
              // If still we can't find the end of this string, perhaps
              // we have reached the end of the String. Just make
              // the secondIndexString the end of the string. 
	      //
	      //(remember the minus 1 part or we might get an arrayOutOfBounds
              // exception) 
	      //
	      // Correction, minus 1 not needed after all.                                
              if (secondIndexString == -1) {
        	secondIndexString = tempString.length();
              }
	      else {
	         secondIndexString = tempString.indexOf((char) secondIndexString, indexString);	      
	      }

              String keyString = null;                        

              // The if statement is just to check bounds
              if (secondIndexString >= (indexString + 1)) {
        	// Setting keyString value to be the string between
        	// the @ sign and the next WhiteSpace
        	keyString = tempString.substring(indexString + 1,
                                        	 secondIndexString);
              }
              else {
        	// Weird logic error in the parser. Should never get here
        	// unless we hit a bug in the code
        	System.err.println("Parser Error. String has negative index");
        	System.exit(1);
              }
	      
	      String replaceString = null;
	      StringBuffer restOfStringBuffer = new StringBuffer();
	      
	      // System.err.println(keyString);	     

	      if (keyString.equals("NATIVEREG")) {
		  // Temporary Hack, output NATIVE reg order for emuLib.cc      
		  Enumeration entryEnum = blockTable.elements();
		  StringBuffer printStringBuffer = new StringBuffer();

		  while (entryEnum.hasMoreElements()) {      
        	    printStringBuffer.append(((ReplacementStruct)entryEnum.nextElement()).getRegValue("NATIVEREG"));
        	    if (entryEnum.hasMoreElements()) {
        		printStringBuffer.append(",\n");
        	    }        
		  }      	
		  String tempReplaceString = tempString.substring(secondIndexString);		  
		  restOfStringBuffer.append(tempReplaceString);		                         		  		  
		  replaceString = printStringBuffer.toString();	
	      }	
	      else if (keyString.equals("NATIVEINIT")) {
		  // Temporary Hack, output NATIVE init reg order for emuLib.cc      
		  Enumeration entryEnum = blockTable.elements();
		  StringBuffer printStringBuffer = new StringBuffer();

		  while (entryEnum.hasMoreElements()) {      
        	       printStringBuffer.append(((ReplacementStruct)entryEnum.nextElement()).getRegValue("NATIVEINIT"));       
		  }      	
		  String tempReplaceString = tempString.substring(secondIndexString);		  
		  restOfStringBuffer.append(tempReplaceString);		                         		  		  
		  replaceString = printStringBuffer.toString();	
	      }
              else if (keyString.equals("NATIVEARRAY")) {
		  // Temporary Hack, output NATIVE init reg order for emuLib.cc      
		  Enumeration entryEnum = blockTable.elements();
		  StringBuffer printStringBuffer = new StringBuffer();

		  while (entryEnum.hasMoreElements()) {      
                      String tempReplaceString = ((ReplacementStruct)entryEnum.nextElement()).getRegValue("NATIVEARRAY");
                      if (tempReplaceString != null) {
                          printStringBuffer.append(tempReplaceString);       
                      }
		  }      	
		  String tempReplaceString = tempString.substring(secondIndexString);		  
		  restOfStringBuffer.append(tempReplaceString);		                         		  		  
		  replaceString = printStringBuffer.toString();                                        
              }		            	      
	      else {
        	  if (!blockTable.containsKey(keyString)) {                                        
        	    // An @ was used in the code but it is not part of the 
        	    // special symbols. Just ignore and moved to the next line
        	    codeBuffer.append(tempString + "\n");
        	    break;
        	  }

        	  // Now need to read string after the keyString                                
        	  String afterKeyString = tempString.substring(secondIndexString);                                          
        	  StringReader stringParser = new StringReader(afterKeyString);

        	  // Eat up all spaces between the keyString and the next word
        	  int cTemp;
        	  do {                                
        	    cTemp = stringParser.read();                                
        	  } while ( cTemp == ' ');


        	  StringBuffer afterKeyBuffer = new StringBuffer();

        	  // We eat up one too many characters, have to put it back
        	  // in the buffer first
        	  afterKeyBuffer.append((char) cTemp);

        	  // Read characters into the buffer until the next WhiteSpace
        	  // or end of String
        	  while (true) {                        
        	    cTemp = stringParser.read();                        
        	    if (cTemp == -1 || Character.isWhitespace((char)cTemp) ) {
        	      break;
        	    }        
        	    // Put all these characters into the afterKeyBuffer                                        
        	    afterKeyBuffer.append((char)cTemp);
        	  }

        	  // Now we know that the word after the keyString is in the
        	  // buffer afterKeyBuffer. This is necessary in order to find
        	  // which code fragment to insert

        	  // restOfStringBuffer holds all the characters of the current
        	  // line after the afterKeyBuffer.                                
        	  if (cTemp != -1) {
        	    restOfStringBuffer.append((char)cTemp);
        	    while ((cTemp = stringParser.read()) != -1) {
        	      restOfStringBuffer.append((char)cTemp);
        	    }
        	  }                                        


        	  replaceString = ((ReplacementStruct)(blockTable.get(keyString))).
                        	    getValue(afterKeyBuffer.toString());                        
        	  if (replaceString == null) {
                	// Means the word following the keyString cannot be found 
                	// in the ReplacementStruct. This means we don't have code
                	// replacement for this. Therefore, it should be a 
                	// syntax error;
        	      System.err.println("Syntax Error. Code for afterKeyWord does not exist.");
        	      System.exit(1);
        	  }
	      }                                     


              // Insert everything before finding the @ sign                                
              codeBuffer.append(tempString.substring(0, indexString));

              // Insert Replacement code replacing the @ symbols
              codeBuffer.append(replaceString);

              // Insert rest of the line after the @ symbols. NewLine needs
              // to be added since readLine had stripped it
              codeBuffer.append(restOfStringBuffer.toString() + "\n");

            }
            else {
              // No @ symbol is found. Put the string directly in and 
              // put back the NewLine character as it was stripped by
              // the readLine
              codeBuffer.append(tempString + "\n");                         
            }                        
	  }  
      } catch (IOException e) {
          System.err.println("IO error at codeReplacement");
	  System.exit(1);
      }
      return codeBuffer.toString();  
  }
  
  public String generateRegFile(Hashtable blockTable) {
      // printStringBuffer will be used to hold the String for the RegFile.java  
      StringBuffer printStringBuffer = new StringBuffer();
      
      // Writing header information
      printStringBuffer.append("import java.io.*;\n");
      printStringBuffer.append("class RegFile implements Serializable{ \n");      
      
      // 3 stages to output RegFile
      
      // Declare stage, where each of the variable declarations are done
      Enumeration entryEnum = blockTable.elements();      
      
      while (entryEnum.hasMoreElements()) {      
        printStringBuffer.append(((ReplacementStruct)(entryEnum.nextElement())).getRegValue("DECLARE"));
      }

      // Param stage, where the formal parameters to the constructor is created
      printStringBuffer.append("public RegFile(\n");
      entryEnum = blockTable.elements();      
      
      while (entryEnum.hasMoreElements()) {             
        printStringBuffer.append(((ReplacementStruct)entryEnum.nextElement()).getRegValue("PARAM"));
        
        // The last variable declaration in every block does not have a comma ending it since we
        // don't know the order we write the blocks out in. Therefore, we always add a comma to 
        // the end of the each block unless it is the last block. In that case, we close it with 
        // a close bracket
        if (entryEnum.hasMoreElements()) {
            printStringBuffer.append(",\n");
        }
        else {
            printStringBuffer.append(") \n");
        }              
      }  
      
      // Construct stage, where the code for the constructor is generated
      printStringBuffer.append("{\n");      
      entryEnum = blockTable.elements();     
            
      while (entryEnum.hasMoreElements()) {      
        printStringBuffer.append(((ReplacementStruct)entryEnum.nextElement()).getRegValue("CONSTRUCT"));
      }
      
      printStringBuffer.append("}\n}\n");
      return printStringBuffer.toString();
  }
  

  public static void main(String argv[])
  {
    if (argv.length < 3) {
    	System.err.println("Syntax error. Correct Syntax is : ");
	System.err.println("CodeGen <machine spec> <emuDebug skeleton> <emuLib skeleton>");
        System.exit(3);
    }
    try {      
    
      // Token scanner class that is used to break up the specification
      // file into appropriate tokens                     
      TokenReader tReader = new TokenReader(new FileReader(argv[0]));     
      
      CodeGen thisCodeGen = new CodeGen(tReader);
      Hashtable blockTable = thisCodeGen.returnBlockTable();

      // Reading the skeleton file                 
      BufferedReader skeletonReader = new BufferedReader(new FileReader(argv[1]));      
      String emuDebugString = thisCodeGen.codeReplacement(skeletonReader, blockTable);            
      
      // Code generating for emuDebug.java is done. Write to file emuDebug.java
      PrintWriter out
        = new PrintWriter(new BufferedWriter(new FileWriter(CodeGen.emuDebugName)));
      
      out.print(emuDebugString);
      out.flush();
      System.out.println(CodeGen.emuDebugName + " has been successfully generated.");
            
      // Reading the skeleton file for emuLib.cc               
      skeletonReader = new BufferedReader(new FileReader(argv[2]));      
      String emuLibString = thisCodeGen.codeReplacement(skeletonReader, blockTable);            
      
      // Code generating for emuLib.cc is done. Write to file emuDebug.java
      out = new PrintWriter(new BufferedWriter(new FileWriter(CodeGen.emuLibName)));
      
      out.print(emuLibString);
      out.flush(); 
      System.out.println(CodeGen.emuLibName + " has been successfully generated.");      
      
      // The second part of CodeGen class needs to create the RegFile.java file
      // from the machine specification          
        
      String regFileString = thisCodeGen.generateRegFile(blockTable);
      // All the code for RegFile is done, create RegFile.java, 
      // write to file and flush      
      out = new PrintWriter(new BufferedWriter(new FileWriter(CodeGen.regFileName)));
      out.print(regFileString);   
      out.flush();         
      System.out.println(CodeGen.regFileName + " has been successfully generated.");      
                       
    } catch (FileNotFoundException e) {
      System.err.println("File(s) cannot be found " + e);
      System.exit(1);
    } catch (IOException e) {
      System.err.println("IO Exception " + e);
      System.exit(1);    
    } catch (SecurityException e) {
      System.err.println("SecurityException " + e);
      System.exit(2);
    }
  }
}
