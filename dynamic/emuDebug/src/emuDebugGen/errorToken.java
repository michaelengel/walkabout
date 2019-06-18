class errorToken extends Token
{
    private char errorChar;
    public errorToken(char c)
    {
        tokenType = symbolLookup.aError;
        errorChar = c;        
    }
    
    public char returnTokenValue() {
        return errorChar;
    }
} 
