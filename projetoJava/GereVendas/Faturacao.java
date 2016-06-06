import java.io.Serializable;

import java.util.Collections;
import java.util.Set;
import java.util.TreeSet;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;
import java.util.Queue;
import java.util.PriorityQueue;
import java.util.Arrays;

public class Faturacao implements Serializable {
    private FatMes[] fatMensal;
    private Map<String, FatAnualProd> todosProdutos; // Map de codigo de produto para a sua faturacao anual

    public Faturacao(){
        fatMensal = new FatMes[Constantes.N_MESES+1];
        
        for(int i = 1; i <= Constantes.N_MESES; ++i)
            fatMensal[i] = new FatMes(i);
        todosProdutos = new TreeMap<>();
    }

    public Faturacao(Faturacao f){
        todosProdutos = f.getTodosProdutos();
        fatMensal = f.getFatMensal();
    }
    
    /* Devolve uma cópia do array da faturação mensal. */
    private FatMes[] getFatMensal(){
        FatMes[] copia = new FatMes[Constantes.N_MESES+1];

        for(int i = 1; i <= Constantes.N_MESES; ++i)
            copia[i] = fatMensal[i].clone();

        return copia;
    }
    
    /* Devolve uma copia do Map todosProdutos. */
    private Map<String, FatAnualProd> getTodosProdutos() {
        Map<String, FatAnualProd> copia = new TreeMap<>();
        
        for(Map.Entry<String, FatAnualProd> entrada : todosProdutos.entrySet())
            copia.put(entrada.getKey(), entrada.getValue().clone());
        
        return copia;
    }

    /**
     * Regista um produto nesta faturação global.
     * @param codigoProduto Código do produto a registar.
     * @return @c true se o produto a adicionar ainda não tinha sido registado.
     */ 
    public boolean registaProduto(String codigoProduto){
        return todosProdutos.putIfAbsent(codigoProduto, new FatAnualProd(codigoProduto)) == null;
    }
    
    /**
     * Regista uma venda nesta faturação global.
     * @param venda Venda a registar.
     * @return true se a venda foi registada com sucesso.
     */
    public void registaVenda(Venda v){
        // quando este metodo e invocado, todosProdutos ja tem todos os produtos registados.
        todosProdutos.get(v.getCodigoProduto()).adicionaUnidades(v.getFilial(), v.getUnidadesVendidas());
        fatMensal[v.getMes()].registaVenda(v);
    }

    // Query1

    /** @return Conjunto dos produtos que nunca foram comprados. */
    public Set<String> nuncaComprados(){
        Set<String> res = new TreeSet<>();

        for(FatAnualProd fAnualProd : todosProdutos.values())
            if(fAnualProd.zeroUnidsVendidas())
                res.add(fAnualProd.getCodigoProduto());

        return res;
    }

    // Query2

    /**
     * Dado um mês, devolve o número total global de vendas realizadas nesse mês.
     * @param mes Mês cujo número total global de vendas se pretende consultar.
     * @return Número total global de vendas do mês especificado.
     * @throws MesInvalidoException se o mês passado como parâmetro não pertencer ao intervalo [1,12].
     */
    public int totalVendasMes(int mes) throws MesInvalidoException{
        if(mes <= 0 || mes > 12)
            throw new MesInvalidoException("O mês '" + mes + "' é inválido!");

        return fatMensal[mes].getTotalVendas();
    }

    // Query4 (usar este método para cada um dos meses)

    /**
     * Dado um código de produto e um mês, devolve um objeto do tipo FatProdMes, 
     * com informação sobre a faturação do produto escolhido, no mês especificado.
     * @param codigoProduto Código de produto a considerar na consulta.
     * @param mes Mês para o qual se pretende obter a faturação do produto especificado.
     * @return Se o produto identificado por @c codigoProduto tiver sido vendido no mês @c mes,
     *         é devolvido um objeto da classe FatProdMes correspondente ao produto e mês especificados, 
     *         se não é devolvido null.
     * @throws MesInvalidoException se o mês passado como parâmetro não pertencer ao intervalo [1,12].
     */
    public FatProdMes getFatProdMes(String codigoProduto, int mes) throws MesInvalidoException{
        if(mes <= 0 || mes > Constantes.N_MESES)
            throw new MesInvalidoException("O mês '" + mes + "' é inválido!");

        return fatMensal[mes].getFatProdMes(codigoProduto);
    }

    public List<String> maisVendidos(int X){
        if(X <= 0)
            return Collections.emptyList();

        return todosProdutos.values()
                            .stream()
                            .sorted(Collections.reverseOrder(new ComparadorMaisVendidos())) // ordem decrescente de quantidade comprada
                            .limit(X)
                            .map(FatAnualProd :: getCodigoProduto)
                            .collect(Collectors.toCollection(ArrayList<String> :: new));
    }
    
    public Faturacao clone(){
        return new Faturacao(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        Faturacao f = (Faturacao) o;

        return Arrays.equals(fatMensal, f.fatMensal) && todosProdutos.equals(f.todosProdutos);
    }
}