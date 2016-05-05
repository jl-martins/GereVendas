public static Venda parseLinhaVenda(String linha)
{
	String[] campos = linha.split(" ");
	int unidadesVendidas, mes, filial;
	String codigoProd, codigoCliente;
	double precoUnitario;
	TipoVenda tipoVenda
	
	codigoProd = campo[0];
	codigoCliente = campo[4];

	try{
		unidadesVendidas = Integer.parseInt(campo[2]);
		mes = Integer.parseInt(campo[5]);
		filial = Integer.parseInt(campo[6]);
		precoUnitario = Double.parseDouble(campo[1]);
	}catch(NumberFormatException | NullPointerException e)
	{
		return null;
	}

	/*Falta tipoVenda*/

	return new Venda(unidadesVendidas, mes, filial, codigoProd, codigoCliente, precoUnitario, tipoVenda);
}

public static ArrayList<Venda> parseAllLinhas(ArrayList<String> linhas)
{
	Iterator<String> it = linhas.iterator();
	ArrayList<Venda> vendas = new ArrayList<>();

	while(it.hasNext())
		vendas.add(parseLinhaVenda(it.next()));

	return vendas;
}


