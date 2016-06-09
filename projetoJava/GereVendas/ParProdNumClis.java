import java.util.Arrays;

/**
 * Classe que representa pares de código de produto, número de clientes distintos que o compraram.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class ParProdNumClis {
	private String prod;
	private int numClis;

	private ParProdNumClis(){
		prod = "";
		numClis = 0;
	}

	public ParProdNumClis(String prod, int numClis){
		this.prod = prod;
		this.numClis = numClis;
	}

	public ParProdNumClis(ParProdNumClis original){
		prod = original.getProd();
		numClis = original.getNumClis();
	}

	public String getProd(){
		return prod;
	}

	public int getNumClis(){
		return numClis;
	}

	public void adicionaNumClis(int inc){
		numClis += inc;
	}

	public void setProd(String prod){
		this.prod = prod;
	}

	public void setNumClis(int numClis){
		this.numClis = numClis;
	}

	public ParProdNumClis clone(){
		return new ParProdNumClis(this);
	}

	public boolean equals(Object o){
		if(this == o)
			return true;
		else if(o == null || this.getClass() != o.getClass())
			return false;

		ParProdNumClis par = (ParProdNumClis) o;
		return prod.equals(par.getProd()) && numClis == par.getNumClis();
	}

	public String toString(){
		StringBuilder sb = new StringBuilder();
		String separador = System.getProperty("line.separator");

		sb.append("Produto: " + (prod == null ? "n/a" : prod)).append(separador);
		sb.append("Nº de clientes: " + numClis + separador);
		return sb.toString();
	}

	public int hashCode(){
		int hash = 7;

		hash = 31*hash + prod.hashCode();
		hash = 31*hash + numClis;
		return hash;
	}
}