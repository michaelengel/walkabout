/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// Swing packages
import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.table.DefaultTableModel;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.TableModelListener;
import javax.swing.event.TableModelEvent;

// General packages
import java.util.*;
import java.io.*;
import java.lang.*;
import java.net.*;

public class emuDebug {
    private static String[] savedArgs;
  
    /******************************************************
    The following are the socket port number to the emulator
    process 
    ******************************************************/
    private static final int emulatorPort = 4445;

    /******************************************************
    The following fields are just sockets and Object streams
    for the main GUI process to communicate with the 
    emulator process 
    ******************************************************/
    private static Socket emuSocket = null ;
    private static ObjectOutputStream objOutEmu = null; 
    private static ObjectInputStream objInEmu = null;
  
    /*******************************************************
    Main GUI frames 
    *******************************************************/
    private JFrame disFrame;
    private JFrame commandFrame;
    private JFrame registerFrame;
    private JFrame breakPointFrame;
    private JFrame traceFrame;

    /*******************************************************
    Components within each GUI frame
    *******************************************************/                
    private DisComponents disComp;
    private CommandComponents commandComp;
    private RegComponents regComp;
    private BreakPointComponents breakComp;
    private TraceComponents traceComp;    
    
    // This component is actaully created in BreakPointComponents
    private OtherRegComponents otherRegComp;
    

        
    /******************************************************* 
    Global GUI components that are needed by multiple frames
    Easiest to put it in the emuDebug class
    ********************************************************/        
    // breakList needed in both disComp and breakComp
    private JList breakList;
    private HashSet breakPointSet;        

    /*******************************************************
    Used by the other GUI componets to get the disAsm output
    that emuDebug class gets initially
    *******************************************************/               
    private static String disAsm; 
          
    public static String returnDisAsm() {
        return disAsm;
    }


    /*******************************************************
    createServerConnection -> Used to initially create the 
                              connection socket to the 
                              emuProcess class which is 
                              executed separated in a 
                              different process
                              
    connectToEmu           -> Used to connect to the emuProces                              
                              after the connection socket is 
                              created                              
    *******************************************************/                       
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
  
    public static void connectToEmu() {
        int MAX_RETRY = 10000000;
    
        //    emuDebug.childProcess = null;

        StringBuffer tempArg = new StringBuffer();
    
        for (int i = 0; i < emuDebug.savedArgs.length; i++) {
            tempArg.append(emuDebug.savedArgs[i]);
            if (i != emuDebug.savedArgs.length - 1)
                tempArg.append(" ");
        }
        while (true) {
            try {
                emuDebug.emuSocket = new Socket("localhost", emuDebug.emulatorPort);
                // Must be output first or else deadlock
                emuDebug.objOutEmu = new ObjectOutputStream(emuDebug.emuSocket.getOutputStream());             
                emuDebug.objInEmu = new ObjectInputStream(emuDebug.emuSocket.getInputStream());  
                break;
            } catch (IOException ie) {}
        }  
    }   
    
        
    /********************************************************
    The following set of functions ending in Emu are wrapper
    functions that send data from the GUI process to the 
    emulator process

    quitEmu                  -> Tells the emulator that the 
                                program is exiting
    disasmEmu                -> Tell the emulator to disAsm 
                                a PC and return the value back
    restartEmu               -> Tell the emulator to restart 
                                itself
    returnRegEmu             -> Tell emulator to return the
                                full register state
    returnPCEmu              -> Tell emulator to return the
                                current PC             
    executeOneInstructionEmu -> Tell emulator to execute one
                                instruction
    executeUntilBreakEmu     -> Tell emulator to execute 
                                until breakpoint is reached
                                and pass the breakpoint 
                                hashset to the emulator
    ********************************************************/  
    private static void quitEmu() {
        try {
            emuDebug.objOutEmu.writeUTF("quit");
            emuDebug.objOutEmu.flush();        
        } catch (Exception e) {        
            System.err.println("quitEmu error : " + e);        
        }  
    }

    private static String disasmEmu(int inPC) {
        String inString = null;
        try {
            emuDebug.objOutEmu.writeUTF("disasm");                
            emuDebug.objOutEmu.flush();
            emuDebug.objOutEmu.writeInt(inPC);
            emuDebug.objOutEmu.flush();                                
            inString = emuDebug.objInEmu.readUTF();
        } catch (Exception e) {
            System.err.println("disasmEmu error: " + e);
        }
        return inString; 
    }
  
  
    private static void restartEmu() {
        try {
            emuDebug.objOutEmu.writeUTF("restart");
            emuDebug.objOutEmu.flush();
            // emuDebug.childProcess.destroy();
            emuDebug.objOutEmu.close();
            emuDebug.objInEmu.close();
            emuDebug.emuSocket.close();
            emuDebug.connectToEmu();  
        } catch (Exception e) {
            System.err.println("restartEmu error: " + e);        
            try {
                // emuDebug.emuSocket.close();
                emuDebug.connectToEmu();                
            } catch (Exception ee){
                System.err.println("restartEmu trap in trap : " + ee);
            }
                
        }
        
    }
  
    private static RegFile returnRegEmu() {
        RegFile inReg = null;
        try {
            emuDebug.objOutEmu.writeUTF("reg");
            emuDebug.objOutEmu.flush();
            inReg = (RegFile) emuDebug.objInEmu.readObject();                
        } catch (Exception e) {
            System.err.println("returnRegEmu error : " + e);
        }        
        return inReg;  
    }
  
    private static int returnPCEmu() {
        int inPC = -1;
        int MAX_RETRIES = 100;
        for (int i = 0; i < MAX_RETRIES; i++) {
            try {
                emuDebug.objOutEmu.writeUTF("pc");
                emuDebug.objOutEmu.flush();
                inPC = emuDebug.objInEmu.readInt();
                break;
            } catch (Exception e) {
                emuDebug.connectToEmu();                    
                System.err.println("returnPCEmu error : " + e);
            }        
        }
        return inPC;            
    }
  
    private static void executeOneInstructionEmu() {
        try {
            emuDebug.objOutEmu.writeUTF("next");        
            emuDebug.objOutEmu.flush();
        } catch (Exception e) {
            System.err.println("executeOneInstructionEmu error : " + e);
        }
    }
  
    private synchronized static void executeUntilBreakEmu(HashSet inSet){
        try {
            emuDebug.objOutEmu.writeUTF("executeUntilBreak");
            emuDebug.objOutEmu.flush();
            emuDebug.objOutEmu.writeInt(inSet.size());
            emuDebug.objOutEmu.flush();
                
            Iterator setIterator = inSet.iterator();                        
            for (int i = 0; i < inSet.size(); i++) {
                emuDebug.objOutEmu.writeObject(setIterator.next());
                emuDebug.objOutEmu.flush();                
            }
        } catch (Exception e) {
        
            System.err.println("executeUntilBreakEmu error : " + e);
        }          
    }
                 




    /****************************************************************
     This class represents the GUI components for the Other Register
     frame
     
     Complete Rewrite of this Component!  
     July 7, 2001
    *****************************************************************/      
    class OtherRegComponents {
    
        private MyTableModel regModel;
        private JScrollPane regScrollPane;
        private JTable regTable;
    
        // My own TableModel that is not editable
        class MyTableModel extends DefaultTableModel {        
            public boolean isCellEditable(int row, int col) {
                return false;
            }                        
        }    
        
        public OtherRegComponents() {
            
            regModel = new MyTableModel();
            regModel.addColumn("Fields");
            regModel.addColumn("Values");
                
            // String[] strArray = {"pc", "npc", "AF", "CF", "CTI", "CWP", "FGF", "FLF",
            //                      "FSR", "FZF", "NEXT", "NF", "OF", "PSR", "TBR", "WIM", "Y", "ZF"};

	    	String[] strArray = {"AF","CTI","CWP","FGF","FLF","FSR","FZF","NEXT","PSR","TBR","WIM","Y"};

            for (int i = 0; i < strArray.length; i++) {
                Object[] objArray = {strArray[i], " "};
                regModel.addRow(objArray);
            }                                                           
           
            regTable = new JTable(regModel);

            regTable.setPreferredScrollableViewportSize(new Dimension(145, 193));                        
            regTable.getColumnModel().getColumn(0).setPreferredWidth(40);
            regTable.getColumnModel().getColumn(1).setPreferredWidth(80);
                                  
            regScrollPane = new JScrollPane(regTable);                        
        }
        
        public Component returnComponents() {
            return regScrollPane;        
        }
        
        public synchronized void modifyRegValue(RegFile newCC){
	    	regModel.setValueAt(new Integer(newCC.r_AF) , 0, 1);
regModel.setValueAt(new Integer(newCC.r_CTI) , 1, 1);
regModel.setValueAt(new Integer(newCC.r_CWP) , 2, 1);
regModel.setValueAt(new Integer(newCC.r_FGF) , 3, 1);
regModel.setValueAt(new Integer(newCC.r_FLF) , 4, 1);
regModel.setValueAt(new Integer(newCC.r_FSR) , 5, 1);
regModel.setValueAt(new Integer(newCC.r_FZF) , 6, 1);
regModel.setValueAt(new Integer(newCC.r_NEXT) , 7, 1);
regModel.setValueAt(new Integer(newCC.r_PSR) , 8, 1);
regModel.setValueAt(new Integer(newCC.r_TBR) , 9, 1);
regModel.setValueAt(new Integer(newCC.r_WIM) , 10, 1);
regModel.setValueAt(new Integer(newCC.r_Y) , 11, 1);

	    
        }            
    }

    
        
            
                
                    
    /****************************************************************
     This class hold everything that is necessary to parse the output
     of the disasm class and then be able to insert it into the 
     Disasm Table
    *****************************************************************/   
    class DisParser {
        
        private BufferedReader inReader;
        public DisParser(String inString) {
            inReader = new BufferedReader(new StringReader(inString));
                
        }

        public Object[] next() {
                
            try {
                char[] memAddress = new char[8];
                char[] memValue = new char[11];
                String instruction;

                if (inReader.read(memAddress) == -1)
                    return null;
                inReader.skip(2);
                                
                if (inReader.read(memValue) == -1)
                    return null;
                inReader.skip(7);
                instruction = inReader.readLine();


                Object[] objArray = {new Boolean(false), 
                                     new String(memAddress), 
                                     new String(memValue), 
                                     instruction};                                        

                return (objArray);

            } catch (IOException e) {
                return null;
            }
        }
                
    }



                            
    /****************************************************************
     This class holds all the GUI components for the command button 
     frame
    ****************************************************************/
    class CommandComponents {
        
        private JPanel comPanel;
        
        // Action Listener class for the action buttons (i.e. 
        // run, stop, continue, next)
        //
        // This class needs to be cleaned up for *efficiency* later!
        class buttonActionListener implements ActionListener {
            public void actionPerformed(ActionEvent buttonE) {
                int thisPC = 0;
                if (buttonE.getActionCommand() == "run") {                                                                                        
                    emuDebug.restartEmu();
                    emuDebug.executeUntilBreakEmu(breakPointSet);          
                    thisPC = emuDebug.returnPCEmu();
                    if (thisPC ==  -1) 
                        return;
                          
                    RegFile tempReg = emuDebug.returnRegEmu();
          
                    regComp.modifyRegValue(tempReg);                                                
                    otherRegComp.modifyRegValue(tempReg);
                } 
                else if (buttonE.getActionCommand() == "continue") {
                    emuDebug.executeOneInstructionEmu();        
                    thisPC = emuDebug.returnPCEmu();
          
                    if (!breakPointSet.contains(new Integer(thisPC)))
                        emuDebug.executeUntilBreakEmu(breakPointSet);
        
                    thisPC = emuDebug.returnPCEmu();

                    if (thisPC == -1)
                        return;
                                                
                    RegFile tempReg = emuDebug.returnRegEmu();
          
                    regComp.modifyRegValue(tempReg);
                    otherRegComp.modifyRegValue(tempReg);
                }
                else if (buttonE.getActionCommand() == "next") {
                    emuDebug.executeOneInstructionEmu();        
                    thisPC = emuDebug.returnPCEmu();
                    RegFile tempReg = emuDebug.returnRegEmu();
          
                    regComp.modifyRegValue(tempReg);
                    otherRegComp.modifyRegValue(tempReg);                                                
                }
                else if (buttonE.getActionCommand() == "stop") {
                    // Change Semantic of Stop *later* so that it just
                    // greys out other buttons!!
                    emuDebug.restartEmu();
                }
                DisParser disTokens = new DisParser(emuDebug.disasmEmu(thisPC)); 
                if (disTokens == null) 
                    return;
                Object[] objArray = disTokens.next();
                Object[] objArrayMod = new Object[3];
                for (int i = 0; i < 3; i++) 
                    objArrayMod[i] = objArray[i + 1];

                regComp.updateCurrentInstruct(objArrayMod);                                                
                disComp.setSelection(thisPC);                                                                                                
                                           
            }
        }        

        // Creating radio buttons that turns on and off relocation
        private JPanel createRelocRadioPanel() {

            final TitledBorder relocBorder = BorderFactory.createTitledBorder("Relocation :");

            // Have to add an action listener to the radio buttons
            // later when it is actually implemented                                                                                                
            JRadioButton relocOn = new JRadioButton("on");
            relocOn.setActionCommand("relocOn");

            JRadioButton relocOff = new JRadioButton("off");
            relocOff.setActionCommand("relocOff");                        
            relocOff.setSelected(true);
                                                        
            ButtonGroup relocGroup = new ButtonGroup();
            relocGroup.add(relocOn);
            relocGroup.add(relocOff);

            JPanel radioPanel = new JPanel();
            radioPanel.add(relocOn);
            radioPanel.add(relocOff);
                        
            radioPanel.setBorder(relocBorder); 
            return radioPanel;                    
        }                           

        // Creates the action buttons (i.e. run, next, continue, stop)           
        private JPanel createActionPanel() {           

            final TitledBorder buttonBorder = BorderFactory.createTitledBorder("Commands :");
                                   
            buttonActionListener actionCommands = new buttonActionListener();
            
            JButton runButton = new JButton("Run");
            runButton.setActionCommand("run");
            runButton.addActionListener(actionCommands);
            
            JButton nextButton = new JButton("Next");
            nextButton.setActionCommand("next");
            nextButton.addActionListener(actionCommands);
                                                                
            JButton contButton = new JButton("Continue");
            contButton.setActionCommand("continue");
            contButton.addActionListener(actionCommands);
                                                                
            JButton stopButton = new JButton("Stop");
            stopButton.setActionCommand("stop");
            stopButton.addActionListener(actionCommands);
                        
            JPanel buttonPanel = new JPanel();
            buttonPanel.add(runButton);
            buttonPanel.add(nextButton);
            buttonPanel.add(contButton);
            buttonPanel.add(stopButton);                        
                        
            buttonPanel.setBorder(buttonBorder);
            return buttonPanel;
        }                                                                        

        // Public constructor        
        public CommandComponents() {
                                                 
            // Create panel for both relocation radio panel 
            // action button panel and insert into comPanel
            comPanel = new JPanel();
            comPanel.add(createRelocRadioPanel());
            comPanel.add(createActionPanel());                                          
        }
        
        // Return the panel that holds all the components
        // after it is created                
        public Component returnComponents() {
            return comPanel;
        }
                        
    }   
    

                              
                                                                                          
    /************************************************************
     This class holds all the components for the Register frame            
    *************************************************************/    
    class RegComponents {
        
        private JTable regTable;
        private JTable regTableFloat;
        private JTable regTableCC;
        private JTable regTablePC;
        private JTable instructTable;
        private MyTableModel regModel;
        private MyTableModel regModelFloat;
        private MyTableModel regModelCC;
        private MyTableModel regModelPC;
        private MyTableModel instructModel;
        
        private JPanel regPanel;
        

        // My own Table Model class that is not editable                         
        class MyTableModel extends DefaultTableModel {        
            public boolean isCellEditable(int row, int col) {
                return false;
            }
                        
        }
        
        // Used by other classes to update the current instruction 
        // in the current instruction table                                        
        public synchronized void updateCurrentInstruct(Object[] objArray) {
            for (int i = 0; i < 3; i++) {
                instructModel.setValueAt(objArray[i], 0, i);
            }                
                   
        }
                

        private JScrollPane createCurrentInstructPane() {
            final TitledBorder pcBorder = BorderFactory.createTitledBorder("Current Instruction:");                  
                                   
            instructModel = new MyTableModel();
            instructModel.addColumn("Address");
            instructModel.addColumn("Value");
            instructModel.addColumn("Assembly Instruction");                                    
                                                                        
            Object[] thisObjArray = {new String(), new String(), new String ()};                  
            instructModel.addRow(thisObjArray);
                                                                                             
            instructTable = new JTable(instructModel);                  
            instructTable.setPreferredScrollableViewportSize(new Dimension(350, 16));

            instructTable.getColumnModel().getColumn(0).setPreferredWidth(70);
            instructTable.getColumnModel().getColumn(1).setPreferredWidth(80);
            instructTable.getColumnModel().getColumn(2).setPreferredWidth(200);                  
                  
            JScrollPane currentInstruct = new JScrollPane(instructTable);  
            currentInstruct.setBorder(pcBorder);           
                  
            return currentInstruct;                                                                                  
        }
           
        private JPanel createRegWindowPanel() {
            // final TitledBorder regWinBorder = BorderFactory.createTitledBorder("Register Window:");                  
            regModel = new MyTableModel();

            // Labels not displayed because we are not putting table headers

	    	regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
regModel.addColumn(" ");
	    
                        
	    	for (int i = 0; i < 8; i++) {
Integer tempInt = new Integer(i); 
Object[] objArray = {
"%g" + tempInt , "","%i" + tempInt , "","%l" + tempInt , "","%o" + tempInt , ""};
regModel.addRow(objArray);
 }
	    

            regTable = new JTable(regModel);

	    	for (int i = 0; i < 7; i++) {
regTable.getColumnModel().getColumn(i++).setPreferredWidth(40);
regTable.getColumnModel().getColumn(i).setPreferredWidth(80);
}
	    
                  
            JPanel regWinPanel = new JPanel();                   
            regWinPanel.add(regTable); 
            // regWinPanel.setBorder(regWinBorder);           
           
            return regWinPanel;
        }

        private JPanel createProgramCounterPanel() {
            final TitledBorder regWinBorderPC = BorderFactory.createTitledBorder("Program Counters:");                  
            regModelPC = new MyTableModel();

            // Labels not displayed because we are not putting the
            // table right on top of a panel
	    	regModelPC.addColumn(" ");
regModelPC.addColumn(" ");
regModelPC.addColumn(" ");
regModelPC.addColumn(" ");
	    	    
	    	Object [] objArray = {"%pc", "","%npc", ""};
                                         
            regModelPC.addRow(objArray);
            regTablePC = new JTable(regModelPC);

	    	for (int i = 0; i < 3; i++) {
regTablePC.getColumnModel().getColumn(i++).setPreferredWidth(40);
regTablePC.getColumnModel().getColumn(i).setPreferredWidth(80);
}
	    

            JPanel regWinPanel = new JPanel();                   
            regWinPanel.add(regTablePC); 
            regWinPanel.setBorder(regWinBorderPC);           
           
            return regWinPanel;


        }

              
        private JPanel createConditionCodePanel() {
            final TitledBorder regWinBorderCC = BorderFactory.createTitledBorder("Integer Condition Codes:");                  

            regModelCC = new MyTableModel();
	    	regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
regModelCC.addColumn(" ");
                        
	    	Object [] objArray = {"%n", "","%z", "","%o", "","%c", ""};

            regModelCC.addRow(objArray);
            regTableCC = new JTable(regModelCC);
	    	
			for (int i = 0; i < 7; i++) {
regTableCC.getColumnModel().getColumn(i++).setPreferredWidth(25);
regTableCC.getColumnModel().getColumn(i).setPreferredWidth(25);
}
	    
                  
            JPanel regWinPanel = new JPanel();                   
            regWinPanel.add(regTableCC); 
            regWinPanel.setBorder(regWinBorderCC);           
           
            return regWinPanel;
        }

        private JPanel createFloatPanel() {
            // final TitledBorder regWinBorder = BorderFactory.createTitledBorder("Register Window:");                  
            regModelFloat = new MyTableModel();

            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
            regModelFloat.addColumn(" ");
                        
            for (int i = 0; i < 8; i++) {
                Integer tempInt = new Integer(i);
                Integer tempInt8 = new Integer(i + 8);
                Integer tempInt16 = new Integer(i + 16);
                Integer tempInt24 = new Integer(i + 24);
                Object[] objArray = {"%f" +  tempInt,
                                     " ",
                                     "%f" + tempInt8,
                                     " ",
                                     "%f" + tempInt16,
                                     " ",
                                     "%f" + tempInt24,
                                     " "};
                                         
                regModelFloat.addRow(objArray);
            } 

            regTableFloat = new JTable(regModelFloat);

            for (int i = 0; i < 7; i++) {
                regTableFloat.getColumnModel().getColumn(i++).setPreferredWidth(40);
                regTableFloat.getColumnModel().getColumn(i).setPreferredWidth(80);
            }
                  
            JPanel regWinPanel = new JPanel();                   
            regWinPanel.add(regTableFloat); 
                       
            return regWinPanel;
        }
                
        public RegComponents() {                                      
            regPanel = new JPanel(); 
            regPanel.add(createCurrentInstructPane());


            JPanel regMiddlePanel = new JPanel();
            regMiddlePanel.add(createConditionCodePanel());
            regMiddlePanel.add(createProgramCounterPanel());

            regPanel.add(regMiddlePanel);


            JTabbedPane registerTabs = new JTabbedPane();
            registerTabs.addTab("Integer Registers", createRegWindowPanel());
            registerTabs.addTab("Float Registers", createFloatPanel());
     
            regPanel.add(registerTabs);        
            regPanel.setLayout(new BoxLayout(regPanel, BoxLayout.Y_AXIS));
        }
            
        public Component returnComponents() {
            return regPanel;            
        }
            
        public synchronized void modifyRegValue(RegFile inRegFile) {

			for (int i = 0; i < 8; i++) { 
regModel.setValueAt(Integer.toHexString(inRegFile.rd[i + 0]), i, 1); 
regModel.setValueAt(Integer.toHexString(inRegFile.rd[i + 24]), i, 3); 
regModel.setValueAt(Integer.toHexString(inRegFile.rd[i + 16]), i, 5); 
regModel.setValueAt(Integer.toHexString(inRegFile.rd[i + 8]), i, 7); 
}
	
	    	regModelCC.setValueAt(Integer.toHexString(inRegFile.r_NF), 0, 1);
regModelCC.setValueAt(Integer.toHexString(inRegFile.r_ZF), 0, 3);
regModelCC.setValueAt(Integer.toHexString(inRegFile.r_OF), 0, 5);
regModelCC.setValueAt(Integer.toHexString(inRegFile.r_CF), 0, 7);

	    	regModelPC.setValueAt(Integer.toHexString(inRegFile.r_pc), 0, 1);
regModelPC.setValueAt(Integer.toHexString(inRegFile.r_npc), 0, 3);

            // regModelFloat                        
        }       
    }        

        
                
    /***************************************************************
    This class holds all the GUI components for the disasm frame                    
    ****************************************************************/                                    
    class DisComponents {
        
        private JScrollPane disScrollPane;
        private JTable disTable;
        private MyTableModel disModel;
                
        private HashMap pcMap;

        // Taken almost verbatim from Java Swing tutorial        
        class MyTableModel extends DefaultTableModel {

            /*
             * JTable uses this method to determine the default renderer/
             * editor for each cell.  If we didn't implement this method,
             * then the last column would contain text ("true"/"false"),
             * rather than a check box.
             */
            public Class getColumnClass(int c) {
                return getValueAt(0, c).getClass();
            }
    
            /*
             * Don't need to implement this method unless your table's
             * editable.
             */
            public boolean isCellEditable(int row, int col) {
                //Note that the data/cell address is constant,
                //no matter where the cell appears onscreen.
                if (col > 0) { 
                    return false;
                } else {
                    return true;
                }
            }
        }        
           
        public synchronized void removeBreakPoint(int inPC) {
            Integer pcInt = new Integer(inPC);
            int pcIndex = ((Integer)(pcMap.get(pcInt))).intValue();  
            breakPointSet.remove(pcInt);            
            disModel.setValueAt(new Boolean(false), pcIndex, 0);          
        }
        
        
        public DisComponents() {

            pcMap = new HashMap();
            breakPointSet = new HashSet();

            disModel = new MyTableModel();                                
            disModel.addColumn("B.P.");
            disModel.addColumn("Address");
            disModel.addColumn("Value");
            disModel.addColumn("Assembly Instruction");
                                                                
            DisParser disTokens = new DisParser(emuDebug.returnDisAsm());

            Object[] objArray;
            int counter = 0;
            while((objArray = disTokens.next())!= null) {
                pcMap.put(Integer.valueOf((String)objArray[1], 16), new Integer(counter++));                        
                disModel.addRow(objArray);
            }
                                                                        
            disTable = new JTable(disModel);
            disTable.setPreferredScrollableViewportSize(new Dimension(390, 500));

            disTable.getColumnModel().getColumn(0).setPreferredWidth(40);
            disTable.getColumnModel().getColumn(1).setPreferredWidth(70);
            disTable.getColumnModel().getColumn(2).setPreferredWidth(80);
            disTable.getColumnModel().getColumn(3).setPreferredWidth(200);        

            disTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
                
            disTable.getModel().addTableModelListener(new TableModelListener(){                
                    public void tableChanged(TableModelEvent tableE){
                        int firstIndex = tableE.getFirstRow();
                        DefaultListModel tempModel = (DefaultListModel)(breakList.getModel());
                        
                        if (((Boolean)(disTable.getValueAt(firstIndex, 0))).booleanValue()) {                        
                            Integer tempInt = breakComp.addBreakPointAt((String)(disTable.getValueAt(firstIndex, 1)));                                
                            
                            // Just in case
                            if (tempInt != null)
                                breakPointSet.add(tempInt);
                        } 
                        else {
                            Integer tempInt = Integer.valueOf((String)(disTable.getValueAt(firstIndex, 1)), 16);
                            tempModel.removeElement(Integer.toHexString(tempInt.intValue()));
                            breakPointSet.remove(tempInt);
                        }
                                                
                        if (tempModel.getSize() > 0)
                            breakList.setSelectedIndex(0);
                            
                        breakList.repaint();
                    }
                });                                                                                       
                                                        
            disScrollPane = new JScrollPane(disTable);                
        }
                
        public void setSelection(int iPC) {
                        
            Integer SelectIn;
            if ((SelectIn = (Integer)(pcMap.get(new Integer(iPC)))) == null)
                return; // PC not in disasm
                                                                
            int selectIndex = SelectIn.intValue();
                                                
            disTable.setRowSelectionInterval(selectIndex, selectIndex);
                        
            // The 16 is an ugly quick hack for the height of a row                        
            disScrollPane.getViewport().setViewPosition(new Point(0, selectIndex * 16));
            disScrollPane.repaint();                                                                                                                
        }
                
        public Component returnComponents() {
            return disScrollPane;        
        }
    }

    /***************************************************************
    This class holds all the GUI components for the Breakpoint frame                    
    which has since been renamed to the Misc. Frame
    ****************************************************************/  
    class BreakPointComponents {
    
        private JScrollPane breakScrollPane;
        private JPanel breakPanel;
        private JTabbedPane breakTabbedPanel;
        private JTextField breakAddress;        
        private JTextField memTextField;        
        private JTextField pcTextField;    
        
        private JTextField resultTextField;                                   
        private JPanel resultTextPanel;
        
        class gotoLineActionListener implements ActionListener {
            public void actionPerformed(ActionEvent e) {
                String text = pcTextField.getText();
                try {
                    disComp.setSelection((Integer.valueOf(text, 16)).intValue());
                } catch (NumberFormatException newE) {}                                                                                                
            }
                
        }                
        class viewMemActionListener implements ActionListener {
            public void actionPerformed(ActionEvent e) {
                String text = pcTextField.getText();
                // Call native function that get memory value  
            }
                
        }                
                
        class breakActionListener implements ActionListener {
        
            public void actionPerformed(ActionEvent breakE) {
                String thisActionCommand = breakE.getActionCommand();
            
                if (thisActionCommand == "remove break") {
                    // Need to take check mark off of selected PC
                    Object[] objArray = breakList.getSelectedValues();    
                    if (objArray == null)
                        return;
                        
                    for (int i = 0; i < objArray.length; i++) {
                        ((DefaultListModel)(breakList.getModel())).removeElement(objArray[i]);                   
                        try {
                            disComp.removeBreakPoint((Integer.valueOf((String)(objArray[i]), 16)).intValue());                            
                        } catch (Exception notHereE) {}
                    }                        
                } 
                else if (thisActionCommand == "add break at") {
                    String addressValue = breakAddress.getText();
                    addBreakPointAt(addressValue);
                    breakAddress.selectAll();
                    breakAddress.replaceSelection("");                                    
                }
                
                if (breakList.getModel().getSize() > 0)
                    breakList.setSelectedIndex(0);                
            }           
        }        

        public Integer addBreakPointAt(String addressValue) {
        
            Integer intValue = null;
            try {
                                                    
                intValue = Integer.valueOf(addressValue, 16);                            
                DefaultListModel tempModel = (DefaultListModel)(breakList.getModel());    
                
                String hexString = Integer.toHexString(intValue.intValue());
                int modelSize = tempModel.getSize();
                int i = 0; 
                for (i = 0; i < modelSize; i++) {
                
                    String tempString = (String) tempModel.getElementAt(i);
                    if (tempString.equals(hexString)) 
                        break;
                }             
                if (i >= tempModel.getSize()){
                    tempModel.addElement(Integer.toHexString(intValue.intValue()));
					breakPointSet.add(intValue);
					// Currently, adding a breakpoint manually will not create
					// a checkmark in the dis table automatically
				}


                    
            } catch (NumberFormatException numE) {
                System.err.println("Not a valid number");                   
            }                                 
            return intValue;
        }        
        
        private JPanel createPCPanel() {
           
            final TitledBorder pcBorder = BorderFactory.createTitledBorder("Goto PC :");
            final JLabel pcLabel = new JLabel("PC Value(HEX) : ");                  
                      
            pcTextField = new JTextField(10);                        
            pcTextField.addActionListener(new gotoLineActionListener());                  

            JPanel pcPanel = new JPanel();
            pcPanel.add(pcLabel);
            pcPanel.add(pcTextField);
            pcPanel.setBorder(pcBorder);                                
                  
            return pcPanel;
        }
           
        private JPanel createMemPanel() {

            final TitledBorder memBorder = BorderFactory.createTitledBorder("View at Mem Address :");
            final JLabel memLabel = new JLabel("Mem Address(HEX) : ");                      
           
            memTextField = new JTextField(10);
            memTextField.addActionListener(new viewMemActionListener());                 
                 
            JPanel memTextPanel = new JPanel();
            memTextPanel.add(memLabel);
            memTextPanel.add(memTextField);
            
           
            final JLabel resultLabel = new JLabel("Result: ");                                 
            resultTextField = new JTextField(16);
            resultTextField.setEditable(false);
                                                                                              
            JPanel resultTextPanel = new JPanel();
            resultTextPanel.add(resultLabel);
            resultTextPanel.add(resultTextField);
                                    
            JRadioButton memByte = new JRadioButton("Byte");
            memByte.setActionCommand("byte");

            JRadioButton memShort = new JRadioButton("Short");
            memShort.setActionCommand("short");
                  
            JRadioButton memInt = new JRadioButton("Int");
            memInt.setActionCommand("int");
            memInt.setSelected(true);
                  
            JRadioButton memLong = new JRadioButton("Long");
            memLong.setActionCommand("long");
                  
            ButtonGroup memGroup = new ButtonGroup();
            memGroup.add(memByte);
            memGroup.add(memShort);
            memGroup.add(memInt);                     
            memGroup.add(memLong);
                  
            JPanel memRadioPanel = new JPanel();
            memRadioPanel.add(memByte);
            memRadioPanel.add(memShort);
            memRadioPanel.add(memInt);
            memRadioPanel.add(memLong);
                  
            JPanel memPanel = new JPanel();
            memPanel.add(memRadioPanel);
            memPanel.add(memTextPanel);    
            memPanel.add(resultTextPanel);              
            memPanel.setBorder(memBorder); 
            memPanel.setLayout(new BoxLayout(memPanel, BoxLayout.Y_AXIS));
           
            return memPanel;     
        }
        
        public JPanel createBreakButtons() {
        
            breakActionListener thisBreakListener = new breakActionListener();
            
            JButton removeBreak = new JButton("Remove Breakpoint");
            removeBreak.setActionCommand("remove break");
            removeBreak.addActionListener(thisBreakListener);
            
            
            breakList.setFixedCellWidth(80);                       
            breakScrollPane = new JScrollPane(breakList);            
                        
            // Add function later
            JLabel addBreak = new JLabel("Add Break at (HEX) : ");
            breakAddress = new JTextField(10);
            breakAddress.setActionCommand("add break at");
            breakAddress.addActionListener(thisBreakListener);
            
            JPanel addBreakPanel = new JPanel();           
            addBreakPanel.add(addBreak);
            addBreakPanel.add(breakAddress);
            

            JPanel breakButtonsPanel = new JPanel();
            breakButtonsPanel.setLayout(new BorderLayout());                           
            breakButtonsPanel.add(removeBreak, BorderLayout.SOUTH);                       
            breakButtonsPanel.add(breakScrollPane, BorderLayout.CENTER);
            breakButtonsPanel.add(addBreakPanel, BorderLayout.NORTH);           
            
            return breakButtonsPanel;        
        }
     
        public BreakPointComponents() {
        
            breakPanel = new JPanel();
            breakPanel.add(createBreakButtons());            
            breakPanel.setLayout(new BoxLayout(breakPanel, BoxLayout.Y_AXIS));                   
                      
            // Create Advanced Panel components with PC panel
            // that goes to specified PC and Memory Panel that
            // looks at memory addresses                
            JPanel advancePanel = new JPanel();
            advancePanel.add(createPCPanel());
            advancePanel.add(createMemPanel());
            advancePanel.setLayout(new BoxLayout(advancePanel, BoxLayout.Y_AXIS));                   
          
            otherRegComp = new OtherRegComponents();                            
                                                                                        
            breakTabbedPanel = new JTabbedPane();
            breakTabbedPanel.addTab("Breakpoint", breakPanel);            
            breakTabbedPanel.addTab("Advance", advancePanel);
            breakTabbedPanel.addTab("Other Registers", otherRegComp.returnComponents());
        } 
        
        public Component returnComponents() {
            return breakTabbedPanel;            
        }          
    }
    
    
    
    
    class TraceComponents {
    
    	private MyTableModel traceModel;
	private JTable traceTable;
	private JTabbedPane traceTabbedPanel;

        // My own TableModel that is not editable
        class MyTableModel extends DefaultTableModel {        
            public boolean isCellEditable(int row, int col) {
                return false;
            }                        
        } 

        public JPanel createTraceComponents() {
        
            JLabel traceLabel = new JLabel("Entry Points of Traces : ");
    			    
            traceModel = new MyTableModel();
            traceModel.addColumn("Entry Address");
            traceModel.addColumn("Iterations");
	               
            traceTable = new JTable(traceModel);

            traceTable.setPreferredScrollableViewportSize(new Dimension(200, 193));                        
            traceTable.getColumnModel().getColumn(0).setPreferredWidth(120);
            traceTable.getColumnModel().getColumn(1).setPreferredWidth(80);
                                  
            JScrollPane traceScrollPane = new JScrollPane(traceTable);   
            
            JPanel addTracePanel = new JPanel();           
	    addTracePanel.setLayout(new BorderLayout());
            addTracePanel.add(traceLabel, BorderLayout.NORTH);
            addTracePanel.add(traceScrollPane, BorderLayout.CENTER);
	    
	    return addTracePanel;        
        }
     
        public TraceComponents() {                                               
	    traceTabbedPanel = new JTabbedPane();
	    traceTabbedPanel.addTab("Summary", createTraceComponents());
        } 
        
        public Component returnComponents() {
            return traceTabbedPanel;            
        }          
    }              
    
    
    

    /************************************************************
    This class changes the behaviour of exiting the program
    so that it will also kill off the emuProcess (not 
    a complete solution, since shell script will start
    up the process again)                
    *************************************************************/
    class closeWinAdapter extends WindowAdapter {
        public void windowClosing(WindowEvent e) {
            emuDebug.quitEmu();
            System.exit(0);
        }                
    }
    
        
    public emuDebug() {
        // First create and set up the top level containers        
        disFrame = new JFrame("Disasm Ouput");
        commandFrame = new JFrame("Command Window");
        registerFrame = new JFrame("Register Values");
        breakPointFrame = new JFrame("Misc. Window"); 
	traceFrame = new JFrame("Trace Window");  
                        
        disFrame.setLocation(new Point(0,25));
        commandFrame.setLocation(new Point(500,25));
        registerFrame.setLocation(new Point(500,130));
        breakPointFrame.setLocation(new Point(0,570));               
	traceFrame.setLocation(new Point(500, 450));
                                                
        // Add Window Listeners to only commandFrame 
        commandFrame.addWindowListener(new closeWinAdapter());

        // Needed in both disComp and breakComp
        breakList = new JList(new DefaultListModel());
        
        // Generate componets for each Frame                
        disComp = new DisComponents();                
        commandComp = new CommandComponents();
        regComp = new RegComponents();        
        breakComp = new BreakPointComponents();
	traceComp = new TraceComponents();
                                
        disFrame.getContentPane().add(disComp.returnComponents(), BorderLayout.CENTER);        
        disFrame.pack();
        disFrame.setVisible(true);                                                
                
        commandFrame.getContentPane().add(commandComp.returnComponents(), BorderLayout.CENTER);        
        commandFrame.pack();
        commandFrame.setVisible(true);                
        
        registerFrame.getContentPane().add(regComp.returnComponents(), BorderLayout.CENTER);        
        registerFrame.pack();
        registerFrame.setVisible(true);        
        
        breakPointFrame.getContentPane().add(breakComp.returnComponents(), BorderLayout.CENTER);
        breakPointFrame.pack();
        breakPointFrame.setVisible(true);  
	
        traceFrame.getContentPane().add(traceComp.returnComponents(), BorderLayout.CENTER);        
        traceFrame.pack();
        traceFrame.setVisible(true);    	
	                                    
    }
  
       
    public static void main(String[] args) {

        // Must first load the library emulib so that we can 
        // use ours native functions

        System.loadLibrary("emulib");

        emuDebug.savedArgs = args;        
        sparcdisdebug sparcDisasm = disasm.secondMain(args);
        disAsm = sparcDisasm.returnDisasm();
        
        emuDebug.connectToEmu();                
        emuDebug thisEmu = new emuDebug();
        

/*      disAsm = LibDisasm.libMain(args);                
        emuDebug thisEmu = new emuDebug();
*/
    }
}
