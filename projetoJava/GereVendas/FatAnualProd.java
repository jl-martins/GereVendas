import java.io.Serializable;
import java.util.Arrays;
import java.util.stream.IntStream;

public class FatAnualProd implements Serializable, Comparable<FatAnualProd> {
	private String codigoProduto;
	private int[] totalUnids;

	/** Constrói a faturação anual do produto com o código passado como parâmetro. */
	public FatAnualProd(String codigoProduto){
		this.codigoProduto = codigoProduto;
		totalUnids = new int[Constantes.N_FILIAIS+1];
	}

	/** Constrói uma cópia da faturação anual do produto passado como parâmetro. */
	public FatAnualProd(FatAnualProd fAnualProd){
		codigoProduto = fAnualProd.getCodigoProduto();
		totalUnids = fAnualProd.getTotalUnids();
	}

	/** @return Código do produto a que esta FatAnualProd diz respeito. */
	public String getCodigoProduto(){
		return codigoProduto;
	}

	/** 
	 * @return Array que na posição de índice @c i tem o total de unidades 
	 * 		   vendidas na filial @c i, para esta FatAnualProd.
	 */
	public int[] getTotalUnids(){
		return (int []) totalUnids.clone(); // totalUnids e um array de ints, logo podemos fazer uma shallow copy
	}

	/**
	 * Adiciona um determinado número de unidades ao total de
	 * unidades vendidas na filial passada como primeiro argumento.
	 * @param filial Filial cujo número total de unidades vendidas se pretende atualizar.
	 * @param nUnids Número de unidades a acrescentar às vendas da filial @c filial.
	 * @throws FilialInvalidaException se @c filial for inválida, i.e. se não pertencer a [1,3].
	 */
	public void adicionaUnidades(int filial, int nUnids) {
		totalUnids[filial] += nUnids;
	}

	/** @return Número total global de unidades vendidas, registadas nesta FatAnualProd. */
	public int totalUnidsGlobal(){
		return IntStream.of(totalUnids).sum();
	}

	/** @return true se esta FatAnualProd não tem registo de qualquer unidade vendida. */
	public boolean zeroUnidsVendidas(){
		return totalUnidsGlobal() == 0;
	}

	@Override
	public int compareTo(FatAnualProd fAnualProd){
		return codigoProduto.compareTo(fAnualProd.getCodigoProduto());
	}

	/** @return Cópia desta FatAnualProd. */
	public FatAnualProd clone(){
		return new FatAnualProd(this);
	}

	/** @return true se esta FatAnualProd for igual ao objeto passado como parâmetro. */
	public boolean equals(Object o){
		if(this == o)
			return true;
		if(o == null || this.getClass() != o.getClass())
			return false;

		FatAnualProd fAnualProd = (FatAnualProd) o;
		return codigoProduto.equals(fAnualProd.getCodigoProduto()) &&
			   Arrays.equals(totalUnids, fAnualProd.getTotalUnids());
	}

	/** @return Representação textual desta FatAnualProd. */
	public String toString(){
		StringBuilder sb = new StringBuilder();

		sb.append("Faturação anual do produto '" + codigoProduto + "'\n");
		sb.append("Vendas:\n");
		for(int i = 1; i <= Constantes.N_FILIAIS; ++i)
			sb.append("Filial ").append(i + ": ").append(totalUnids[i] + "\n");

		return sb.toString();
	}

	/** @return Valor do hash code desta FatAnualProd. */
	public int hashCode(){
		int hash = 7;

		hash = 31*hash + codigoProduto.hashCode();
		hash = 31*hash + Arrays.hashCode(totalUnids);
		return hash;
	}
}