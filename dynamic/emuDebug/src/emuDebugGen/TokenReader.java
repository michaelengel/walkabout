import java.io.BufferedReader;
import java.io.Reader;
import java.io.IOException; 

/************************************************************
A very simple token scanner class. 

Produces 4 types of tokens
- General Token (for all the special symbols)
- identToken (for all the string identifier tokens)
- intToken (for the integer tokens)
- errorToken (for syntax errors)

Also returns a null if something goes wrong with the IO reads

The way the TokenReader works:
- next() always gives the next token. However, it will always
  return the same token until a skip() is called
- skip() skips the next token. The next next() call will now
  get the token after the one that was just skipped

*************************************************************/      
class TokenReader extends BufferedReader
{    
  // The following specifies different states in the
  // token reader state diagram. 
  private static final int INITIAL = 0;
  private static final int CHAR_INPUT = 1;
  private static final int DIGIT_INPUT = 2;
  private static final int COMPOUND_SYMBOL = 3;
  
  // A hard limit on the identifier size
  private static final int MAX_IDENTIFIER_SIZE = 128;  
    
  // nextToken is what the next() method will always
  // be returning
  private Token nextToken = null;

  public TokenReader(Reader in) {
    super(in);
  }
    
  // All skip() does is to make the nextToken null.
  // Therefore, the next time next() is called, it will
  // see that it is null and it must then read from the
  // Reader    
  //
  // Without running skip(), next() will always return 
  // the same token everytime it is called.
  public void skip() {    
    nextToken = null;
  }

  // Main function that returns the next token in the from
  // the Reader. 
  public Token next() {

    int intValue = 0;    
    
    // Setting initial state to be INITIAL
    int currentState = INITIAL;
    
    // Compound symbols currently not implemented    
    // char compoundSymbol_first = 0;
    char identifierBuffer[] = new char[MAX_IDENTIFIER_SIZE];
    int identifierOffset = 0;

    // Used to head the character that is read in
    char c;
    
    // If not null, that means we haven't called a skip yet
    // so we return the existing value	
    if (nextToken != null) {
      return nextToken;
    }

    // Loop forever, we exit this loop by calling a return
    // with the appropriate token                
    while (true) {
	switch (currentState) {
	  case INITIAL:  
	  
	    // First do a read to an int. If the value is -1,
	    // then we've reached the end of the stream and
	    // we just return a null
	    int tempInt = 0;      
	    try {
	      tempInt = read();            
	    } catch (IOException e) {
	      // If stream dies for some reason, just 
	      // complain and return a null
	      System.err.println("IO error!");
	      return null;		    
	    }

	    // End of stream reached, set nextToken
	    // to null and return it		    
	    if (tempInt == -1) {
	      // done			
	      nextToken = null;
	      return nextToken;
	    }		    
	    c = (char) tempInt;
	    if (Character.isWhitespace(c)) {
	      // Skip all white spaces
	    }
	    else if (Character.isLetter(c) || c == '_') {
	      // We know that this is a letter, add it to the
	      // identifier buffer and set the state to CHAR_INPUT
	      identifierBuffer[identifierOffset++] = c;
	      currentState = CHAR_INPUT;
	    }
	    else if (Character.isDigit(c)) {
	      // We know that this is a number, save the number
	      // and set the state to DIGIT_INPUT	    
	      intValue = Character.getNumericValue(c);                            
	      currentState = DIGIT_INPUT;
	    }		
	    else {
	      // We hit an unexpected character, just emit
	      // an errorToken
	      nextToken = new errorToken(c);
	      return nextToken;
	    }		          
	    break;		    
	  case CHAR_INPUT:	  
	    // In charcter input state, keep collecting characters
	    // into the buffer until we hit a non character
	    try {
	      // Mark the buffer stream such that we can go back
	      // in case this is a non-character and belongs to 
	      // a different state
	      //
	      // marking for the whole MAX_IDENTIFIER_SIZE is 
	      // probably not necessary. All we really need is to
	      // mark it for 1 character	     
	      mark(MAX_IDENTIFIER_SIZE);		
	      c = (char) read();     
	    } catch (IOException e){
	      System.err.println("IO error");
	      return null;
	    }
		     			    
	    if (Character.isLetter(c) || c == '_') {
	      // If c is a character, just keep adding it
	      // to the identifierBuffer
	      identifierBuffer[identifierOffset++] = c;
	    }
	    else if (c == ':') {		    
    	      // We now know it is a label
	      //
	      // Don't do reset() since we don't want to keep
	      // the : symbol	      
	      
	      // This is commented out since we shouldn't really
	      // explicity null out the String like this in java
	      // identifierBuffer[identifierOffset] = '\0';			
	      
	      String stringBuffer = new String(identifierBuffer, 0,
					       identifierOffset);
	      identifierOffset = 0;  // Just in case
			
	      nextToken = new labelToken(stringBuffer);
	      return nextToken;
	    }
	    else {
	      try {
	        // The next character doesn't belong in the identToken
		// reset the stream to where it was before reading 
		// this character
		reset();
	      } catch (IOException e) {
		System.err.println("IO exception");
		return null;
	      }
	      // identifierBuffer[identifierOffset] = '\0';			
	      String stringBuffer = new String(identifierBuffer, 0,
					       identifierOffset);
	      identifierOffset = 0; // Just in case
	      
	      // Check whether this String matches any of the 
	      // special symbols in symbolLookup. If it is, 
	      // create a General Token. Else, create an
	      // identifier token
	      if (symbolLookup.matchWith(stringBuffer))
		nextToken = new Token(stringBuffer);
	      else
		nextToken = new identToken(stringBuffer);
	      return nextToken;
	    }
	    break;
	  case DIGIT_INPUT:
	    // Do more or less the same thing as the CHAR_INPUT state
	    // except we are looking for numbers this time
	    try {
	      mark(MAX_IDENTIFIER_SIZE);
	      c = (char) read();                      
	    } catch (IOException e) {
	      System.err.println("IO error");
	      return null;
	    }
	    
	    // If it is a number, apply horner to get the final number
	    if (Character.isDigit(c)) {
	      intValue = intValue * 10 + Character.getNumericValue(c);                            
	    }
	    else {
	      try {
		reset();
	      } catch (IOException e){
		System.err.println("IO error");
		return null;
	      }			
	      // An intToken is generated and returned
	      nextToken = new intToken(intValue);
	      return nextToken;
	    }
	    break;                      
	  }
      }            
  }        
}
