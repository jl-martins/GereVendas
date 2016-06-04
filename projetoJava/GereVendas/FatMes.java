public class FatMes implements Serializable {
	private final int mes;
	private int totalVendas;
	private double totalFaturado;
	private Map<String, FatProdMes> fatProds;

	// DECIDIR SE VALE A PENA TER ESTE CONSTRUTOR!
	private FatMes(){}

	/** Constrói uma faturação vazia para o mês passado como parâmetro. */
	public FatMes(int mes) throws MesInvalidoException{
		if(mes <= 0 || mes > 12)
			throw new MesInvalidoException("O mês '" + mes + "' é inválido!");

		this.mes = mes;
		totalVendas = 0;
		totalFaturado = 0.0;
		fatProds = new TreeMap<>();
	}

	/** Constrói uma cópia da faturação do mês passada como parâmetro. */
	public FatMes(FatMes fatMes){
		totalVendas = fatMes.getTotalVendas();
		totalFaturado = fatMes.getTotalFaturado();
		fatProds = fatMes.getFatProds();
	}

	/** @return Total de vendas registadas nesta faturação do mês. */
	public int getTotalVendas(){
		return totalVendas;
	}

	/** @return Faturação total registada nesta faturação do mês. */
	public double getTotalFaturado(){
		return totalFaturado;
	}

	private TreeMap<String, FatProdMes> getFatProds(){
		Map<String, FatProdMes> copia = new TreeMap<>();

		for(Map.Entry<String, FatProdMes> fProd : fatProds)
			copia.put(fProd.getKey(), fProd.getValue().clone());

		return copia;
	}

	/**
	 * @return Se nesta faturação do mês existirem vendas do produto cujo código foi passado parâmetro,
	 * 		   então é devolvida a faturação desse produto no mês em causa; caso contrário é devolvido null.
	 */
	public FatProdMes getFatProdMes(String codigoProduto){
		FatProdMes fProdMes = fatProds.get(codigoProduto);

		return (fProdMes != null) ? fProdMes.clone() : null;
	}

	/**
	 * Regista uma venda nesta faturação do mês.
	 * @param codigoProduto Código do produto vendido.
	 * @param nUnids Número de unidades vendidas.
	 * @param filial Filial em que foi efetuada a venda.
	 */
	public void registaVenda(String codigoProduto, double precoUnitario, int nUnids, int filial){
		FatProdMes fProdMes;
		double faturacao = nUnids * precoUnitario;
		
		++totalVendas;
		totalFaturado += faturacao;

		fProdMes = fatProds.get(codigoProduto)
		if(fProdMes != null)
			fProdMes.adiciona(nUnids, faturacao, filial);
		else
			fatProds.put(codigoProduto, new FatProdMes(codigoProduto, nUnids, faturacao, filial));
	}

	/** @return Cópia desta faturação do mês. */
	public FatMes clone(){
		return new FatMes(this);
	}

	/** @return true se esta faturação do mês for igual ao objeto passado como parâmetro. */
	public boolean equals(Object o){
		if(this == o)
			return true;
		if(o == null || this.getClass() != o.getClass())
			return false;
	
		FatMes fatMes = (FatMes) o;
		return totalVendas == fatMes.getTotalVendas() &&
			   totalFaturado == fatMes.getTotalFaturado() &&
			   fatProds.equals(fatMes.getFatProds());
	}

	/** @return Representação textual desta faturação do mês. */
	public String toString(){
		StringBuilder sb = new StringBuilder();
		String separadorLinhas = System.getProperty("line.separator");

		sb.append("-> Faturação do mês " + meses[mes] + separadorLinhas);
		sb.append("Total de vendas: " + totalVendas + separadorLinhas);
		sb.append("Total faturado: " + totalFaturado + separadorLinhas);
		sb.append("Faturação dos produtos: " + separadorLinhas);
		for(FatProdMes fProdMes : fatProds.values())
			sb.append(fProdMes.toString());

		return sb.toString();
	}

	/** @return Valor do hash code desta faturação do mês. */
	public int hashCode(){
		int hash = 7;
		long aux;

		hash = 31*hash + totalVendas;
		aux = Double.doubleToLongBits(totalFaturado);
		hash = 31*hash + (int) (aux ^ (aux >>> 32));
		hash = 31*hash + fatProds.hashCode();
		return hash;
	}
}
