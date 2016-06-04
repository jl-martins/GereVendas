import java.util.Arrays;

public class EstatisticaGeral
{
	private int[] totalCompras;
	private double[][] totalFacturado;
	private int[] totalCliDistintos;

	public EstatisticaGeral()
	{
		totalCompras = new int[13];
		totalFacturado = new double[13][4];
		totalCliDistintos = new int[13];
	}

	public EstatisticaGeral(int[] totalCompras, double[][] totalFacturado, int[] totalCliDistintos)
	{
		this.totalCompras = totalCompras;
		this.totalFacturado = totalFacturado;
		this.totalCliDistintos = totalCliDistintos;
	}
	
	public EstatisticaGeral(EstatisticaGeral est)
	{
		this(est.getTotalCompras(), est.getTotalFacturado(), est.getTotalCliDistintos());
	}

	public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
	public double[][] getTotalFacturado() { 
		tF[][] n = new double[13][4];
		for(int i = 0; i < totalFacturado.length; i++)
			tf[i] = Arrays.copyOf(totalFacturado[i], totalFacturado[i].length);
		return n;
	}
	public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }

	public void setTotalCompras(int[] tC) { totalCompras = Arrays.copyOf(tC, totalCompras.length); } // length do nosso array para o caso do array argumento for >
	public void setTotalFacturado(double[][] tF) { 
		for(int i = 0; i < totalFacturado.length; i++)
			totalFacturado[i] = Arrays.copyOf(tF[i], totalFacturado[i].length);
	}
	public void setTotalCliDistintos(int[] tCD) { totalCliDistintos = Arrays.copyOf(tCD, totalCliDistintos.length); }

	/* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

	public boolean equals(Object o)
	{
		if(this == o) return true;
		if((o == null) || (this.getClass() != o.getClass())) return false;

		EstatisticaGeral obj = (EstatisticaGeral) o;

		boolean res = false;
		double[][] tF = obj.getTotalFacturado();
		for(int i = 1; i < 13 && !res; i++)
			res = Arrays.equals(totalFacturado[i], tF[i]);

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
