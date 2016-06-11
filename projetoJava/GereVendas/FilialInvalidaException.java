/**
 * Classe de excepções atiradas quando um método de Hipermercado recebe uma filial inválida.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class FilialInvalidaException extends Exception {
    public FilialInvalidaException(String msg) { super(msg); }
}
