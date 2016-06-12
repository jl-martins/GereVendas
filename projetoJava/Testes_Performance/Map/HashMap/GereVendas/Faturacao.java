import java.io.Serializable;

import java.util.Collections;
import java.util.Set;
import java.util.TreeSet;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;
import java.util.Queue;
import java.util.PriorityQueue;
import java.util.Arrays;

/**
 * <p>
 * Classe que representa a faturação do projeto <strong>GereVendas</strong>.
 * Cada instância de <code>Faturacao</code> relaciona produtos com as suas respetivas vendas,
 * filial a filial e globalmente, possibilitando a obtenção de informações relativas 
 * às vendas mensais e/ou anuais desses produtos
 * <p>
 * Uma instância de faturação referencia todos os produtos da loja/hipermercado
 * em questão (mesmo os que nunca foram vendidos), sem fazer qualquer referência
 * a clientes.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class Faturacao implements Serializable {
    /** Número de filiais para as quais esta faturação guarda informação */
    private final int nfiliais;
    /** 
     * Faturação de cada mês (a posição de índice i armazena a faturação do mês i)
     * Em cada mês só é feita referência aos produtos vendidos nesse mês.
     */
    private FatMes[] fatMensal;
    /** Mapeia códigos de produto na respetiva faturação do ano (referencia todos os produtos) */
    private Map<String, FatAnualProd> todosProdutos;
    
    /** Construtores */
    
    /** 
     * Cria uma faturação com os totais de unidades vendidas e de valor gasto inicializados a zero
     * e que relaciona produtos e vendas de um número de filiais igual a Constantes.N_FILIAIS.
     */
    public Faturacao(){
        this(Constantes.N_FILIAIS);
    }
    
    /** 
     * Cria uma faturação com os totais de unidades vendidas e de valor gasto inicializados a zero
     * e que relaciona produtos e vendas de um número de filiais igual ao valor passado como parâmetro.
     * @param nfiliais Número de filiais a considerar.
     */
    public Faturacao(int nfiliais){
        this.nfiliais = nfiliais;
        fatMensal = new FatMes[Constantes.N_MESES+1];
        
        for(int mes = 1; mes <= Constantes.N_MESES; ++mes)
            fatMensal[mes] = new FatMes(mes);
        todosProdutos = new HashMap<>();
    }
    
    /** 
     * Cria uma cópia da faturação passada como parâmetro. 
     * @param original Faturação a copiar.
     */
    public Faturacao(Faturacao original){
        nfiliais = original.getNfiliais();
        todosProdutos = original.getTodosProdutos();
        fatMensal = original.getFatMensal();
    }
    
    /** Getters */
    
    /**
     * Devolve o número de filiais para as quais esta faturação relaciona produtos e vendas.
     * @return Número de filiais.
     */
    public int getNfiliais(){
        return nfiliais;
    }
    
    /** @return Cópia do array de faturação mensal. */
    private FatMes[] getFatMensal(){
        FatMes[] copia = new FatMes[Constantes.N_MESES+1];

        for(int mes = 1; mes <= Constantes.N_MESES; ++mes)
            copia[mes] = fatMensal[mes].clone();
            
        return copia;
    }
    
    /** @return Cópia do mapeamento de código de produto para a respetiva faturação do ano. */
    private Map<String, FatAnualProd> getTodosProdutos() {
        Map<String, FatAnualProd> copia = new HashMap<>();
        
        todosProdutos.forEach( (k,v) -> copia.put(k, v.clone()) );
        return copia;
    }
    
    // Nota: Esta classe não tem setters para que não seja possível alterar diretamente fatMensal e todosProdutos.
    // A alteração e atualização destes campos da faturação é feita através dos métodos registaProduto() e registaVenda().
    
    /**
     * Regista um produto nesta faturação.
     * @param codigoProduto Código do produto a registar.
     * @return <code>true</code> se o produto a adicionar ainda não tinha sido registado.
     */ 
    public boolean registaProduto(String codigoProduto){
        return todosProdutos.putIfAbsent(codigoProduto, new FatAnualProd(codigoProduto)) == null;
    }
    
    /**
     * Regista uma venda nesta faturação.
     * @param v Venda a registar.
     * @return <code>true</code> se a venda foi registada com sucesso.
     * @throws NullPointerException se a venda passada como parâmetro for <code>null</code>.
     */
    public void registaVenda(Venda v){
        // quando este metodo é invocado, todosProdutos já tem todos os produtos registados.
        FatAnualProd fAnualProd = todosProdutos.get(v.getCodigoProduto());
        fAnualProd.adicionaUnidades(v.getFilial(), v.getUnidadesVendidas());
        
        fatMensal[v.getMes()].registaVenda(v);
    }
    
    /**
     * Devolve um array de inteiros que na posição de índice <code>i</code> armazena o número de compras registadas no mês <code>i</code>.
     * (por número de compras entende-se o número de vendas registadas e não a quantidade vendida)
     * @return Array de inteiros com o número de compras de cada mês.
     */
    public int[] comprasPorMes(){
        int[] res = new int[Constantes.N_MESES+1];
        
        for(int mes = 1; mes <= Constantes.N_MESES; ++mes)
            res[mes] = fatMensal[mes].getTotalVendas();
            
        return res;
    }
    
    /** 
     * Devolve uma matriz de doubles que na posição <code>(i,j)</code> armazena o
     * valor total faturado no mês <code>i</code>, na filial <code>j</code>.
     * @return Matriz com a faturação total por mês, de cada filial.
     */
    public double[][] faturacaoPorFilialPorMes(){
        double res[][] = new double[Constantes.N_MESES+1][nfiliais+1];
        
        for(int mes = 1; mes <= Constantes.N_MESES; ++mes)
            res[mes] = fatMensal[mes].faturacaoPorFilial();
            
        return res;
    }
    
    /** 
     * Cria e retorna o conjunto dos códigos de produtos que nunca foram comprados.
     * @return Conjunto dos códigos dos produtos nunca comprados. 
     */
    public Set<String> nuncaComprados(){
        Set<String> res = new TreeSet<>();

        for(FatAnualProd fAnualProd : todosProdutos.values())
            if(fAnualProd.zeroUnidsVendidas())
                res.add(fAnualProd.getCodigoProduto());
                
        return res;
    }

    /**
     * Dado um mês, devolve o número total global de vendas realizadas nesse mês.
     * @param mes Mês cujo número total global de vendas se pretende consultar.
     * @return Número total global de vendas do mês especificado.
     */
    public int totalVendasMes(int mes) {
        return fatMensal[mes].getTotalVendas();
    }

    /**
     * Dado um código de produto e um mês, devolve um objeto do tipo FatProdMes, 
     * com informação sobre a faturação do produto escolhido, no mês especificado.
     * @param codigoProduto Código de produto a considerar na consulta.
     * @param mes Mês para o qual se pretende obter a faturação do produto especificado.
     * @return Faturação do produto identificado por <code>codigoProduto</code>, no mês especificado.
     */
    public FatProdMes getFatProdMes(String codigoProduto, int mes) {   
        FatProdMes fProdMes = fatMensal[mes].getFatProdMes(codigoProduto);
        // Se o produto não foi vendido no mês escolhido, devolvemos uma 
        // FatProdMes com o número de unidades vendidas e faturação a 0.
        return (fProdMes != null) ? fProdMes.clone() : new FatProdMes(mes, nfiliais, codigoProduto);
    }
    
    /** 
     * Produz uma lista de pares de (código, quantidade vendida) dos <code>X</code> produtos mais vendidos, 
     * ordenada decrescentemente pelo número de unidades vendidas.
     * Nota: Se <code>X</code> for menor ou igual a 0, é devolvida uma lista vazia.
     *       Se <code>X</code> for superior ao número de produtos, é devolvida uma lista de tamanho igual ao número de produtos.
     * @param X tamanho do top de produtos mais vendidos do ano.
     * @return Lista de ParProdQtd com os códigos dos <code>X</code> produtos mais vendidos em todo o ano.

     */
    public List<ParProdQtd> maisVendidos(int X){
        if(X <= 0)
            return Collections.emptyList();

        return todosProdutos.values()
                            .stream()
                            .sorted(Collections.reverseOrder(new ComparadorMaisVendidos()))
                            .limit(X)
                            .map(fatAnualProd -> new ParProdQtd(fatAnualProd.getCodigoProduto(), fatAnualProd.totalUnidsGlobal()))
                            .collect(Collectors.toCollection(ArrayList :: new));
    }
    
    /**
     * Cria e retorna uma cópia desta faturação.
     * @return Cópia desta faturação. 
     */
    @Override
    public Faturacao clone(){
        return new Faturacao(this);
    }
    
    /**
     * Testa se esta faturação é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        Faturacao faturacao = (Faturacao) o;
        return nfiliais == faturacao.getNfiliais() &&
               Arrays.equals(fatMensal, faturacao.fatMensal) &&
               todosProdutos.equals(faturacao.todosProdutos);
    }
    
    /**
     * Cria e devolve uma representação textual desta faturação.
     * @return Representação textual desta faturação.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Faturação" + separador);
        sb.append("Número de filiais: " + nfiliais + separador);
        for(int mes = 1; mes <= Constantes.N_MESES; ++mes)
            sb.append(fatMensal[mes]);
        
        sb.append("-> Faturação anual" + separador);
        for(FatAnualProd fAnualProd : todosProdutos.values())
            sb.append(fAnualProd.toString());
        
        return sb.toString();
    }
    
    /** 
     * Calcula e devolve o valor do <i>hash code</i> desta faturação.
     * @return Valor do <i>hash code</i> desta faturação.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[] {fatMensal, todosProdutos});
    }
}
