
/**
 * Enumeration class TipoVenda - write a description of the enum class here
 * 
 * @author (your name here)
 * @version (version number or date here)
 */
public enum TipoVenda {
    P, N;
    
    public TipoVenda tipoFromChar(char c) throws TipoVendaInvalidoException {
        switch(c){
            case 'P': return P;
            case 'N': return N;
            default : throw new TipoVendaInvalidoException("Tipo de Venda Invalido");
        }            
    }
    
    /* fazer para ler de strings */
}
