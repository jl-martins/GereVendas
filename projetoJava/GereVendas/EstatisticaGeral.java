import java.util.Arrays;

public class EstatisticaGeral {
    private int[] totalCompras;
    private double[][] totalFaturado;
    private int[] totalCliDistintos;

    public EstatisticaGeral(){
        totalCompras = new int[13];
        totalFaturado = new double[13][4];
        totalCliDistintos = new int[13];
    }

    public EstatisticaGeral(int[] totalCompras, double[][] totalFaturado, int[] totalCliDistintos){
        this();
        setTotalCompras(totalCompras);
        setTotalFaturado(totalFaturado);
        setTotalCliDistintos(totalCliDistintos);
    }
    
    public EstatisticaGeral(EstatisticaGeral est){
        this(est.getTotalCompras(), est.getTotalFaturado(), est.getTotalCliDistintos());
    }

    public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
    
    public double[][] getTotalFaturado() { 
        double[][] copia = new double[13][4];
        
        for(int i = 1; i < totalFaturado.length; i++)
            copia[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
        return copia;
    }
    
    public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
    // length do nosso array para o caso do array argumento ser maior
    public void setTotalCompras(int[] totalCompras) {
        this.totalCompras = Arrays.copyOf(totalCompras, totalCompras.length);
    }
    
    public void setTotalFaturado(double[][] totalFaturado) { 
        for(int i = 1; i < totalFaturado.length; i++)
            this.totalFaturado[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
    }
    
    public void setTotalCliDistintos(int[] totalCliDistintos) {
        this.totalCliDistintos = Arrays.copyOf(totalCliDistintos,    totalCliDistintos.length);
    }

    /* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

    public boolean equals(Object o)
    {
        if(this == o)
            return true;
        if((o == null) || (this.getClass() != o.getClass()))
            return false;

        EstatisticaGeral est = (EstatisticaGeral) o;
        boolean res = true;
        double[][] totalFaturado = est.getTotalFaturado();
        
        for(int i = 1; i < 13 && res; i++)
            res = Arrays.equals(this.totalFaturado[i], totalFaturado[i]);

        return res && Arrays.equals(totalCompras, est.getTotalCompras()) &&
               Arrays.equals(totalCliDistintos, est.getTotalCliDistintos());
    }

    public EstatisticaGeral clone()
    {
        return new EstatisticaGeral(this);
    }

    public String toString()
    {
        StringBuilder str = new StringBuilder();
        return str.toString();
    }
}
