
/**
 * Classe de exceções atiradas quando um produto não consta no hipermercado.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class ProdutoInexistenteException extends Exception {
    public ProdutoInexistenteException(String msg){ super(msg); }
}
