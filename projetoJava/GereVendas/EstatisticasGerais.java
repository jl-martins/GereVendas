import java.util.Arrays;
import java.io.Serializable;

public class EstatisticasGerais implements Serializable{
    private int[] totalCompras;
    private double[][] totalFaturado;
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

    public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
    
    public double[][] getTotalFaturado() { 
        double[][] copia = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        
        for(int i = 1; i < totalFaturado.length; i++)
            copia[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
        return copia;
    }
    
    public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
    // length do nosso array para o caso do array argumento ser maior
    public void setTotalCompras(int[] totalCompras) {
        this.totalCompras = Arrays.copyOf(totalCompras, this.totalCompras.length);
    }
    
    public void setTotalFaturado(double[][] totalFaturado) { 
        for(int i = 1; i < totalFaturado.length; i++)
            this.totalFaturado[i] = Arrays.copyOf(totalFaturado[i], this.totalFaturado[i].length);
    }
    
    public void setTotalCliDistintos(int[] totalCliDistintos) {
        this.totalCliDistintos = Arrays.copyOf(totalCliDistintos, this.totalCliDistintos.length);
    }

    /* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

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

    public EstatisticasGerais clone(){
        return new EstatisticasGerais(this);
    }

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
