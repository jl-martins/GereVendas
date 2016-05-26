
/**
 * Enumeration class TipoVenda - write a description of the enum class here
 * 
 * @author (your name here)
 * @version (version number or date here)
 */
public enum TipoVenda {
    P, N;

    public static TipoVenda tipoFromChar(char c) throws TipoVendaInvalidoException {
        switch(c){
            case 'P': return P;
            case 'N': return N;
            default : throw new TipoVendaInvalidoException("Tipo de Venda Invalido");
        }            
    }

    public static TipoVenda tipoFromString(String str) throws TipoVendaInvalidoException
    {
        /*Mais Exceptions!?*/
        TipoVenda tipo = null;
        String aux = null;
        try{
            aux = str.trim().toUpperCase();	
            if(aux.length()>1) throw new TipoVendaInvalidoException("");
            tipo = tipoFromChar(aux.charAt(0));
        }catch(IllegalArgumentException | NullPointerException | TipoVendaInvalidoException  e){throw new TipoVendaInvalidoException("O tipo de apartamento '" + str + "' é inválido!");}
        return tipo;
    }
}
