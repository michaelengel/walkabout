class identToken extends Token
{
    private String tokenStringValue;
        
//    public identToken(){}
    public identToken (String inString) {
        tokenType = symbolLookup.aIdentifier;
        tokenStringValue = inString;            
    }    
    
    public String returnTokenValue() {
        return tokenStringValue;
    }
}
