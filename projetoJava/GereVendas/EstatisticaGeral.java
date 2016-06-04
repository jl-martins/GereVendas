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

	public int[] getTotalCompras() { return totalCompras; }
	public double[][] getTotalFacturado() { return totalFacturado; }
	public int[] getTotalCliDistintos() { return totalCliDistintos; }

	public void setTotalCompras(int[] tC) { totalCompras = tC; }
	public void setTotalFacturado(double[][] tF) { totalFacturado = tF; }
	public void setTotalCliDistintos(int[] tCD) { totalCliDistintos = tCD; }

	/* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

	public boolean equals(Object o)
	{
		if(this == o) return true;
		if((o == null) || (this.getClass() != o.getClass())) return false;

		EstatisticaGeral obj = (EstatisticaGeral) o;

		boolean res = false;
		double[][] tF = obj.getTotalFacturado();
		for(int i = 0; i < 14 && !res; i++)
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
