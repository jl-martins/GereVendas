import java.util.Set;
import java.util.Map;
import java.util.List;
import java.util.Queue;
import java.util.Comparator;
import java.util.Collections;
import java.util.HashSet;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Arrays;
import java.util.stream.Collectors;

/** Classe que guarda informações relativas a uma filial e às compras que nela foram feitas, incluindo dados relativos aos clientes que compraram na filial.
 * 
 * @author LI3_Grupo1
 * @version 1.1 (06/2016)
 */

public class Filial implements java.io.Serializable{
    private List<Map<String, ComprasPorCliente>> clientesOrdenados; /* deve ter tamanho 26 para organizar os clientes pelo 
    1o char do codigo, a cada cliente faz corresponder as suas compras */

    /** 
     * Construtor padrão, cria uma Filial sem nenhum registo de vendas 
     */
    public Filial(){
        clientesOrdenados = new ArrayList<>(26);
        for(int i = 0; i < 26; i++){
            clientesOrdenados.add(new HashMap<>());
        }
    }

    /** 
     * Constrói uma cópia da Filial passada como argumento.
     * @param f Filial a copiar
     */
    public Filial(Filial f){
        this.clientesOrdenados = new ArrayList<>(26);
        for(int i = 0; i < 26; i++){
            Map<String, ComprasPorCliente> copiaMap = copiaMap(f.clientesOrdenados.get(i));
            this.clientesOrdenados.add(copiaMap);
        }
    }

    /** Faz uma deep-copy de um Map<String, ComprasPorCliente>. 
     * @param map Map<String, ComprasPorCliente> a ser copiado.
     * @return Cópia do Map<String, ComprasPorCliente> passado como argumento.
     */
    private static HashMap<String, ComprasPorCliente> copiaMap(Map<String, ComprasPorCliente> map){
        HashMap<String, ComprasPorCliente> novoMap = new HashMap<>();
        map.forEach((key, value) -> novoMap.put(key, value.clone()));
        return novoMap;
    }

    /**
     * Dado um código do cliente, devolve o indíce correspondente na clientesOrdenados.
     * @param codigoCliente Código do Cliente.
     * @return Indíce correspondente na clientesOrdenados.
     * @throws NullPointerException se o codigoCliente for null.
     */
    private static int indiceCorrespondente(String codigoCliente){
        java.util.Objects.requireNonNull(codigoCliente);
        char c = codigoCliente.charAt(0);
        c = Character.toUpperCase(c);
        int indice = ((int) c) - ((int) 'A');
        indice %= 26; /* Neste caso, garantimos que se o código de clientes não começar por uma letra do alfabeto(se passarmos um ficheiro 
                         cujos códigos de cliente começam com números, por ex), conseguimos guardar estes valores */        
        return indice;
    }

    /** 
     * Método auxiliar que associa a um código de cliente a estrutura que contém os dados relativos às suas compras.
     * @param codigoCliente Código do Cliente.
     * @return ComprasPorCliente que contém a info relativa às compras realizadas pelo cliente. Não há perda de encapsulamento porque o método é privado.
     */
    private ComprasPorCliente comprasDoCliente(String codigoCliente){
        int indice = indiceCorrespondente(codigoCliente);
        Map<String, ComprasPorCliente> mapCorrespondente = clientesOrdenados.get(indice);
        ComprasPorCliente compras = mapCorrespondente.get(codigoCliente);
        return compras;
    }

    /**
     * Regista a informação de uma Venda nos dados do Cliente que ao qual a venda foi feita. 
     * @param v Venda a registar.
     */
    public void registaVenda(Venda v){
        String codigoCliente = v.getCodigoCliente();
        int indice = indiceCorrespondente(codigoCliente);
        ComprasPorCliente compras;
        Map<String, ComprasPorCliente> aux = clientesOrdenados.get(indice);

        compras = aux.get(codigoCliente);
        if(compras == null){
            compras = new ComprasPorCliente(codigoCliente);
            aux.put(codigoCliente, compras);
        }

        compras.registaVenda(v);        
    }

    /**
     * Calcula o conjunto de clientes que compraram nesta filial, num determinado mês.
     * @param mes Mês para o qual se pretende calcular o conjunto de clientes.
     * @return Conjunto dos códigos de cliente dos clientes que compraram no mês pedido na filial.
     */
    public Set<String> clientesCompraramMes(int mes){
        Set<String> clientes = new HashSet<>();
        for(int i = 0; i < 26; i++){
            clientes.addAll(clientesOrdenados.get(i).entrySet().stream().filter(p->p.getValue().comprouNoMes(mes)).map(p->p.getKey()).collect(Collectors.toCollection(HashSet::new)));   
        }
        return clientes;
    }

    /**
     * Devolve um conjunto de ComprasDoProduto referentes às compras realizadas por um determinado cliente num determinado mês nesta filial.
     * @param codigoCliente Cliente cujas compras pretendemos obter.
     * @param mes Mês em que as compras que queremos obter foram feitas.
     * return Conjunto de ComprasDoProduto referentes às compras realizadas por um determinado cliente num determinado mês nesta filial.
     */
    public Set<ComprasDoProduto> comprasFeitasMes(String codigoCliente, int mes){
        ComprasPorCliente compras = comprasDoCliente(codigoCliente);  
        if(compras == null)
            return null;
        return compras.comprasPorMes(mes);        
    }

    /** Determina, para cada mês, o número total de compras que um determinado cliente fez nesta filial.
     * @param codigoCliente Código do cliente sobre o qual se pretende consultar informação.
     * @return Array de inteiros cujo valor no indice i corresponde ao número total de compras que um determinado cliente fez no mês i.
     */
    public int[] quantasComprasPorMes(String codigoCliente){
        return comprasDoCliente(codigoCliente).getQuantasComprasPorMes();
    }

    /**
     * Dado um código de produto e um mês, calcula o conjunto dos clientes que compraram esse produto num determinado mês, nesta filial.
     * @param codigoProduto Código do produto para o qual se pretende calcular que clientes o compraram num mês.
     * @param mes Mês para o qual se pretende calcular que clientes que compraram o produto.
     * @return Conjunto dos clientes que compraram o produto no mês pretendido nesta filial.
     */
    public Set<String> clientesCompraramProdutoMes(String codigoProduto, int mes){
        Set<String> res = new HashSet<>();

        for(Map<String, ComprasPorCliente> m : clientesOrdenados)
            for(ComprasPorCliente cpc : m.values())
                if(cpc.comprouProdutoMes(codigoProduto, mes))
                    res.add(cpc.getCodigoCliente());
        return res;
    }

    /**
     * Dado um código de produto, calcula o conjunto dos clientes que compraram esse produto, nesta filial.
     * @param codigoProduto Código do produto para o qual se pretende calcular que clientes o compraram.
     * @return Conjunto dos clientes que compraram o produto nesta filial.
     */
    public Set<String> clientesCompraramProduto(String codigoProduto){
        Set<String> res = new HashSet<>();

        for(Map<String, ComprasPorCliente> map : clientesOrdenados)
            for(ComprasPorCliente cpc : map.values())
                if(cpc.comprouProduto(codigoProduto))
                    res.add(cpc.getCodigoCliente());
        return res;
    }

    /**
     * Calcula os três maiores compradores desta filial.
     * @result Array de 3 ParCliFat ordenados decrescentemente pelo total gasto pelo cliente.
     */
    public ParCliFat[] tresMaioresCompradores(){
        Comparator<ParCliFat> ordemCrescFaturacao = 
            (p1,p2) -> p1.getFat() < p2.getFat() ? -1 : p1.getFat() > p2.getFat() ? 1 : 0;
        /* o comprador do top3 que gastou menos dinheiro e mantido na cabeca da queue para que      *
         * possa ser facilmente removido, caso surja um comprador que gastou mais dinheiro que ele. */
        Queue<ParCliFat> top3 = new PriorityQueue<>(3, ordemCrescFaturacao);

        for(Map<String, ComprasPorCliente> map : clientesOrdenados){
            for(ComprasPorCliente cpc : map.values()){
                ParCliFat par = new ParCliFat(cpc.getCodigoCliente(), cpc.totalGastoAno());

                if(top3.size() < 3)
                    top3.add(par);
                else if(ordemCrescFaturacao.compare(top3.peek(), par) < 0){
                    top3.poll();
                    top3.add(par);
                }
            }
        }
        ParCliFat[] arrTop3 = top3.toArray(new ParCliFat[3]);
        Arrays.sort(arrTop3, Collections.reverseOrder(ordemCrescFaturacao));
        return arrTop3;
    }

    /** 
     * Cria um mapeamento que a cada cliente que comprou nesta filial, faz corresponder o conjunto de produtos que ele comprou.
     * @return Mapeamento que a cada cliente que comprou nesta filial, faz corresponder o conjunto de produtos que ele comprou
     */
    public Map<String, Set<String>> mapClienteProdsComp(){
        Map<String, Set<String>> res = new HashMap<>();

        for(Map<String, ComprasPorCliente> map : clientesOrdenados)
            for(ComprasPorCliente cpc : map.values())
                res.put(cpc.getCodigoCliente(), cpc.produtosCompradosAno());

        return res;
    }

    /**
     * Cria um mapeamento que, a cada cliente que comprou na filial, faz corresponder um ParQtdValor que contém a quantidade que esse cliente 
     * comprou e quanto gastou na compra de um determinado produto.
     * @param Produto para o qual pretendemos obter o mapeamento.
     * @return Mapeamento que, a cada cliente que comprou na filial, faz corresponder um ParQtdValor que contém a quantidade que esse cliente 
     * comprou e quanto gastou na compra do produto passado como argumento.
     */
    public Map<String, ParQtdValor> mapClienteParQtdValor(String codigoProduto){
        Map<String, ParQtdValor> res = new HashMap<>();

        for(Map<String, ComprasPorCliente> map : clientesOrdenados)
            for(ComprasPorCliente cpc : map.values())
                res.put(cpc.getCodigoCliente(), cpc.totalCompradoEgasto(codigoProduto));

        return res;
    }

    /**
     * Cria e devolve uma cópia desta Filial.
     * @return Cópia desta Filial.
     */
    @Override
    public Filial clone(){
        return new Filial(this);
    }

    /**
     * Gera e devolve uma representação textual desta Filial.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        return clientesOrdenados.toString();
    }

    /**
     * Calcula e devolve o valor do hash code desta Filial.
     * @return Valor do hash code desta Filial.
     */
    @Override
    public int hashCode(){
        return clientesOrdenados.hashCode();
    }

    /**
     * Testa se esta Filial é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(o == this)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return false;
        Filial f = (Filial) o;
        return this.clientesOrdenados == null? (f.clientesOrdenados == null) : this.clientesOrdenados.equals(f.clientesOrdenados);
    }
}
