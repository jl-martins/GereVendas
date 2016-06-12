import java.util.Comparator;

/**
 * Comparador de objetos da classe FatAnualProd que compara o total global
 * de unidades vendidas registado em duas instâncias de FatAnualProd e, se
 * esse total for igual, compara o código de produto de cada uma das instâncias,
 * conforme o <code>compareTo()</code> da classe String. Este comparador define
 * uma ordem crescente de quantidade total vendida (para obter um comparador que
 * define uma ordem decrescente de quantidade total, basta passar uma instância
 * de ComparadorMaisVendidos para Collections.reverseOrder()).
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class ComparadorMaisVendidos implements Comparator<FatAnualProd>{
    /**
     * Compara o total global de unidades vendidas das FatAnualProd duas
     * passadas para o método e, se esse total for igual, compara os códigos
     * de produto a que cada uma das instâncias diz respeito.
     *
     * @return -1, 0 ou 1, conforme o primeiro argumento seja menor, igual
     *         ou maior do que o segundo argumento, respetivamente.
     */
	public int compare(FatAnualProd f1, FatAnualProd f2){
		int unids1, unids2;

		unids1 = f1.totalUnidsGlobal();
		unids2 = f2.totalUnidsGlobal();
		if(unids1 < unids2)
			return -1;
		else if(unids1 > unids2)
			return 1;
		else
			return f1.getCodigoProduto().compareTo(f2.getCodigoProduto());
	}
}