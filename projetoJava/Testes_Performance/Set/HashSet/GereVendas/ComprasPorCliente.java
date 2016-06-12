import java.util.Map;
import java.util.Set;
import java.util.List;

import java.util.Arrays;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.HashSet;

import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Classe que guarda informação sobre as compras que um cliente fez, mês a mês.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class ComprasPorCliente implements java.io.Serializable, Comparable<ComprasPorCliente> {
    /** Código do cliente para o qual guardamos dados das compras */
    private String codigoCliente;

    /** 
     * Lista de 13 Maps, 1 por mês, que a cada produto comprado nesse mês faz corresponder o respetivo ComprasDoProduto. O elemento de indice 0 
     * serve apenas de padding para podermos aceder à info dos meses pelo número do mês. 
     */
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

    /**
     * Faz uma deep-copy de um {@ code Map<String, ComprasDoProduto>}. 
     * @param map {@code Map<String, ComprasDoProduto>} a ser copiado.
     * @return Cópia do {@code Map<String, ComprasDoProduto>} passado como argumento.
     */
    private static HashMap<String, ComprasDoProduto> copiaMap(Map<String, ComprasDoProduto> map){
        /* o fator de carga de um HashMap é 0.75 por omissão. Ao inicializarmos a cópia de map  *
         * com o tamanho (int) Math.ceil(map.size() / 0.75) evitamos a realização de rehashing. */
        HashMap<String, ComprasDoProduto> novoMap = new HashMap<>((int) Math.ceil(map.size() / 0.75));
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
     * Testa se o cliente desta ComparasPorCliente comprou no mês passado como parâmetro.
     * @param mes Mês a considerar na verificação.
     * @return @c true se o cliente desta ComprasPorCliente comprou no mês <code>mes</code>.
     */
    public boolean comprouNoMes(int mes){
        return quantasComprasPorMes[mes] > 0;
    }

    /**
     * Devolve uma lista de ComprasDoProduto que corresponde aos dados de todos os produtos que o cliente comprou.
     * @param mes Mês para o qual queremos obter os dados.
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
     * @return <code>true</code> se o cliente desta ComprasPorCliente comprou o produto no mês pretendido.
     */
    public boolean comprouProdutoMes(String codigoProduto, int mes){
        return comprasPorMes.get(mes).containsKey(codigoProduto);
    }

    /** 
     * Verifica se o cliente comprou um determinado produto.
     * @param codigoProduto Código do produto que queremos verificar se foi comprado.
     * @return <code>true</code> se o cliente desta ComprasPorCliente comprou o produto.
     */
    public boolean comprouProduto(String codigoProduto){
        boolean comprou = false;

        for(int mes = 1; mes <= Constantes.N_MESES && !comprou; ++mes)
            comprou = comprouProdutoMes(codigoProduto, mes);

        return comprou;
    }

    /**
     * Calcula o total gasto nas compras que o cliente fez ao longo do ano.
     * @return Total gasto nas compras feitas pelo cliente.
     */
    public double totalGastoAno(){
        double total = 0.0;

        for(Map<String, ComprasDoProduto> map : comprasPorMes)
            for(ComprasDoProduto cdp : map.values())
                total += cdp.getFaturacao();

        return total;
    }

    /**
     * Devolve um {@code Set<String> } com os códigos dos produtos que o cliente comprou ao longo de todo o ano.
     * @return Conjunto dos códigos dos produtos que o cliente comprou ao longo de todo o ano.
     */
    public Set<String> produtosCompradosAno(){
        Set<String> produtosComprados = new HashSet<>();

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

    /**
     * Cria e devolve uma cópia desta ComprasPorCliente.
     * @return Cópia desta ComprasPorCliente.
     */
    @Override
    public ComprasPorCliente clone(){
        return new ComprasPorCliente(this);
    }

    /**
     * Devolve uma representação textual da ComprasPorCliente sobre o qual o método foi invocado.
     * @return Representação textual da ComprasPorCliente sobre o qual o método foi invocado.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String ls = System.getProperty("line.separator");
        sb.append("Código de cliente: ");
        sb.append(codigoCliente + ls);
        for(int i = 1; i < 13; i++){
            sb.append("Quantas compras foram feitas no mês " + i + ": " + quantasComprasPorMes[i]+ ls);
            sb.append("Produtos comprados no mês " + i + ":"+ ls);
            sb.append(comprasPorMes.get(i));
        }
        return sb.toString();
    }

    /**
     * Calcula e devolve o valor do hash code desta ComprasPorCliente.
     * @return Valor do hash code desta ComprasPorCliente.
     */
    @Override
    public int hashCode(){
        int code = Arrays.hashCode(new Object[]{codigoCliente, comprasPorMes});
        code = 31 * code + Arrays.hashCode(quantasComprasPorMes);
        return code;
    }

    /**
     * Testa se esta ComprasPorCliente é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return false;
        ComprasPorCliente c = (ComprasPorCliente) o;
        return ((this.codigoCliente == null)? c.codigoCliente == null : this.codigoCliente.equals(c.codigoCliente)) &&
        Arrays.equals(this.quantasComprasPorMes, c.quantasComprasPorMes) &&
        (this.comprasPorMes == null? c.comprasPorMes == null : this.comprasPorMes.equals(c.comprasPorMes));
    }

    /** Estabelece a ordem natural dos objetos ComprasPorCliente, sendo esta igual à ordem natural dos códigos de Cliente que guardam(Strings) */
    public int compareTo(ComprasPorCliente c){
        if(this.codigoCliente == null){
            if(c.codigoCliente == null)
                return 0;
            else 
                return -1;
        }

        return this.codigoCliente.compareTo(c.codigoCliente);
    }
}
