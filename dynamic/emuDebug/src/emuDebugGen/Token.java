class Token
{
    protected String tokenType;     
            
    public Token() {}                                    
    public Token(String inTokenType) {
        tokenType = inTokenType;
    }                       
            
    public String returnTokenType() {
        return tokenType;
    }                   
} 
