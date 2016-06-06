import java.util.Set;
import java.util.TreeSet;
/**
 * Write a description of class Filiais here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Filiais
{
    Filial[] filiais;
    /* ver o que fazer com o construtor padrão - por um numero padrao de filiais*/
    public Filiais(int n){
        filiais = new Filial[n];
    }
    
    public Filiais(){
        filiais = new Filial[Constantes.N_FILIAIS];
    }
    
    public int quantasFiliais(){
        return filiais.length;
    }
    
    public void registaVenda(Venda v){
        int f = v.getFilial();
        filiais[f-1].registaVenda(v);
    }

    /* Dado um mês válido, determinar o número total de clientes distintos que as fizeram */
    public int quantosClientesCompraramMes(int mes) throws MesInvalidoException{
        if(mes < 1 || mes > 12)
            throw new MesInvalidoException("O mês inserido é inválido");
        Set<String> clientes = new TreeSet<>();
        for(int i = 0; i < filiais.length; i++){
            clientes.addAll(filiais[i].clientesCompraramMes());
        }
        return clientes.size();       
    }
}
