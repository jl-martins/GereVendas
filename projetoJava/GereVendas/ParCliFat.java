public class ParCliFat {
	private String cli;
	private double fat;

	private ParCliFat(){
		cli = "";
		fat = 0.0;
	}

	public ParCliFat(String cli, double fat){
		this.cli = cli;
		this.fat = fat;
	}

	public ParCliFat(ParCliFat original){
		cli = original.getCli();
		fat = original.getFat();
	}

	public String getCli(){
		return cli;
	}

	public double getFat(){
		return fat;
	}

	public void adicionaFat(double inc){
		fat += inc;
	}

	public void setCli(String cli){
		this.cli = cli;
	}

	public void setFat(double fat){
		this.fat = fat;
	}

	public ParCliFat clone(){
		return new ParCliFat(this);
	}

	public boolean equals(Object o){
		if(this == o)
			return true;
		else if(o == null || this.getClass() != o.getClass())
			return false;

		ParCliFat par = (ParCliFat) o;
		return cli.equals(par.getCli()) && fat == par.getFat();
	}

	public String toString(){
		StringBuilder sb = new StringBuilder();


		return sb.toString();
	}

	public int hashCode(){
		int hash = 7;
		long aux;

		hash = 31*hash + cli.hashCode();
		aux = Double.doubleToLongBits(fat);
		hash = 31*hash + (int) (aux ^ (aux >>> 32));
		return hash;
	}
}