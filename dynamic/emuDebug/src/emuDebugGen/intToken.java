class intToken extends Token
{
    private int tokenIntValue;
        
    public intToken (int intValue) {
        tokenType = symbolLookup.aNumber;
        tokenIntValue = intValue;
    }
    
    public int returnTokenValue() {
        return tokenIntValue;
    }        
}
