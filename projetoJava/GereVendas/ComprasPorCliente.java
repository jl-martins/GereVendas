import java.util.Map;
import java.util.Set;
import java.util.List;

import java.util.Arrays;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.HashSet;

import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * 
 */
public class ComprasPorCliente implements java.io.Serializable{
    /** Código do cliente para o qual guardamos dados das compras */
    private String codigoCliente;

    /** Lista de 12 Maps, 1 por mês, que a cada produto comprado nesse mês faz corresponder o respetivo ComprasDoProduto. */
    private List<Map<String, ComprasDoProduto>> comprasPorMes;

    /** Array de 13 elementos, que ao indice i faz corresponder o número de compras que o cliente fez no mês i */
    private int[] quantasComprasPorMes;

    /** Construtor por omissão (declarado como private para que o utilizador não o possa invocar) */
    private ComprasPorCliente(){}

    /** 
     * Constrói uma nova ComprasPorCliente para o cliente passado como argumento, sem informação de compras associada a ela.
     * @param codigoCliente Código do cliente para o qual queremos criar uma ComprasPorCliente
     */
    public ComprasPorCliente(String codigoCliente){
        this.codigoCliente = codigoCliente;
        comprasPorMes = new ArrayList<>(13);
        quantasComprasPorMes = new int[13];

        for(int i = 0; i < 13; i++){
            comprasPorMes.add(new HashMap<>());
        }
    }

    /** 
     * Constrói uma cópia do ComprasPorCliente passado como argumento.
     * @param c ComprasPorCliente a copiar.
     */
    public ComprasPorCliente(ComprasPorCliente c){
        this.codigoCliente = c.codigoCliente;
        this.comprasPorMes = new ArrayList<>(13);
        this.quantasComprasPorMes = Arrays.copyOf(c.quantasComprasPorMes, c.quantasComprasPorMes.length);
        for(int i = 0; i < 13; i++){
            this.comprasPorMes.add(copiaMap(c.comprasPorMes.get(i)));
        }     
    }

    /** Faz uma deep-copy de um Map<String, ComprasDoProduto>. 
     * @param map Map<String, ComprasDoProduto> a ser copiado.
     * @return Cópia do Map<String, ComprasDoProduto> passado como argumento.
     */
    private static HashMap<String, ComprasDoProduto> copiaMap(Map<String, ComprasDoProduto> map){
        HashMap<String, ComprasDoProduto> novoMap = new HashMap<>();
        map.forEach((key, value) -> novoMap.put(key, value.clone()));
        return novoMap;
    }

    /** 
     * Devolve o código do cliente a que este ComprasPorCliente diz respeito.
     * @return Código do cliente a que este ComprasPorCliente diz respeito. 
     */
    public String getCodigoCliente(){ 
        return codigoCliente;
    }

    /**
     * Devolve um array de 13 elementos, que ao indice i faz corresponder o número de compras que o cliente fez no mês i.
     * @return Array de 13 elementos, que ao indice i faz corresponder o número de compras que o cliente fez no mês i.
     */
    public int[] getQuantasComprasPorMes(){
        int[] ret = new int[13];
        System.arraycopy(quantasComprasPorMes, 1, ret, 1, 12);
        return ret;
    }

    /**
     * Regista os dados de uma venda neste ComprasPorCliente.
     * @param v Venda a registar.
     */
    public void registaVenda(Venda v){
        int mes = v.getMes();
        String codigoProduto = v.getCodigoProduto();
        quantasComprasPorMes[mes]++;
        Map<String, ComprasDoProduto> aux = comprasPorMes.get(mes);
        ComprasDoProduto comprasProduto = aux.get(codigoProduto);

        if(comprasProduto == null){
            comprasProduto = new ComprasDoProduto(codigoProduto);
            aux.put(codigoProduto, comprasProduto);
        }

        comprasProduto.registaVenda(v);
    }

    /** 
     * @return @c true se o cliente desta ComprasPorCliente comprou no mês passado como argumento.
     */
    public boolean comprouNoMes(int mes){
        return quantasComprasPorMes[mes] > 0;
    }

    /**
     * Devolve uma lista de ComprasDoProduto que corresponde aos dados de todos os produtos que o cliente comprou.
     * @param mes Mês para o qual queremos obter dados.
     * @return Conjunto das ComprasDoProduto que corresponde aos dados de todos os produtos que o cliente comprou
     */
    public Set<ComprasDoProduto> comprasPorMes(int mes){
        Map<String, ComprasDoProduto> mapAux = comprasPorMes.get(mes);
        return mapAux.values().stream().map(ComprasDoProduto::clone).collect(Collectors.toCollection(HashSet::new));        
    }

    /** 
     * Verifica se o cliente comprou um determindao produto num determinado mês.
     * @param codigoProduto Código do produto que queremos verificar se foi comprado.
     * @param mes Mês em que queremos verificar se o cliente comprou o produto.
     * @return @c true se o cliente desta ComprasPorCliente comprou o produto no mês pretendido.
     */
    public boolean comprouProdutoMes(String codigoProduto, int mes){
        return comprasPorMes.get(mes).containsKey(codigoProduto);
    }

    /** 
     * Verifica se o cliente comprou um determinado produto.
     * @param codigoProduto Código do produto que queremos verificar se foi comprado.
     * @return @c true se o cliente desta ComprasPorCliente comprou o produto.
     */
    public boolean comprouProduto(String codigoProduto){
        boolean comprou = false;

        for(int mes = 1; mes <= Constantes.N_MESES && !comprou; ++mes)
            comprou = comprouProdutoMes(codigoProduto, mes);

        return comprou;
    }

    /**
     * Calcula o total gasto nas compras que o cliente fez ao longo do ano.
     * @return Total gasto nas compras que o cliente fez ao longo do ano.
     */
    public double totalGastoAno(){
        double total = 0.0;

        for(Map<String, ComprasDoProduto> map : comprasPorMes)
            for(ComprasDoProduto cdp : map.values())
                total += cdp.getFaturacao();

        return total;
    }

    /**
     * Devolve conjunto dos códigos que o cliente comprou ao longo de todo o ano.
     * @return Conjunto dos códigos que o cliente comprou ao longo de todo o ano.
     */
    public Set<String> produtosCompradosAno(){
        Set<String> produtosComprados = new TreeSet<>();

        for(Map<String, ComprasDoProduto> map : comprasPorMes)
            produtosComprados.addAll(map.keySet());

        return produtosComprados;
    }

    /**
     * Devolve um ParQtdValor com a quantidade total que o cliente comprou e o total que o cliente gastou num produto.
     * @param codigoProduto Produto sobre o qual pretendemos obter informações.
     * @return ParQtdValor com a quantidade total que o cliente comprou e o total que o cliente gastou num produto.
     */
    public ParQtdValor totalCompradoEgasto(String codigoProduto){
        int qtdTotal = 0;
        double valorTotal = 0.0;

        for(Map<String, ComprasDoProduto> map : comprasPorMes){
            ComprasDoProduto compraMes = map.get(codigoProduto);

            if(compraMes != null){ // o cliente comprou o produto no mes a que o Map<String, ComprasDoProduto> atual diz respeito
                qtdTotal += compraMes.getUnidadesCompradas();
                valorTotal += compraMes.getFaturacao();
            }
        }
        return new ParQtdValor(qtdTotal, valorTotal);
    }

    public ComprasPorCliente clone(){
        return new ComprasPorCliente(this);
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Código de cliente: ");
        sb.append(codigoCliente + "\n");
        for(int i = 1; i < 13; i++){
            sb.append("Compras no mes" + i + ": " + quantasComprasMes[i]+ "\n");
        }
        /*   ** acabar */
    }

    public int hashCode(){
        return 0;
    }

    public boolean equals(Object o){
        return true;
    }

    /* definir compareTo a ordenar por nome */
}
