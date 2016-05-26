import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;

public class Tarefa5 {

	public static Venda parseLinhaVenda(String linha)
	{
		String[] campos = linha.split(" ");
		int unidadesVendidas, mes, filial;
		String codigoProd, codigoCliente;
		double precoUnitario;
		TipoVenda tipoVenda;
	
		codigoProd = campos[0];
		codigoCliente = campos[4];

		try{
			unidadesVendidas = Integer.parseInt(campos[2]);
			mes = Integer.parseInt(campos[5]);
			filial = Integer.parseInt(campos[6]);
			precoUnitario = Double.parseDouble(campos[1]);
			tipoVenda = TipoVenda.tipoFromString(campos[3]);
		}catch(NumberFormatException | NullPointerException | TipoVendaInvalidoException e)
		{
			return null;
		}

		return new Venda(codigoProd, precoUnitario, unidadesVendidas, tipoVenda, codigoCliente, mes, filial);
	}

	public static ArrayList<Venda> parseAllLinhas(ArrayList<String> linhas)
	{
		Iterator<String> it = linhas.iterator();
		ArrayList<Venda> vendas = new ArrayList<>();

		while(it.hasNext())
			vendas.add(parseLinhaVenda(it.next()));

		return vendas;
	}

	public static HashSet<Venda> parseAllLinhasToSet(ArrayList<String> linhas)
	{
		Iterator<String> it = linhas.iterator();
		HashSet<Venda> vendas = new HashSet<>();

		while(it.hasNext())
			vendas.add(parseLinhaVenda(it.next()));
	
		return vendas;
	}
}
