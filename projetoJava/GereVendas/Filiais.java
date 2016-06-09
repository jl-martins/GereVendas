import java.util.Set;
import java.util.Map;
import java.util.List;
import java.util.Comparator;
import java.util.Collections;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.stream.Collectors;

public class Filiais implements java.io.Serializable{
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
    public int quantosClientesCompraramMes(int mes){
        Set<String> clientes = new TreeSet<>();
        for(int i = 0; i < filiais.length; i++){
            clientes.addAll(filiais[i].clientesCompraramMes(mes));
        }
        return clientes.size();       
    }

    public int[] quantosClientesPorMes(){
        int[] resultados = new int[Constantes.N_MESES+1];
        for(int i = 1; i < Constantes.N_MESES; i++)
            resultados[i] = quantosClientesCompraramMes(i);
        return resultados;
    }

    /* Dado um cliente e um mes, devolve as compras feitas nesse mes pelo Cliente */
    public Set<ComprasDoProduto> comprasFeitasMes(String idCliente, int mes){
        Set<ComprasDoProduto> compras = new TreeSet<>();
        for(int i = 0; i < filiais.length; i++){
            compras.addAll(filiais[i].comprasFeitasMes(idCliente,mes));
        }
        return compras;
    }

    /* query 5 - apagar este comentario */

    public List<ComprasDoProduto> comprasFeitasTotal(String idCliente){
        List<ComprasDoProduto> compras = new ArrayList<>();
        for(int i = 1; i < 13; i++){
            compras.addAll(comprasFeitasMes(idCliente, i));
        }
        return compras;        
    }

    public List<ParProdQtd> produtosMaisComprados(List<ComprasDoProduto> compras){
        Comparator<ParProdQtd> compCompras =
            (p1, p2) -> {
                if(p1.getQtd() > p2.getQtd()) return -1;
                if(p1.getQtd() < p2.getQtd()) return 1;
                else return p1.getProd().compareTo(p2.getProd()); /* verificar para string a null */
            }; 

        Map<String, List<ComprasDoProduto>> comprasPorProduto = compras.stream().collect(Collectors.groupingBy(ComprasDoProduto::getCodigoProduto));
        List<ParProdQtd> resultado = new ArrayList<>();
        for(Map.Entry<String, List<ComprasDoProduto>> e : comprasPorProduto.entrySet()){
            int quantidadeVendida = e.getValue().stream().mapToInt(ComprasDoProduto::getUnidadesCompradas).sum();
            resultado.add(new ParProdQtd(e.getKey(),quantidadeVendida));
        }
        Collections.sort(resultado, compCompras);
        return resultado;
    }

    /* */
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

    // Query4
    public int quantosCompraramProdutoMes(String codigoProduto, int mes){
        Set<String> clientesCompraram = new TreeSet<>();

        for(int i = 0; i < filiais.length; ++i)
            clientesCompraram.addAll(filiais[i].clientesCompraramProdutoMes(codigoProduto, mes));

        return clientesCompraram.size();
    }

    // Query6
    public int quantosCompraramProduto(String codigoProduto){
        Set<String> clientesCompraram = new TreeSet<>();

        for(int i = 0; i < filiais.length; ++i)
            clientesCompraram.addAll(filiais[i].clientesCompraramProduto(codigoProduto));

        return clientesCompraram.size();
    }

    // Query7
    public ParCliFat[] tresMaioresCompradores(int f){ // VER SE VALE A PENA ATIRAR FilialInvalidaException
        return filiais[f-1].tresMaioresCompradores();
    }
    
    // Query8
    public List<ParCliProdsDif> clisCompraramMaisProdsDif(int X){
        List<Map<String, Set<String>>> mapsParciais = new ArrayList<>(filiais.length);
        Map<String, Set<String>> mapGeral = new HashMap<>(); // map de codigo de cliente para conjunto de produtos comprados pelo cliente

        Comparator<Map.Entry<String, Set<String>>> ordemDecNumProdsDif =
            (e1, e2) -> {
                int n1 = e1.getValue().size();
                int n2 = e2.getValue().size();
                return n1 < n2 ? 1 : n1 > n2 ? -1 : e1.getKey().compareTo(e2.getKey());
            };
        
        for(int i = 0; i < filiais.length; ++i)
            mapsParciais.add(filiais[i].mapClienteProdsComp());
    
        for(Map<String, Set<String>> m : mapsParciais){
            for(Map.Entry<String, Set<String>> e : m.entrySet()){
                String codigoCliente = e.getKey();
                Set<String> prodsDif = mapGeral.get(codigoCliente);
    
                if(prodsDif == null) // o cliente ainda nao esta no mapGeral
                    mapGeral.put(codigoCliente, e.getValue());
                else
                    prodsDif.addAll(e.getValue());
            }
        }
        return mapGeral.entrySet()
                       .stream()
                       .sorted(ordemDecNumProdsDif)
                       .limit(X)
                       .map(e -> new ParCliProdsDif(e.getKey(), e.getValue().size()))
                       .collect(Collectors.toCollection(ArrayList :: new));
    }

    // Query9
    public List<ParCliFat> clientesMaisCompraram(String codigoProduto, int X){
        List<Map<String, ParQtdValor>> mapsParciais = new ArrayList<>(filiais.length);
        Map<String, ParQtdValor> mapGeral;
        Comparator<Map.Entry<String, ParQtdValor>> ordemDecQtd =
           (e1,e2) -> {
               int qtd1 = e1.getValue().getQtd();
               int qtd2 = e2.getValue().getQtd();
               return qtd1 < qtd2 ? 1 : qtd1 > qtd2 ? -1 : e1.getKey().compareTo(e2.getKey());
           };
           
        int i;
        for(i = 0; i < filiais.length; ++i)
            mapsParciais.add(filiais[i].mapClienteParQtdValor(codigoProduto));
    
        mapGeral = mapsParciais.get(0); // comeca a construir o mapGeral a partir do primeiro map parcial
        for(i = 1; i < filiais.length; ++i){
            for(Map.Entry<String, ParQtdValor> e : mapsParciais.get(i).entrySet()){
                ParQtdValor par = mapGeral.get(e.getKey());
    
                if(par == null)
                    mapGeral.put(e.getKey(), par);
                else
                    par.adiciona(e.getValue()); // incrementa os campos do parQtdValor que ja esta no mapGeral
            }
        }
        return mapGeral.entrySet()
                       .stream()
                       .sorted(ordemDecQtd)
                       .limit(X)
                       .map(e -> new ParCliFat(e.getKey(), e.getValue().getValor()))
                       .collect(Collectors.toCollection(ArrayList :: new));
    }
}
