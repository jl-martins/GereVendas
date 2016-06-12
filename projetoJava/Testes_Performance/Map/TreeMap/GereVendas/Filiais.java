import java.util.Set;
import java.util.Map;
import java.util.List;
import java.util.Comparator;
import java.util.Collections;
import java.util.TreeSet;
import java.util.ArrayList;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.Arrays;

/** 
 * Classe que guarda informações relativas às diversas filiais de um Hipermercado e que permite fazer operações sobre as várias filiais em conjunto.
 * 
 * @author LI3_Grupo1
 * @version 1.1 (06/2016)
 */

public class Filiais implements java.io.Serializable{
    private Filial[] filiais;

    /** Construtor por omissão, que cria por defeito Constantes.N_FILIAIS filiais.*/
    public Filiais(){
        this(Constantes.N_FILIAIS);
    }

    /** 
     * Constrói e inicializa <code>n</code> filiais.
     * @param n Número de filiais guardadas na Filiais.
     */
    public Filiais(int n){
        filiais = new Filial[n];
        for(int i = 0; i < n; i++){
            filiais[i] = new Filial();
        }
    }

    /** 
     * Constrói uma cópia da Filiais passada como argumento.
     * @param f Filiais a ser copiada.
     */
    public Filiais(Filiais f){
        int quantasFiliais = f.quantasFiliais();
        this.filiais = new Filial[quantasFiliais];
        for(int i = 0; i < quantasFiliais; i++){
            this.filiais[i] = f.filiais[i].clone();
        }
    }

    /** 
     * Devolve quantas filiais é que estão guardadas no objeto sobre o qual o método foi invocado.
     * @return Quantas filiais é que estão guardadas.
     */
    public int quantasFiliais(){
        return filiais.length;
    }

    /** Regista os dados de uma venda na filial onde a venda foi feita. */
    public void registaVenda(Venda v){
        int f = v.getFilial();
        filiais[f-1].registaVenda(v);
    }

    /** 
     * Determina o número total de clientes distintos que fizeram compras num determinado mês, em todas as filiais.
     * @param mes Mês para o qual se pretende obter informação.
     * @return Número total de clientes distintos que fizeram compras num determinado mês, em todas as filiais.
     */
    public int quantosClientesCompraramMes(int mes){
        Set<String> clientes = new TreeSet<>();
        for(int i = 0; i < filiais.length; i++){
            clientes.addAll(filiais[i].clientesCompraramMes(mes));
        }
        return clientes.size();       
    }

    /** 
     * Determina, para cada mês, o número total de clientes distintos que fizeram compras nesse mês, em todas as filiais.
     * @return Array de inteiros cujo valor no índice <code>i</code> corresponde
     *         ao número de clientes distintos que compraram no mês <code>i</code>.
     */
    public int[] quantosClientesPorMes(){
        int[] resultados = new int[Constantes.N_MESES+1];
        for(int i = 1; i <= Constantes.N_MESES; i++)
            resultados[i] = quantosClientesCompraramMes(i);
        return resultados;
    }

    /**
     * Devolve uma lista de ComprasDoProduto que contém informação das compras que um 
     * determinado cliente fez num determinado mês, em todas as filiais.
     * @param codigoCliente Código do cliente cuja informação se pretende consultar. 
     * @param mes Mês em que se pretende consultar a informação.
     * @return Lista de ComprasDoProduto com informação das compras do cliente no mês pretendido.
     */
    public List<ComprasDoProduto> comprasFeitasMes(String codigoCliente, int mes){
        List<ComprasDoProduto> compras = new ArrayList<>();
        for(int i = 0; i < filiais.length; i++){
            compras.addAll(filiais[i].comprasFeitasMes(codigoCliente,mes));
        }
        return compras;
    }

    /** 
     * Devolve uma lista de ComprasDoProduto que contém informação sobre as compras que um determinado cliente fez em todas as filiais.
     * @param codigoCliente Código do cliente cuja informação se pretende consultar. 
     * @return Lista de ComprasDoProduto com informação sobre as compras do cliente selecionado.
     */
    public List<ComprasDoProduto> comprasFeitasTotal(String codigoCliente){
        List<ComprasDoProduto> compras = new ArrayList<>();
        for(int i = 1; i <= Constantes.N_MESES; i++){
            compras.addAll(comprasFeitasMes(codigoCliente, i));
        }
        return compras;        
    }

    /** 
     * Devolve uma lista de ParProdQtd, ordenada decrescentemente pela quantidade vendida e, 
     * em casos de quantidades iguais, ordenada pelo código de produto correspondente.
     * @param compras Lista de ComprasDoProduto (estes dados servem de base para calcular a quantidade vendida).
     * @return Lista de ParProdQtd, ordenada decrescentemente pela quantidade vendida e,
     *         em casos de quantidades iguais, ordenada pelo código de produto correspondente.
     */
    public static List<ParProdQtd> produtosMaisComprados(List<ComprasDoProduto> compras){
        Comparator<ParProdQtd> compCompras =
            (p1, p2) -> {
                if(p1.getQtd() > p2.getQtd()){
                    return -1;
                }if(p1.getQtd() < p2.getQtd()){
                    return 1;
                }else {
                    String prod1 = p1.getProd();
                    String prod2 = p2.getProd();
                    return prod1 != null? prod1.compareTo(prod2) : (prod2 == null? 0 : -1);
                }
            }; 

        Map<String, List<ComprasDoProduto>> comprasPorProduto = compras.stream().collect(Collectors.groupingBy(ComprasDoProduto::getCodigoProduto));
        List<ParProdQtd> resultado = new ArrayList<>(comprasPorProduto.size());

        comprasPorProduto.forEach( 
            (key, value) -> {             
                int quantidadeVendida = value.stream().mapToInt(ComprasDoProduto::getUnidadesCompradas).sum();
                resultado.add(new ParProdQtd(key,quantidadeVendida));
            }
        );

        Collections.sort(resultado, compCompras);
        return resultado;
    }

    /**
     * Determina, para cada mês, o número total de compras que um determinado cliente fez em todas as filiais.
     * @param codigoCliente Código do cliente sobre o qual se pretende consultar informação.
     * @return Array de inteiros cujo valor no indice <code>i</code> corresponde ao número total de compras 
     *         que um determinado cliente fez em todas as filiais no mês <code>i</code>.
     */
    public int[] quantasComprasPorMes(String codigoCliente){
        int[] quantasComprasPorMes = new int[Constantes.N_MESES+1];
        for(int i = 0; i < filiais.length; i++){
            int[] quantasComprasPorMesPorFilial = filiais[i].quantasComprasPorMes(codigoCliente);
            for(int j = 1; j <= Constantes.N_MESES; j++){
                quantasComprasPorMes[j] += quantasComprasPorMesPorFilial[j];
            }
        }
        return quantasComprasPorMes;
    }

    /**
     * Dada uma lista de ComprasDoProduto, calcula quantos produtos distintos foram comprados.
     * @param compras Lista de ComprasDoProduto.
     * @return Número de produtos distintos que foram comprados, calculada com base em <code>compras</code>.
     */
    public static int quantosProdutosDistintosComprou(List<ComprasDoProduto> compras){
        Set<String> produtosComprados = compras.stream().map(ComprasDoProduto::getCodigoProduto).collect(Collectors.toSet());
        return produtosComprados.size();
    }

    /**
     * Dada uma lista de ComprasDoProduto, calcula o total gasto (i.e. total faturado do ponto de vista do Hipermercado) nas compras.
     * @param compras Lista de ComprasDoProduto para a qual se pretende calcular o total gasto.
     * @return Total gasto, calculado com base na lista de ComprasDoProduto.
     */
    public static double quantoGastou(List<ComprasDoProduto> compras){
        return compras.stream().mapToDouble(ComprasDoProduto::getFaturacao).sum();
    }

    /**
     * Dado um código de produto e um mês, calcula quantos clientes compraram esse produto num determinado mês, em todas as filiais.
     * @param codigoProduto Código do produto para o qual se pretende calcular quantos clientes o compraram num mês.
     * @param mes Mês para o qual se pretende calcular o número de clientes que compraram o produto.
     * @return Quantos clientes compraram o produto no mês pretendido.
     */
    public int quantosCompraramProdutoMes(String codigoProduto, int mes){
        Set<String> clientesCompraram = new TreeSet<>();

        for(int i = 0; i < filiais.length; ++i)
            clientesCompraram.addAll(filiais[i].clientesCompraramProdutoMes(codigoProduto, mes));

        return clientesCompraram.size();
    }

    /**
     * Dado um código de produto, calcula quantos clientes compraram esse produto em todas as filiais e todos os meses.
     * @param codigoProduto Código do produto para o qual se pretende calcular quantos clientes o compraram.
     * @return Quantos clientes compraram o produto.
     */
    public int quantosCompraramProduto(String codigoProduto){
        Set<String> clientesCompraram = new TreeSet<>();

        for(int i = 0; i < filiais.length; ++i)
            clientesCompraram.addAll(filiais[i].clientesCompraramProduto(codigoProduto));

        return clientesCompraram.size();
    }

    /**
     * Calcula os três maiores compradores de uma dada filial.
     * @param f Filial para a qual se pretende calcular o resultado.
     * @result Array de 3 ParCliFat ordenados decrescentemente pela total gasto pelo cliente.
     */
    public ParCliFat[] tresMaioresCompradores(int f){
        return filiais[f-1].tresMaioresCompradores();
    }

    /**
     * Calcula e devolve o <i>top</i> <code>X</code> de clientes que mais produtos diferentes compraram e
     * devolve-o na forma de uma {@code List<ParCliProdsDif> }, ordenada decrescentemente pelo número de
     * produtos diferentes comprados e, em caso de empate, por ordem alfabética de código de cliente.
     * @param X Número de elementos da lista a devolver.
     * @return Lista de ParCliProdsDif com os <code>X</code> clientes que mais produtos diferentes
     *         compraram e o respetivo número de produtos diferentes comprados, ordenada decrescentemente
     *         por número de produtos distintos adquiridos.
     */
    public List<ParCliProdsDif> clisCompraramMaisProdsDif(int X){
        List<Map<String, Set<String>>> mapsParciais = new ArrayList<>(filiais.length);
        Map<String, Set<String>> mapGeral = new TreeMap<>(); // map de codigo de cliente para conjunto de produtos comprados pelo cliente

        Comparator<Map.Entry<String, Set<String>>> ordemDecNumProdsDif =
            (e1, e2) -> {
                int n1 = e1.getValue().size();
                int n2 = e2.getValue().size();
                return n1 < n2 ? 1 : n1 > n2 ? -1 : e1.getKey().compareTo(e2.getKey());
            };

        for(int i = 0; i < filiais.length; ++i)
            mapsParciais.add(filiais[i].mapClienteProdsComp());
        // Cria um mapeamento geral a partir dos mapeamentos parciais
        for(Map<String, Set<String>> map : mapsParciais){
            map.forEach( (cli, prodsDifParc) -> {
                    Set<String> prodsDifGeral = mapGeral.get(cli);

                    if(prodsDifGeral == null) // o cliente ainda nao está no mapGeral
                        mapGeral.put(cli, prodsDifParc);
                    else
                        prodsDifGeral.addAll(prodsDifParc); // atualiza o valor (conjunto de produtos diferentes comprados) que já está no map geral
                });
        }
        return mapGeral.entrySet()
                       .stream()
                       .sorted(ordemDecNumProdsDif)
                       .limit(X)
                       .map(e -> new ParCliProdsDif(e.getKey(), e.getValue().size()))
                       .collect(Collectors.toCollection(ArrayList :: new));
    }

    /**
     * Calcula e devolve o <i>top</i> <code>X</code> de clientes que mais compraram
     * o produto passado como 1º argumento, ordenando o resultado por ordem decrescente
     * de quantidade comprada. O resultado produzido é uma {@code List<TriploCliQtdGasto> },
     * pelo que além de ser devolvido o código de cliente e o número de unidades que
     * esse cliente comprou, também é retornado o total gasto no produto passado como 1º argumento.
     * @param codigoProduto Código de produto a considerar na consulta.
     * @param X Número de elementos do <i>top</i> de clientes que mais compraram o produto em questão.
     * @return Lista de TriploCliQtdGasto, ordenada decrescentemente pela quantidade gasta
     *         no produto identificado por <code>codigoProduto</code> e, em casos de quantidades
     *         iguais, ordenada por ordem alfabética de códigos de cliente.
     */
    public List<TriploCliQtdGasto> clientesMaisCompraram(String codigoProduto, int X){
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

        mapGeral = mapsParciais.get(0); // começa a construir o map geral a partir do primeiro map parcial
        for(i = 1; i < filiais.length; ++i){
            mapsParciais.get(i).forEach( (cli, parParcial) -> {
                    ParQtdValor parGeral = mapGeral.get(cli);

                    if(parGeral == null)
                        mapGeral.put(cli, parParcial);
                    else
                        parGeral.adiciona(parParcial); // incrementa os campos do parQtdValor que já estava no mapGeral
                });
        }
        return mapGeral.entrySet()
        .stream()
        .sorted(ordemDecQtd)
        .limit(X)
        .map(e -> new TriploCliQtdGasto(e.getKey(), e.getValue().getQtd(), e.getValue().getValor()))
        .collect(Collectors.toCollection(ArrayList :: new));
    }
    
    /**
     * Cria e devolve uma cópia deste Filiais.
     * @return Cópia deste Filiais.
     */
    @Override
    public Filiais clone(){
        return new Filiais(this);
    }
    
    /**
     * Testa se este Filiais é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
        Filiais f = (Filiais) o;
        return Arrays.deepEquals(this.filiais, f.filiais);
    }
    
    /**
     * Gera e devolve uma representação textual deste Filiais.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String ls = System.getProperty("line.separator");
        for(int i = 0; i < filiais.length; i++){
            sb.append("Filial " + (i+1) + ":" + ls);
            sb.append(filiais[i].toString());
            sb.append(ls);
        }
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do hash code deste Filiais.
     * @return Valor do hash code deste Filiais.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(filiais);
    }
}
