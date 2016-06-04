public class FaturacaoGlobal implements Serializable {
	private FatMes[] fatMensal;
	private Set<FatAnualProd> todosProdutos;
	private static final int N_MESES = 12;

	public FaturacaoGlobal(){
		todosProdutos = new TreeSet<>();
		fatMensal = new FatMes[N_MESES+1];
		
		for(int i = 1; i <= N_MESES; ++i)
			fatMensal[i] = new FatMes(i);
	}

	public FaturacaoGlobal(FaturacaoGlobal fg){
		todosProdutos = this.getTodosProdutos();
		fatMensal = this.getFatMensal();
	}

	/* Devolve uma cópia do array da faturação mensal. */
	private FatMes[] getFatMensal(){
		FatMes copia = new FatMes[N_MESES+1];

		for(int i = 1; i <= N_MESES; ++i)
			copia[i] = fatMensal[i].clone();

		return copia;
	}

	/**
	 * Regista um produto nesta faturação global.
	 * @param codigoProduto Código do produto a registar.
	 * @return @c true se o produto a adicionar ainda não tinha sido registado.
	 */ 
	public boolean registaProduto(String codigoProduto){
		return todosProdutos.add(new FatAnualProd(codigoProduto));
	}

	public void registaVenda(String codigoProduto, double precoUnitario, int nUnids, int mes, int filial){
		todosProdutos.get(codigoProduto).adicionaUnidades(filial, nUnids);
		fatMensal[mes].registaVenda(codigoProduto, precoUnitario, nUnids, filial);
	}

	// Query1

	/** @return Conjunto dos produtos que nunca foram comprados. */
	public Set<String> nuncaComprados(){
		Set<String> res = new TreeSet<>();

		for(FatAnualProd fAnualProd : todosProdutos)
			if(fAnualProd.zeroUnidsVendidas())
				res.add(fAnualProd.getCodigoProduto());

		return res;
	}

	// Query2

	/**
	 * Dado um mês, devolve o número total global de vendas realizadas nesse mês.
	 * @param mes Mês cujo número total global de vendas se pretende consultar.
	 * @return Número total global de vendas do mês especificado.
	 * @throws MesInvalidoException se o mês passado como parâmetro não pertencer ao intervalo [1,12].
	 */
	public int totalVendasMes(int mes) throws MesInvalidoException{
		if(mes <= 0 || mes > 12)
			throw new MesInvalidoException("O mês '" + mes + "' é inválido!");

		return fatMensal[mes].getTotalVendas();
	}

	// Query4 (usar este método para cada um dos meses)

	/**
	 * Dado um código de produto e um mês, devolve um objeto do tipo FatProdMes, 
	 * com informação sobre a faturação do produto escolhido, no mês especificado.
	 * @param codigoProduto Código de produto a considerar na consulta.
	 * @param mes Mês para o qual se pretende obter a faturação do produto especificado.
	 * @return Se o produto identificado por @c codigoProduto tiver sido vendido no mês @c mes,
	 *		   é devolvido um objeto da classe FatProdMes correspondente ao produto e mês especificados, 
	 *		   se não é devolvido null.
	 * @throws MesInvalidoException se o mês passado como parâmetro não pertencer ao intervalo [1,12].
	 */
	public FatProdMes getFatProdMes(String codigoProduto, int mes) throws MesInvalidoException{
		if(mes <= 0 || mes > 12)
			throw new MesInvalidoException("O mês '" + mes + "' é inválido!");

		return fatMensal[mes].getFatProdMes(codigoProduto);
	}

	public List<String> maisVendidos(int X){
		if(X <= 0)
			return Collections.EMPTY_SET; // se X nao for positivo, o conjunto dos mais vendidos e um conjunto vazio
		
		ComparadorMaisVendidos comparador = new ComparadorMaisVendidos();
		Queue<FatAnualProd> topX = new PriorityQueue<>(todosProdutos.size(), comparador);
		// Cria uma PriorityQueue com os X produtos mais vendidos.
		for(FatAnualProd f : todosProdutos){
			if(topX.size() < X)
				topX.add(f.clone());
			else if(comparador.compare(f, topX.peek()) > 0){
				topX.poll();
				topX.add(f.clone());
			}
		}
		return Array.stream(todosProdutos.toArray())
					.sorted(Collections.reverseOrder(comparador))
					.map(FatAnualProd :: getCodigoProduto)
					.collect(Collectors.toCollection(ArrayList :: new));
	}
}