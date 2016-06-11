import java.util.Arrays;
import java.io.Serializable;

public class EstatisticasGerais implements Serializable{
    /** Total de compras para cada mês.  */
	private int[] totalCompras;
	/** Total faturado em cada mês nas diferentes Filial */
    private double[][] totalFaturado;
	/** Total de clientes distintos que compraram em cada mês */
    private int[] totalCliDistintos;

    public EstatisticasGerais(){
        totalCompras = new int[Constantes.N_MESES+1];
        totalFaturado = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        totalCliDistintos = new int[Constantes.N_MESES+1];
    }

    public EstatisticasGerais(int[] totalCompras, double[][] totalFaturado, int[] totalCliDistintos){
        this();
        setTotalCompras(totalCompras);
        setTotalFaturado(totalFaturado);
        setTotalCliDistintos(totalCliDistintos);
    }
    
    public EstatisticasGerais(EstatisticasGerais est){
        this(est.getTotalCompras(), est.getTotalFaturado(), est.getTotalCliDistintos());
    }

	/** @return total de compras por mês. */
    public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
    
	/** @return total faturado em cada mes pelas diferentes filiais. */
    public double[][] getTotalFaturado() { 
        double[][] copia = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        
        for(int i = 1; i < totalFaturado.length; i++)
            copia[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
        return copia;
    }
    
	/** @return total de clientes distintos que compraram em cada mes. */
    public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
    
	// length do nosso array para o caso do array argumento ser maior
    /** Atualiza o total de compras por mês.  */
	public void setTotalCompras(int[] totalCompras) {
        this.totalCompras = Arrays.copyOf(totalCompras, this.totalCompras.length);
    }
    
	/** Atualiza to total faturado por mes em cada filial. */
    public void setTotalFaturado(double[][] totalFaturado) { 
        for(int i = 1; i < totalFaturado.length; i++)
            this.totalFaturado[i] = Arrays.copyOf(totalFaturado[i], this.totalFaturado[i].length);
    }
    
	/** Atualiza o total de clientes distintos que compraram em cada mes. */
    public void setTotalCliDistintos(int[] totalCliDistintos) {
        this.totalCliDistintos = Arrays.copyOf(totalCliDistintos, this.totalCliDistintos.length);
    }

	/**
	 * Testa se estas EstatisticasGerais são iguais ao objecto passado como argumento.
	 * @return <code>true</code> se os objectos comparados forem iguais, <code>false</code> caso o contrário se suceda.
	 */
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
	 * Cria e retorna uma cópia de uma qualquer instancia desta classe
	 * @return cópia da instancia (desta classe) que chamou o método.
	 */
    public EstatisticasGerais clone(){
        return new EstatisticasGerais(this);
    }

	/**
	 * Cria e retorna uma string com toda a informacao de EstatisticasGerais
	 * @return string com toda a informacao contida pela instancia (desta classe) que invocou o metodo.
	 */
    public String toString(){
        StringBuilder str = new StringBuilder();

		str.append("- Total de compras e total de clientes distintos que compraram por mês: \n");
		str.append("|  Meses  |");
		str.append("|  Total Compras  |"); 
		str.append("|  Clientes Distintos  |\n");
		for(int i = 1; i <= Constantes.N_MESES; i++)
		{
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

		for(int i = 1; i <= Constantes.N_MESES; i++)
		{
		    str.append(String.format("|%7d  |", i));
			for(int j = 1; j <= Constantes.N_FILIAIS; j++){
				str.append(String.format("|%16.2f    |", totalFaturado[i][j]));
			}
			str.append("\n");
		}

        return str.toString();
    }
}
