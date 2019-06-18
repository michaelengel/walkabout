import java.io.*;
import java.util.*;

/********************************************************
CodeBlock class basically parses through the stream of 
tokens from the TokenReader and matches them up with 
"blocks". The way the specification files are organized
are into blocks, with each block being more or less 
independent from each other.

Each block starts with a label, but after the label the
block can have just about any structure

Usually, after the label, the structure of the block 
is a collection "BlockStructs". BlockStructs are 
basically just a simple structure of 2 Tokens that 
holds the most basic unit of information

After matching up a block from the specification, code 
will be generated into the Replacement Struct. What type 
of code will be determined by the type of block. 
*********************************************************/
class CodeBlock {

    // The blockList structure basically holds the
    // individual BlockStructs for all the BlockStructs
    // in a "block"
    private LinkedList blockList;
    
    // The ReplacementStruct holds the final Code 
    // fragments for each Code Block
    private ReplacementStruct replaceStruct;
    
    // If a syntax error occurs while parsing through
    // the tokens, this field will be set to true
    // and can be accessed by the public method
    // syntaxError()
    private boolean syntaxErrorField = false;
    
    // The finishField is set when no more CodeBlocks
    // can be find from the given TokenStream
    private boolean finishedField = false;   
    
    // blockLabel holds the label of the block and 
    // can be accessed by the method returnLabel()     
    private String blockLabel; 
    
    
    // The following matchWith* methods are convenience
    // methods used to make sure the Token is of the
    // the correct subclass of Token         
    public static boolean matchWithIdent(Token inToken) {
        return (inToken.getClass().getName().equals(
            (new identToken("")).getClass().getName()));            
    }             
    
    public static boolean matchWithInt(Token inToken) {
        return (inToken.getClass().getName().equals(
            (new intToken(0)).getClass().getName()));
    }    
    
    public static boolean matchWithLabel(Token inToken) {
        return (inToken.getClass().getName().equals(
            (new labelToken("")).getClass().getName()));
    } 

    public static boolean matchWithToken(Token inToken) {
        return (inToken.getClass().getName().equals(
            (new Token("")).getClass().getName()));
    }

    // Static array number used so that arrayNames can 
    // be kept the same with only a number at the end
    // of the name that differentiates them apart
    private static int arrayNumber = 0;
    
    public static void incrementArray(){        
        arrayNumber++;    
    }
    
    public static int getArrayNumber() {
        return CodeBlock.arrayNumber;
    }
                           
    /*****************************************************
     BlockStructs are basically just a simple structure 
     of 2 Tokens that holds the most basic unit of 
     information. 
     
     BlockStructs itself are usually not created as it
     is just the parent class for other more specialized
     classes
     
     The semantics of the left and right token is not
     defined for the parent class, other than the fact
     that the left token should be on the left hand side
     and the right token is on the right hand side.     
    ******************************************************/
    class BlockStruct {
  
        // Left hand side
        protected Token left;
    
        // Right hand side
        protected Token right;

        // Syntax error encounter while constructing
        // the BlockStruct                
        protected boolean syntaxErrorFieldStruct = false;
        
        // BlockStruct cannot be constructed with
        // the given TokenReader
        protected boolean emptyField = false; 

        // Public method to see whether a syntaxError occured
        public boolean syntaxError() {
            return syntaxErrorFieldStruct;
        }
        // Public method to see whether the struct is Empty                
        // i.e. a struct could not be created from the 
        // token reader
        public boolean isEmpty() {
            return emptyField;
        }
    }
    
    // Child Classes of BlockStruct, basically extended
    // to work with Condition Codes                  
    class ConditionStruct extends BlockStruct{
        
        public ConditionStruct(TokenReader inReader) {
            Token tokenTemp = inReader.next();            

	    // if tokenTemp is null, we're done
            if (tokenTemp == null) {
                emptyField = true;
                return;
            }                                        
            
            if (!CodeBlock.matchWithIdent(tokenTemp)) {
                emptyField = true;
                return;
            }                                                            
	    
            left = tokenTemp;
            inReader.skip();
                        
            tokenTemp = inReader.next();                        
            
	    // if tokenTemp is null, syntax error	    
	    if (tokenTemp == null) {
	        syntaxErrorFieldStruct = true;
		return;	    
	    }

            if (!CodeBlock.matchWithIdent(tokenTemp)) {
                syntaxErrorFieldStruct = true;
                return;
            }                                                                                             
            right = tokenTemp;
            inReader.skip();
        }
        
    }
    
    // Child Classes of BlockStruct, basically extended
    // to work with IntegerRegisters (used for modeling
    // register windows in SPARC)    
    class IntegerRegistersStruct extends BlockStruct {                
        public IntegerRegistersStruct(TokenReader inReader) {
            Token tokenTemp = inReader.next();

	    // if tokenTemp is null, we're done
            if (tokenTemp == null) {
                emptyField = true;
                return;
            } 

            if (!CodeBlock.matchWithIdent(tokenTemp)) {
                emptyField = true;
                return;
            }                                                 
            left = tokenTemp;
            inReader.skip();
                        
            tokenTemp = inReader.next();

	    // if tokenTemp is null, syntax error
	    if (tokenTemp == null) {
	        syntaxErrorFieldStruct = true;
		return;	    
	    }            

            if (!CodeBlock.matchWithInt(tokenTemp)) {
                syntaxErrorFieldStruct = true;
                return;
            }                                                  
            right = tokenTemp;
            inReader.skip();
        }                        
    }
        
    // Child Classes of ConditionStruct, basically 
    // functionally the same as ConditionStruct
    class ProgramCountersStruct extends ConditionStruct {
        
        public ProgramCountersStruct(TokenReader inReader) {
            super(inReader);                
        }
        
    }
    
    // Child Classes of ConditionStruct, basically 
    // functionally the same as ConditionStruct        
    class MiscRegistersStruct extends ConditionStruct {
        
        public MiscRegistersStruct(TokenReader inReader) {
            super(inReader);                
        }
        
    }        
    
    // Returns the syntaxErrorField            
    //
    // Used to determine whether the syntax
    // from the stream of tokens is correct
    public boolean syntaxError() {
        return syntaxErrorField;
    }

    // Returns the label of the block
    public String returnLabel(){
        return blockLabel;
        
    }
    
    // Returns the finishedField
    //
    // Used to determine whether any more 
    // blocks is available from the given
    // token reader
    public boolean finished() {
        return finishedField;
    }      
    
    
    // Returns the ReplacementStruct which holds all
    // the code fragments.         
    public ReplacementStruct returnStruct() {
        return replaceStruct;
    }

    
    // Given a list of BlockStructs, we iterate through and
    // return a String that represents an object array used 
    // for the field names of a table
    //
    // eg: Object[] objArray = {"%pc",
    //                          " ",
    //                          "%npc",
    //                          " "};
    //          
    // Note that not all the Code Blocks will be using 
    // this method to create it's objarray  
    public String createObjArrayString() {
        StringBuffer tempBuffer = new StringBuffer();
                
        tempBuffer.append("Object [] objArray = {");

        ListIterator setIterator = blockList.listIterator();                
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();        
            tempBuffer.append("\"%");
            tempBuffer.append(((identToken)(tempBlockStruct.left)).returnTokenValue());
            tempBuffer.append("\", \"\"");
            if (!setIterator.hasNext()) {
                tempBuffer.append("};");
            }
            else {
                tempBuffer.append(",");
            }
        }                
        return tempBuffer.toString();
    }
    
    // Given a list of BlockStructs, we iterate through and
    // return a String that represents an object array used 
    // for the field names of a table. This one is specialized
    // for the Register Frame
    //
    // eg : for (int i = 0; i < 8; i++) {
    //          Integer tempInt = new Integer(i);
    //          Object[] objArray = {"%g" +  tempInt,
    //                               " ",
    //                               "%i" + tempInt,
    //                               " ",
    //                               "%l" + tempInt,
    //                               " ",
    //                               "%o" + tempInt,
    //                               " "};
    //                                   
    //           regModel.addRow(objArray);
    //        } 
    //
    // Note that this method not only creates the objArray 
    // but also adds it into the regModel (which is not done
    // by the other createObjArray method
    public String createObjArrayForReg(int columnSize) {
        StringBuffer objArrayBuffer = new StringBuffer();
        objArrayBuffer.append("for (int i = 0; i < ");
        objArrayBuffer.append(columnSize);
        objArrayBuffer.append("; i++) {\n");
        objArrayBuffer.append("Integer tempInt = new Integer(i); \n");
        objArrayBuffer.append("Object[] objArray = {\n");                
        ListIterator setIterator = blockList.listIterator();                
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();        
            objArrayBuffer.append("\"%");
            objArrayBuffer.append(((identToken)(tempBlockStruct.left)).returnTokenValue());
            objArrayBuffer.append("\" + tempInt , \"\"");
            if (!setIterator.hasNext()) {
                objArrayBuffer.append("};\n");
            }
            else {
                objArrayBuffer.append(",");
            }
        }
        objArrayBuffer.append("regModel.addRow(objArray);\n }\n");
        return objArrayBuffer.toString();                        
    }    
    
    
    // Given a list of BlockStructs, we find the size of it and
    // return a String that represents many column creation
    // methods for a table.
    //
    // eg : regModelPC.addColumn(" ");
    //      regModelPC.addColumn(" ");
    //      regModelPC.addColumn(" ");
    //      regModelPC.addColumn(" ");    
    public String createColumnString(String regModelName) {
        
        StringBuffer tempBuffer = new StringBuffer();                                
        for (int i = 0; i < blockList.size() * 2; i++) {
            tempBuffer.append(regModelName);
            tempBuffer.append(".addColumn(\" \");\n");                
        }                                            
        return tempBuffer.toString();        

    }

        
    // Give the table name and the desired width of col 1 and col 1,
    // and the size of the list the following type of code is generated 
    // for the table
    //
    // eg : for (int i = 0; i < 7; i++) {
    //         regTableCC.getColumnModel().getColumn(i++).setPreferredWidth(25);
    //         regTableCC.getColumnModel().getColumn(i).setPreferredWidth(25);
    //      }                        
    public String createColumnWidthString(String regTableName, int col1, int col2) {
        StringBuffer tempBuffer = new StringBuffer();
        tempBuffer.append("for (int i = 0; i < ");
        tempBuffer.append(new Integer(blockList.size() * 2 - 1));                
        tempBuffer.append("; i++) {\n");                
        tempBuffer.append(regTableName);
        tempBuffer.append(".getColumnModel().getColumn(i++).setPreferredWidth(");
        tempBuffer.append(new Integer(col1));
        tempBuffer.append(");\n");
        tempBuffer.append(regTableName);
        tempBuffer.append(".getColumnModel().getColumn(i).setPreferredWidth(");
        tempBuffer.append(new Integer(col2));
        tempBuffer.append(");\n");                
        tempBuffer.append("}\n");                                
                
        return tempBuffer.toString();        
    }


    // Give the list of BlockStructs, the following code is generated.
    // The purpose of the code is for updating the tables for the
    // event handlers.
    //
    // eg:  regModelCC.setValueAt(new Integer(inRegFile.r_NF), 0, 1);          
    //      regModelCC.setValueAt(new Integer(inRegFile.r_ZF), 0, 3);          
    //      regModelCC.setValueAt(new Integer(inRegFile.r_OF), 0, 5);          
    //      regModelCC.setValueAt(new Integer(inRegFile.r_CF), 0, 7);  
    //
    // Note not all Code Blocks will use this function to create
    // modify values
    public String createModifyValueString(String regModelName) {
        StringBuffer tempBuffer = new StringBuffer();                
        ListIterator listIterator = blockList.listIterator();                
        int i = 1;
        while (listIterator.hasNext()) {
            tempBuffer.append(regModelName);
            tempBuffer.append(".setValueAt(Integer.toHexString(inRegFile.");                
            tempBuffer.append(((identToken)(((BlockStruct)(listIterator.next())).right)).returnTokenValue());
            tempBuffer.append("), 0, ");
            tempBuffer.append(new Integer(i));
            tempBuffer.append(");\n");
            i = i + 2; // counting odd numbers 1, 3 ,5, ...
        }                                            
        return tempBuffer.toString();                
    }
    
    // Special ModifyValue for IntegerRegisters
    public String createModifyValueStringReg(int columnSize, String regName) {
        StringBuffer modifyValueBuffer = new StringBuffer();
                
        modifyValueBuffer.append("for (int i = 0; i < ");
        modifyValueBuffer.append(columnSize);
        modifyValueBuffer.append("; i++) { \n");
                
        ListIterator setIterator = blockList.listIterator();        
                
        int counter = 1;        
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();        
            modifyValueBuffer.append("regModel.setValueAt(Integer.toHexString(inRegFile.");
            modifyValueBuffer.append(regName + "[i + ");
            modifyValueBuffer.append(((intToken)(tempBlockStruct.right)).returnTokenValue());
            modifyValueBuffer.append("]), i, " + counter + "); \n");
            counter = counter + 2;
        }                
        modifyValueBuffer.append("}\n");    
        return modifyValueBuffer.toString();
    }
    
    // Special ModifyValue for MiscRegisters
    public String createModifyValueStringMisc() {    
        StringBuffer tempBuffer = new StringBuffer();
        
        ListIterator listIterator = blockList.listIterator();                        
        int i = 0;
        while (listIterator.hasNext()) {
            tempBuffer.append("regModel.setValueAt(new Integer(newCC.");
            tempBuffer.append(((identToken)(((BlockStruct)(listIterator.next())).right)).returnTokenValue());
            tempBuffer.append(") , ");
            tempBuffer.append(new Integer(i++));
            tempBuffer.append(", 1);\n");
        }    
        return tempBuffer.toString();
    } 
    

    // Creates strArray for MiscRegisters
    // 
    // eg:   String[] strArray = {"AF", "CTI", "CWP", "FGF", "FLF",
    //                            "FSR", "FZF", "NEXT", "PSR", "TBR", 
    //                            "WIM", "Y"};           
    public String createStrArrayString() {    
        StringBuffer tempBuffer = new StringBuffer();                
        
        ListIterator listIterator = blockList.listIterator();                                
        tempBuffer.append("String[] strArray = {");                
        while (listIterator.hasNext()) {                        
            tempBuffer.append("\"");                
            tempBuffer.append(((identToken)(((BlockStruct)(listIterator.next())).left)).returnTokenValue());
            tempBuffer.append("\"");
            if (listIterator.hasNext())
                tempBuffer.append(",");
            else
                tempBuffer.append("};");                        
        } 
        return tempBuffer.toString();    
    }            

  
    // Used to create the RegFile.java file, this method
    // creates the String for the variable declaration 
    public String createRegFileDeclare(){
        StringBuffer regFileBuffer = new StringBuffer();
        ListIterator setIterator = blockList.listIterator();
    
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();
            regFileBuffer.append("int " + ((identToken)(tempBlockStruct.right)).returnTokenValue() + ";\n");
        }
        return regFileBuffer.toString();
    }
  
  
    // Used to create the RegFile.java, this method
    // creates the String for array variable declarations
    public String createRegFileDeclareArray(int ColumnSize, String regName) {     
        return new String("int " + regName + "[] = new int[" + blockList.size() * ColumnSize + "]; \n");  
    }
   
   
    // Used to create the RegFile.java, this method
    // creates the String for the parameters of the 
    // constructor
    public String createRegFileParam() {
        StringBuffer regFileBuffer = new StringBuffer();
        ListIterator setIterator = blockList.listIterator();
    
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();
                
            // Remember that the last one in the linkedList does not have a comma 
            // added to the end of it!
            if (setIterator.hasNext()) {
                regFileBuffer.append("int I_" + 
                    ((identToken)(tempBlockStruct.right)).returnTokenValue() + ", \n");
            }
            else {
                regFileBuffer.append("int I_" + 
                    ((identToken)(tempBlockStruct.right)).returnTokenValue());
            }
                
        }
        return regFileBuffer.toString();  
    }

    // Used to create the RegFile.java, this method
    // creates the String for the array parameters 
    // of the constructor
    public String createRegFileParamArray(String regName) {     
        return new String("int I_" + regName + "[]");  
    }  
  
    // Used to create the RegFile.java, this method
    // creates the String for the constructor 
    // (variable assignment)
    public String createRegFileConstruct() {
        StringBuffer regFileBuffer = new StringBuffer();
                
        ListIterator setIterator = blockList.listIterator();
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();
            String tokenValue = ((identToken)(tempBlockStruct.right)).returnTokenValue();
            regFileBuffer.append(tokenValue + " = I_" + tokenValue + "; \n");
        }
        return regFileBuffer.toString();  
    }

    // Used to create the RegFile.java, this method
    // creates the String for the constructor  
    // (array assignment)
    public String createRegFileConstructArray(String regName) {
        StringBuffer regFileBuffer = new StringBuffer();
     
        regFileBuffer.append("for (int i = 0; i < I_" + regName + ".length; i++) { \n");
        regFileBuffer.append(regName + "[i] = I_" + regName + "[i];\n }");
        return regFileBuffer.toString();
    }


    // Used to create the parameters used in the emuLib.cc file        
    public String createRegFileNative() { 
        StringBuffer regFileBuffer = new StringBuffer();
        
        ListIterator setIterator = blockList.listIterator();
        while (setIterator.hasNext()) {
            BlockStruct tempBlockStruct = (BlockStruct)setIterator.next();
            String tokenValue = ((identToken)(tempBlockStruct.right)).returnTokenValue();
            regFileBuffer.append("regs." + tokenValue);
            if (setIterator.hasNext()) {
                regFileBuffer.append(",\n");
            }
        }
        return regFileBuffer.toString();   
    }
    

    // Used to create the array parameters used in the emuLib.cc file        
    public String createRegFileNativeArray() { 
        return ("intArray" + CodeBlock.getArrayNumber());
    }  
    
    
    // Used to create the array init in the emuLib.cc file
    public String createRegFileNativeArrayCreate(int columnSize, String regName) {

	    // int arraySize = 32;
	    // jintArray intArray= env->NewIntArray((jsize)arraySize);
	    // env->SetIntArrayRegion((jintArray)intArray, 0,arraySize, regs.rd);
        
        StringBuffer tempBuffer = new StringBuffer();
        
        int arrayNum = CodeBlock.getArrayNumber();
        
        tempBuffer.append("int arraySize");
        tempBuffer.append(arrayNum + " = " + blockList.size() * columnSize + ";\n");
        tempBuffer.append("jintArray intArray");
        tempBuffer.append(arrayNum + " = " + "env->NewIntArray((jsize)arraySize"
                          + arrayNum + ");\n");
        tempBuffer.append("env->SetIntArrayRegion((jintArray)intArray");
        tempBuffer.append(arrayNum + ", 0, arraySize" 
                          + arrayNum + ", regs." + regName + ");\n");

        return tempBuffer.toString();
    }
    
    
    

    // Used to create the init parameters used in the emuLib.cc file      
    // for proper JNI object init  
    public String createRegFileNativeInit() { 
        StringBuffer regFileBuffer = new StringBuffer();
        
        ListIterator setIterator = blockList.listIterator();
        while (setIterator.hasNext()) {
	    setIterator.next();	
	    regFileBuffer.append("I");
        }
        return regFileBuffer.toString();   
    }    

    // Used to create the init parameters used in the emuLib.cc file      
    // for proper JNI object init for arrays         
    public String createRegFileNativeInitArray() {
//        StringBuffer regFileBuffer = new StringBuffer();
        return ("[I");
//        ListIterator setIterator = blockList.listIterator();
//        while (setIterator.hasNext()) {
//	    regFileBuffer.append("[I");
//        }
//        return regFileBuffer.toString();   
    }     
  
    // Ppublic constructor for the CodeBlock class
    public CodeBlock(TokenReader inReader) {
        
        replaceStruct = new ReplacementStruct();
        
        // Get the first token
        Token inToken = inReader.next();
        
        // We can skip here because if inToken is null,
        // we are done anyway
        inReader.skip();            
        
        // inToken is null, this means we're done as 
        // there are no more tokens in the TokenReader        
        if (inToken == null) {
            finishedField = true;
            return;
        }
            
        // Check to make sure it is a labelToken
        // If it is not, then it is a syntax error    
        if (!CodeBlock.matchWithLabel(inToken)) {
            syntaxErrorField = true;
            return;
        }
        
        // Save the block label    
        blockLabel = ((labelToken) inToken).returnTokenValue();        
        
        // Create the linkedList that holds all the BlockStructs        
        // and the temporary code struct that will be used
        blockList = new LinkedList();
        BlockStruct newStruct = null;
               
        if (blockLabel.equals("ConditionCode")){
            // Get as many ConditionStructs as we can from the TokenReader
            while (!(newStruct = new ConditionStruct(inReader)).isEmpty()) {
                if (newStruct.syntaxError()){
                    syntaxErrorField = true;
                    return;
                }                                        
                blockList.add(newStruct);
            }

            // Generate the code fragments and put them in the replaceStruct
            replaceStruct.insertIntoStruct("COLUMN", createColumnString("regModelCC"));                        
            replaceStruct.insertIntoStruct("OBJARRAY", createObjArrayString());                        
            replaceStruct.insertIntoStruct("COLUMNWIDTH", createColumnWidthString("regTableCC", 25, 25));                        
            replaceStruct.insertIntoStruct("MODIFYVALUE", createModifyValueString("regModelCC"));
      
            replaceStruct.insertIntoRegFile("DECLARE", createRegFileDeclare());
            replaceStruct.insertIntoRegFile("PARAM", createRegFileParam());
            replaceStruct.insertIntoRegFile("CONSTRUCT", createRegFileConstruct());   
            replaceStruct.insertIntoRegFile("NATIVEREG", createRegFileNative());   
	    replaceStruct.insertIntoRegFile("NATIVEINIT", createRegFileNativeInit());                             
        }
        else if (blockLabel.equals("IntegerRegisters")){
            
            // Column size value                        
            Integer columnSize = null;    
            
            // Register name value
            String regName = null;
                                    
            // Keep looping to get all the special init values 
            // for the Register Table (columnSize and regName)
            while (true) {                
                // Weird place to have no more tokens, therefore 
                // return witha syntaxError                
                if (inReader.next() == null) {                        
                    syntaxErrorField = true;
                    return;
                }
                
                // Next token is not a special symbol, therefore
                // we're done with the special symbol value init
                if (!CodeBlock.matchWithToken(inReader.next())) {                    
                    break;
                }
                
                // Now determine what special symbol it is
                if (inReader.next().returnTokenType().equals("ColumnSize")) {
                    // Get next token
                    inReader.skip();
                    if (inReader.next() == null) {
                        syntaxErrorField = true;
                        return;
                    }                            
                    
                    if (!CodeBlock.matchWithInt(inReader.next())) {
                        syntaxErrorField = true;
                        return;                                        
                    }
                                                        
                    columnSize = new Integer(((intToken)inReader.next()).returnTokenValue());
                    inReader.skip();
                }                
                else if (inReader.next().returnTokenType().equals("RegisterName")) {
                    // Get next token
                    inReader.skip();
                    if (inReader.next() == null) {
                        syntaxErrorField = true;
                        return;
                    }                            
                    
                    if (!CodeBlock.matchWithIdent(inReader.next())) {
                        syntaxErrorField = true;
                        return;                                        
                    }                
                                        
                    regName = ((identToken)inReader.next()).returnTokenValue();
                    inReader.skip();                                
                }
                else {
                    // We have unexpected special symbols, compiler error
                    syntaxErrorField = true;
                    return;
                }                                                                                                                                        
            }  
            
            if (columnSize == null || regName == null) {
                // Both columnSize and regName must be set
                syntaxErrorField = true;
                return;            
            }

            while (!(newStruct = new IntegerRegistersStruct(inReader)).isEmpty()) {
                if (newStruct.syntaxError()){
                    syntaxErrorField = true;
                    return;
                }                                        
                blockList.add(newStruct);
            }                
                
            replaceStruct.insertIntoStruct("COLUMN", createColumnString("regModel"));
            replaceStruct.insertIntoStruct("OBJARRAY_INSERTED", createObjArrayForReg(columnSize.intValue()));                                
            replaceStruct.insertIntoStruct("COLUMNWIDTH", createColumnWidthString("regTable", 40, 80));                            
            replaceStruct.insertIntoStruct("MODIFYVALUE", createModifyValueStringReg(columnSize.intValue(), regName));        
                  
            replaceStruct.insertIntoRegFile("DECLARE", createRegFileDeclareArray(columnSize.intValue(), regName));
            replaceStruct.insertIntoRegFile("PARAM", createRegFileParamArray(regName));
            replaceStruct.insertIntoRegFile("CONSTRUCT", createRegFileConstructArray(regName));  
            replaceStruct.insertIntoRegFile("NATIVEREG", createRegFileNativeArray());
	        replaceStruct.insertIntoRegFile("NATIVEINIT", createRegFileNativeInitArray());	          
            replaceStruct.insertIntoRegFile("NATIVEARRAY", createRegFileNativeArrayCreate(columnSize.intValue(), regName));
            CodeBlock.incrementArray();

        }        
        else if (blockLabel.equals("ProgramCounter")){
            while (!(newStruct = new ProgramCountersStruct(inReader)).isEmpty()) {
                if (newStruct.syntaxError()){
                    syntaxErrorField = true;
                    return;
                }                                        
                blockList.add(newStruct);
            }            
                
            replaceStruct.insertIntoStruct("COLUMN", createColumnString("regModelPC"));
            replaceStruct.insertIntoStruct("OBJARRAY", createObjArrayString());                        
            replaceStruct.insertIntoStruct("COLUMNWIDTH", createColumnWidthString("regTablePC", 40, 80));                        
            replaceStruct.insertIntoStruct("MODIFYVALUE", createModifyValueString("regModelPC"));                                                                        
      
            replaceStruct.insertIntoRegFile("DECLARE", createRegFileDeclare());
            replaceStruct.insertIntoRegFile("PARAM", createRegFileParam());
            replaceStruct.insertIntoRegFile("CONSTRUCT", createRegFileConstruct());  
            replaceStruct.insertIntoRegFile("NATIVEREG", createRegFileNative());
	    replaceStruct.insertIntoRegFile("NATIVEINIT", createRegFileNativeInit());	                          
        }                                              
            
        else if (blockLabel.equals("MiscRegisters")){
            while (!(newStruct = new MiscRegistersStruct(inReader)).isEmpty()) {
                if (newStruct.syntaxError()){
                    syntaxErrorField = true;
                    return;
                }                                        
                blockList.add(newStruct);
            }            
                
            // Only need STRARRAY and MODIFYVALUE                                       
            replaceStruct.insertIntoStruct("STRARRAY", createStrArrayString());        
            replaceStruct.insertIntoStruct("MODIFYVALUE", createModifyValueStringMisc());                          
            
            replaceStruct.insertIntoRegFile("DECLARE", createRegFileDeclare());
            replaceStruct.insertIntoRegFile("PARAM", createRegFileParam());
            replaceStruct.insertIntoRegFile("CONSTRUCT", createRegFileConstruct());                                          
            replaceStruct.insertIntoRegFile("NATIVEREG", createRegFileNative()); 
	    replaceStruct.insertIntoRegFile("NATIVEINIT", createRegFileNativeInit());	                         
        }
        else {
            syntaxErrorField = true;
            return;
        }
    }           
}
