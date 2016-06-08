public class ParProdQtd {
	private String prod;
	private int qtd;

	private ParProdQtd(){
		prod = "";
		qtd = 0;
	}

	public ParProdQtd(String prod, int qtd){
		this.prod = prod;
		this.qtd = qtd;
	}

	public ParProdQtd(ParProdQtd original){
		prod = original.getProd();
		qtd = original.getQtd();
	}

	public String getProd(){
		return prod;
	}

	public int getQtd(){
		return qtd;
	}

	public void adicionaQtd(int inc){
		qtd += inc;
	}

	public void setProd(String prod){
		this.prod = prod;
	}

	public void setQtd(int qtd){
		this.qtd = qtd;
	}

	public ParProdQtd clone(){
		return new ParProdQtd(this);
	}

	public boolean equals(Object o){
		if(this == o)
			return true;
		else if(o == null || this.getClass() != o.getClass())
			return false;

		ParProdQtd par = (ParProdQtd) o;
		return prod.equals(par.getProd()) && qtd == par.getQtd();
	}

	public String toString(){
		StringBuilder sb = new StringBuilder();

		sb.append("(produto: " + prod);
		sb.append(", quantidade: " + qtd + ")");
		return sb.toString();
	}

	public int hashCode(){
		int hash = 7;

		hash = 31*hash + prod.hashCode();
		hash = 31*hash + qtd;
		return hash;
	}
}