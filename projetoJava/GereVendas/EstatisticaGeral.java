import java.util.Arrays;

public class EstatisticaGeral {
	private int[] totalCompras;
	private double[][] totalFacturado;
	private int[] totalCliDistintos;

	public EstatisticaGeral(){
		totalCompras = new int[13];
		totalFacturado = new double[13][4];
		totalCliDistintos = new int[13];
	}

	public EstatisticaGeral(int[] totalCompras, double[][] totalFacturado, int[] totalCliDistintos){
		this();
		setTotalCompras(totalCompras);
		setTotalFaturado(totalFacturado);
		setTotalCliDistintos(totalCliDistintos);
	}
	
	public EstatisticaGeral(EstatisticaGeral est){
		this(est.getTotalCompras(), est.getTotalFacturado(), est.getTotalCliDistintos());
	}

	public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
	
	public double[][] getTotalFacturado() { 
		double[][] copia = new double[13][4];
		
		for(int i = 1; i < totalFacturado.length; i++)
			copia[i] = Arrays.copyOf(totalFacturado[i], totalFacturado[i].length);
		return copia;
	}
	
	public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
	// length do nosso array para o caso do array argumento ser maior
	public void setTotalCompras(int[] totalCompras) {
		this.totalCompras = Arrays.copyOf(totalCompras, totalCompras.length);
	}
	
	public void setTotalFacturado(double[][] totalFacturado) { 
		for(int i = 1; i < totalFacturado.length; i++)
			this.totalFacturado[i] = Arrays.copyOf(totalFacturado[i], totalFacturado[i].length);
	}
	
	public void setTotalCliDistintos(int[] totalCliDistintos) {
		this.totalCliDistintos = Arrays.copyOf(totalCliDistintos, totalCliDistintos.length);
	}

	/* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

	public boolean equals(Object o)
	{
		if(this == o) return true;
		if((o == null) || (this.getClass() != o.getClass())) return false;

		EstatisticaGeral est = (EstatisticaGeral) o;

		boolean res = true;
		double[][] totalFacturado = est.getTotalFacturado();
		for(int i = 1; i < 13 && res; i++)
			res = Arrays.equals(this.totalFacturado[i], totalFacturado[i]);

		return res && Arrays.equals(totalCompras, obj.getTotalCompras())
			&& Arrays.equals(totalCliDistintos, obj.getTotalCliDistintos());
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
