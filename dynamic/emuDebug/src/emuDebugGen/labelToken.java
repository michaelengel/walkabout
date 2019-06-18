class labelToken extends Token
{
    private String tokenStringValue;
        
    public labelToken (String inString) {
        tokenType = symbolLookup.aLabel;
        tokenStringValue = inString;            
    }    
    
    public String returnTokenValue() {
        return tokenStringValue;
    }
}
