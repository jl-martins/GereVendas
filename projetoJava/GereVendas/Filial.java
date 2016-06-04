import java.util.Set;
import java.util.List;
import java.util.ArrayList;
import java.util.TreeSet;

/**
 * Write a description of class Filial here.
 */

public class Filial
{
    private List<Set<ComprasPorCliente>> clientesOrdenados; /* deve ter tamanho 26 para organizar os clientes pelo 1o char do codigo */
    
    public Filial(){
        clientesOrdenados = new ArrayList<>(26);
        for(int i = 0; i < 26; i++){
            clientesOrdenados.add(new TreeSet<>());
        }
    }
}
