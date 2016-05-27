
/**
 * Enumeration class TipoVenda - write a description of the enum class here
 * 
 * @author (your name here)
 * @version (version number or date here)
 */
public enum TipoVenda {
    P, N;

    public static TipoVenda fromChar(char c) throws TipoVendaInvalidoException {
        return fromString("" + c);
    }

    public static TipoVenda fromString(String str) throws TipoVendaInvalidoException
    {
        TipoVenda tipo = null;

        try{
            tipo = valueOf(str.trim().toUpperCase());	
        }catch(IllegalArgumentException | NullPointerException e){throw new TipoVendaInvalidoException("O tipo de apartamento '" + str + "' é inválido!");}
        return tipo;
    }
}
