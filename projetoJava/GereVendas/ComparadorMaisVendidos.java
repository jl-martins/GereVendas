public class ComparadorMaisVendidos implements Comparator<FatAnualProd>{
	int compare(FatAnualProd f1, FatAnualProd f2){
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