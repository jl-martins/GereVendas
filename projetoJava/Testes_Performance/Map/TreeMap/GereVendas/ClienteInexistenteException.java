
/**
 * Classe de exceções atiradas quando um cliente não está registada num Hipermercado.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class ClienteInexistenteException extends Exception {
    public ClienteInexistenteException(String msg){ super(msg); }
}
