import java.util.Arrays;
import java.io.Serializable;

/**
 * Classe de estatísticas gerais relativas aos dados registados nas estruturas de um Hipermercado.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public final class EstatisticasGerais implements Serializable{
    /** Total de compras para cada mês.  */
    private int[] totalCompras;
    /** Matriz que na posição (i,j) tem total faturado no mês i, na filial j. */
    private double[][] totalFaturado;
    /** Total de clientes distintos que compraram em cada mês. */
    private int[] totalCliDistintos;
    
    /** Construtores */
    
    /**
     * Cria uma instância de EstatisticasGerais com cópias dos dados passados como parâmetro.
     * @param totalCompras Total de compras por mês.
     * @param totalFaturado Total faturado por mês (linhas) e por filial (colunas).
     * @param totalCliDistintos Total de clientes distintos que compraram em cada mês.
     */
    public EstatisticasGerais(int[] totalCompras, double[][] totalFaturado, int[] totalCliDistintos){
        this.totalCompras = Arrays.copyOf(totalCompras, totalCompras.length);
        this.totalFaturado = new double[totalFaturado.length][];
        for(int mes = 1; mes < totalFaturado.length; ++mes)
            this.totalFaturado[mes] = Arrays.copyOf(totalFaturado[mes], totalFaturado[mes].length);
        
        this.totalCliDistintos = Arrays.copyOf(totalCliDistintos, totalCliDistintos.length);
    }
    
    /**
     * Constrói uma cópia das estatísticas gerais passadas como parâmetro.
     * @param est Estatísticas gerais a copiar.
     */
    public EstatisticasGerais(EstatisticasGerais est){
        this(est.getTotalCompras(), est.getTotalFaturado(), est.getTotalCliDistintos());
    }

    /** 
     * Array que na posição de índice <code>i</code> guarda
     * o total de compras realizadas no mês <code>i</code>.
     * @return Total de compras por mês.
     */
    public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
    
    /**
     * Devolve uma matriz que na posição <code>(i,j)</code> guarda o
     * total faturado no mês <code>i</code>, na filial <code>j</code>.
     * @return Total faturado em cada mês nas diferentes filiais.
     */
    public double[][] getTotalFaturado() { 
        double[][] copia = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        
        for(int i = 1; i < totalFaturado.length; i++)
            copia[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
        return copia;
    }
    
    /** 
     * Devolve um array que na posição de índice i tem o número de clientes distintos que compram no mês i.
     * @return Total de clientes distintos que compraram em cada mês.
     */
    public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
    
    // Não disponibilizamos setters para garantir a imutabilidade das instâncias de EstatisticasGerais.
    
    /**
     * Cria e retorna uma cópia de uma qualquer instancia desta classe
     * @return cópia da instancia (desta classe) que chamou o método.
     */
    @Override
    public EstatisticasGerais clone(){
        return new EstatisticasGerais(this);
    }
    
    /**
     * Testa se estas EstatisticasGerais são iguais ao objecto passado como argumento.
     * @return <code>true</code> se os objectos comparados forem iguais, <code>false</code> caso o contrário se suceda.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if((o == null) || (this.getClass() != o.getClass()))
            return false;

        EstatisticasGerais est = (EstatisticasGerais) o;
        boolean res = true;
        double[][] totalFaturado = est.getTotalFaturado();
        
        for(int i = 1; i <= Constantes.N_MESES && res; i++)
            res = Arrays.equals(this.totalFaturado[i], totalFaturado[i]);

        return res && Arrays.equals(totalCompras, est.getTotalCompras()) &&
               Arrays.equals(totalCliDistintos, est.getTotalCliDistintos());
    }

    /**
     * Cria e retorna uma string com toda a informação destas estatísticas gerais.
     * @return String com toda a informação contida na instância (desta classe) que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder str = new StringBuilder();

        str.append("- Total de compras e total de clientes distintos que compraram por mês: \n");
        str.append("|  Meses  |");
        str.append("|  Total Compras  |"); 
        str.append("|  Clientes Distintos  |\n");
        for(int i = 1; i <= Constantes.N_MESES; i++){
            str.append(String.format("|%7d  |", i));
            str.append(String.format("|%15d  |", totalCompras[i]));
            str.append(String.format("|%20d  |\n", totalCliDistintos[i]));
        }
        str.append("\n");
        
        str.append("- Total faturado por mês por cada Filial: \n");
        str.append("|  Meses  |");
        for(int i = 1; i <= Constantes.N_FILIAIS; i++)
            str.append("|        Filial ").append(i).append("    |");

        str.append("\n");
        for(int i = 1; i <= Constantes.N_MESES; i++){
            str.append(String.format("|%7d  |", i));
            for(int j = 1; j <= Constantes.N_FILIAIS; j++){
                str.append(String.format("|%16.2f    |", totalFaturado[i][j]));
            }
            str.append("\n");
        }
        return str.toString();
    }
    
    /**
     * Calcula e devolve o <i>hash code</i> destas estatísticas gerais.
     * @return Valor do <i>hash code</i> destas estatísticas gerais.
     */
    @Override
    public int hashCode(){
        int hash = 7;
        
        hash = 31 * hash + Arrays.hashCode(totalCompras);
        hash = 31 * hash + Arrays.deepHashCode(totalFaturado);
        hash = 31 * hash + Arrays.hashCode(totalCliDistintos);
        return hash;
    }
}
