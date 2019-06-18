import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.*;
import java.util.*;
class ReplacementStruct {
   
  private HashSet replaceSet;
  private HashSet regFileSet;
	
  private Hashtable replaceTable;
  private Hashtable regFileTable;

  public ReplacementStruct() {
    replaceTable = new Hashtable();
    replaceSet = new HashSet();
    replaceSet.add("MODIFYVALUE");
    replaceSet.add("STRARRAY");
    replaceSet.add("COLUMN");
    replaceSet.add("OBJARRAY");
    replaceSet.add("COLUMNWIDTH");		
    replaceSet.add("OBJARRAY_INSERTED");		
		
    regFileTable = new Hashtable();
    regFileSet = new HashSet();
    regFileSet.add("DECLARE");
    regFileSet.add("PARAM");
    regFileSet.add("CONSTRUCT");
    regFileSet.add("NATIVEREG");		
    regFileSet.add("NATIVEINIT");
    regFileSet.add("NATIVEARRAY");		
  }	
	
  public void insertIntoStruct(String key, String value) {
    if (replaceSet.contains(key)) {
      replaceTable.put(key, value);

    }		
  }	
	
  public void insertIntoRegFile(String key, String value) {
    if (regFileSet.contains(key)) {
      regFileTable.put(key, value);
    }		
  }
	
  public String getValue(String key) {
    return (String) replaceTable.get(key);	
  }
	
  public String getRegValue(String key) {
    return (String) regFileTable.get(key);
	
  }
}
