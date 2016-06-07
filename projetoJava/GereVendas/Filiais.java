import java.util.Set;
import java.util.TreeSet;
import java.util.stream.Collectors;
/**
 * Write a description of class Filiais here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Filiais implements java.io.Serializable
{
    Filial[] filiais;
    /* ver o que fazer com o construtor padrão - por um numero padrao de filiais*/
    public Filiais(int n){
        filiais = new Filial[n];
        for(int i = 0; i < n; i++){
            filiais[i] = new Filial();
        }
    }
    
    public Filiais(){
        this(Constantes.N_FILIAIS);
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
            clientes.addAll(filiais[i].clientesCompraramMes(mes));
        }
        return clientes.size();       
    }
    
    /* Dado um cliente e um mes, devolve as compras feitas nesse mes pelo Cliente */
    public Set<ComprasDoProduto> comprasFeitasMes(String idCliente, int mes){
        Set<ComprasDoProduto> compras = new TreeSet<>();
        for(int i = 0; i < filiais.length; i++){
            compras.addAll(filiais[i].comprasFeitasMes(idCliente,mes));
        }
        return compras;
    }
    
    public int[] quantasComprasPorMes(String idCliente){
        int[] quantasComprasPorMes = new int[13];
        for(int i = 0; i < filiais.length; i++){
            int[] quantasComprasPorMesPorFilial = filiais[i].quantasComprasPorMes(idCliente);
            for(int j = 1; j < 13; j++){
                quantasComprasPorMes[j] += quantasComprasPorMesPorFilial[j];
            }
        }
        return quantasComprasPorMes;
    }
    
    /* Dado um código de cliente, determinar, para cada mês, quantas compras fez, quantos produtos distintos comprou e quanto gastou no total.*/
    
    public static int quantosProdutosDistintosComprou(Set<ComprasDoProduto> compras){
        Set<String> produtosComprados = compras.stream().map(ComprasDoProduto::getCodigoProduto).collect(Collectors.toSet());
        return produtosComprados.size();
    }
    
    public static double quantoGastou(Set<ComprasDoProduto> compras){
        return compras.stream().mapToDouble(ComprasDoProduto::getFaturacao).sum();
    }
    
}
