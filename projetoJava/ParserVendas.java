import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;

public class ParserVendas{
    public static Venda parseLinhaVenda(String linha){
        int unidadesVendidas, mes, filial;
        String codigoProd, codigoCliente;
        double precoUnitario;
        TipoVenda tipoVenda;
        String[] campos = linha.split(" ");

        codigoProd = campos[0];
        codigoCliente = campos[4];

        try{
            unidadesVendidas = Integer.parseInt(campos[2]);
            mes = Integer.parseInt(campos[5]);
            filial = Integer.parseInt(campos[6]);
            precoUnitario = Double.parseDouble(campos[1]);
            tipoVenda = TipoVenda.fromString(campos[3]);
        }
        catch(NumberFormatException | NullPointerException | TipoVendaInvalidoException e){
            return null;
        }
        return new Venda(codigoProd, precoUnitario, unidadesVendidas, tipoVenda, codigoCliente, mes, filial);
    }

    public static ArrayList<Venda> parseAllLinhas(ArrayList<String> linhas){
        ArrayList<Venda> vendas = new ArrayList<>();
        
        for(String str : linhas){
            Venda v = parseLinhaVenda(str);
            if(v != null)
                vendas.add(v);
        }
        return vendas;
    }

    public static HashSet<Venda> parseAllLinhasToSet(ArrayList<String> linhas){
        HashSet<Venda> vendas = new HashSet<>();
        
        for(String str : linhas){
            Venda v = parseLinhaVenda(str);
            if(v != null)
                vendas.add(v);
        }
        return vendas;
    }
}
