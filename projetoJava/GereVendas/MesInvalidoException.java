/**
 * Classe de excepções atiradas quando um método de Hipermercado recebe um mês inválido.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class MesInvalidoException extends Exception {
    public MesInvalidoException(String msg){ super(msg); }
}
