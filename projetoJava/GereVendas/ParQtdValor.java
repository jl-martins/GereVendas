public class ParQtdValor{
    private int qtd;
	private double valor;

	private ParQtdValor(){
		qtd = 0;
		valor = 0.0;
	}

	public ParQtdValor(int qtd, double valor){
		this.qtd = qtd;
		this.valor = valor;
	}

	public ParQtdValor(ParQtdValor original){
		qtd = original.getQtd();
		valor = original.getValor();
	}

	public int getQtd(){
		return qtd;
	}

	public double getValor(){
		return valor;
	}
	
	public void adiciona(ParQtdValor par){
	    qtd += par.getQtd();
	    valor += par.getValor();
	}
	
	public void adicionaQtd(int inc){
	    qtd += inc;
	}
	   
	public void adicionaValor(double inc){
		valor += inc;
	}

	public void setQtd(int qtd){
		this.qtd = qtd;
	}

	public void setValor(double valor){
		this.valor = valor;
	}

	public ParQtdValor clone(){
		return new ParQtdValor(this);
	}

	public boolean equals(Object o){
		if(this == o)
			return true;
		else if(o == null || this.getClass() != o.getClass())
			return false;

		ParQtdValor par = (ParQtdValor) o;
		return qtd == par.getQtd() && valor == par.getValor();
	}

	public String toString(){
		StringBuilder sb = new StringBuilder();

		sb.append("(quantidade: " + qtd);
		sb.append(", valor: " + valor + ")");
		return sb.toString();
	}

	public int hashCode(){
		int hash = 7;
		long aux;

		hash = 31*hash + qtd;
		aux = Double.doubleToLongBits(valor);
		hash = 31*hash + (int) (aux ^ (aux >>> 32));
		return hash;
	}
}
