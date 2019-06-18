import java.util.HashSet;

// This class holds all the special symbols
class symbolLookup {

    private static HashSet specialSym;
    
    public final static String aIdentifier = "aIdentifier";
    public final static String aNumber = "aNumber";
    public final static String aError = "aError";
    public final static String aLabel = "aLabel";

    static {
    	symbolLookup.specialSym = new HashSet();
	symbolLookup.specialSym.add("ColumnSize");  
	symbolLookup.specialSym.add("RegisterName");  
    }
    
    public static boolean matchWith(String inString) {
	return specialSym.contains(inString);
    }
}
