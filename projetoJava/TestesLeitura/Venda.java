import java.io.Serializable;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.util.List;

/* Mover programas de parsing para esta classe */

public class Venda implements Serializable {
    private int unidadesVendidas, mes, filial;
    private String codigoProduto, codigoCliente;
    private double precoUnitario;
    // private TipoVenda tipoVenda;

    /* Impede que se use o construtor vazio */
    private Venda(){}

    /* Campos ordenados da mesma forma que aparecem nas linhas do ficheiro de vendas */
    public Venda(String codigoProduto, double precoUnitario, int unidadesVendidas, /* TipoVenda tipoVenda,*/ String codigoCliente, int mes, int filial){
        this.unidadesVendidas = unidadesVendidas;
        this.mes = mes;
        this.filial = filial;
        this.codigoProduto = codigoProduto;
        this.codigoCliente = codigoCliente;
        this.precoUnitario = precoUnitario;
        // this.tipoVenda = tipoVenda;
    } 

    public Venda(Venda v){        
        this(v.getCodigoProduto(), v.getPrecoUnitario(), v.getUnidadesVendidas(), /* v.getTipoVenda(), */ v.getCodigoCliente(), v.getMes(), v.getFilial());
    }
    
    public boolean eValida(){
        return unidadesVendidas >= 0 && unidadesVendidas <= 200 && mes > 0 && mes <= 12 &&
               filial >= 1 && filial <= 3 && precoUnitario >= 0.0 && precoUnitario <= 999.99;
    }

    public static Venda parseLinhaVenda(String linha){
        int unidadesVendidas, mes, filial;
        String codigoProduto, codigoCliente;
        double precoUnitario;
        TipoVenda tipoVenda;
        String[] campos = linha.split(" ");

        codigoProduto = campos[0];
        codigoCliente = campos[4];

        try{
            unidadesVendidas = Integer.parseInt(campos[2]);
            mes = Integer.parseInt(campos[5]);
            filial = Integer.parseInt(campos[6]);
            precoUnitario = Double.parseDouble(campos[1]);
            // tipoVenda = TipoVenda.fromString(campos[3]);
        }
        catch(NumberFormatException | NullPointerException e/* | TipoVendaInvalidoException e */){
            return null;
        }
        return new Venda(codigoProduto, precoUnitario, unidadesVendidas, /* tipoVenda,*/ codigoCliente, mes, filial);
    }

    public static ArrayList<Venda> parseAllLinhas(ArrayList<String> linhas){
        ArrayList<Venda> vendas = new ArrayList<>();

        for(String str : linhas){
            Venda v = parseLinhaVenda(str);
            if(v != null) // se a venda é válida
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
    
    /* getters */
    public int getUnidadesVendidas(){
        return unidadesVendidas;
    }

    public int getMes(){
        return mes;
    }

    public int getFilial(){
        return filial;
    }

    public String getCodigoProduto(){
        return codigoProduto;
    }

    public String getCodigoCliente(){
        return codigoCliente;
    }

    public double getPrecoUnitario(){
        return precoUnitario;
    }

    /* public TipoVenda getTipoVenda(){
        return tipoVenda;
    }*/

    /* nao definimos setters porque queremos que as instâncias desta classe sejam imutáveis */

    /* Métodos standard */
    public Venda clone(){
        return new Venda(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
        
        Venda v = (Venda) o;
        return this.unidadesVendidas == v.unidadesVendidas && this.mes == v.mes && this.filial == v.filial &&
               (this.codigoProduto == null ? v.codigoProduto == null : this.codigoProduto.equals(v.codigoProduto)) &&
               (this.codigoCliente == null ? v.codigoCliente == null : this.codigoCliente.equals(v.codigoCliente)) &&
               this.precoUnitario == v.precoUnitario;
               // && this.tipoVenda == v.tipoVenda;
    }
    public int hashCode(){
        return Arrays.hashCode(new Object[]{unidadesVendidas, mes, filial, codigoProduto, codigoCliente, precoUnitario, /* tipoVenda */});
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Venda" + separador);
        sb.append("Unidades vendidas: " + unidadesVendidas + separador);
        sb.append("Mes: " + mes + separador);
        sb.append("Filial: " + filial + separador);
        sb.append("Código de produto: " + codigoProduto + separador);
        sb.append("Código de cliente: " + codigoCliente + separador);
        sb.append("Preço unitário" + precoUnitario + separador);
        // sb.append("Tipo de venda: " + tipoVenda.toString() + separador);
        return sb.toString();
    }
}

