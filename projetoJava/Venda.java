
/**
 * Write a description of class Venda here.
 * 
 * @author Grupo1
 * @version 1.0
 */

import java.io.Serializable;
import java.util.Arrays;

public class Venda implements Serializable {
    private int unidadesVendidas, mes, filial;
    private String codigoProduto, codigoCliente;
    private double precoUnitario;
    private TipoVenda tipoVenda;
        
    /* impede que usem o construtor vazio */
    private Venda(){}
    
    public Venda(Venda v){
        this(v.getUnidadesVendidas(), v.getMes(), v.getFilial(), v.getCodigoProduto(), v.getCodigoCliente(), v.getPrecoUnitario(), v.getTipoVenda());
    }
    
    public Venda(int unidadesVendidas, int mes, int filial, String codigoProduto, String codigoCliente, double precoUnitario, TipoVenda tipoVenda)
    {
        this.unidadesVendidas = unidadesVendidas;
        this.mes = mes;
        this.filial = filial;
        this.codigoProduto = codigoProduto;
        this.codigoCliente = codigoCliente;
        this.precoUnitario = precoUnitario;
        this.tipoVenda = tipoVenda;
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
    
    public TipoVenda getTipoVenda(){
        return tipoVenda;
    }
     
    /* nao definimos setters porque queremos que as instancias desta class sejam imutaveis */
    
    /* Funcoes standard */
    public int hashCode(){
        return Arrays.hashCode(new Object[]{unidadesVendidas, mes, filial, codigoProduto, codigoCliente, precoUnitario, tipoVenda});
    }
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("unidadesVendidas: ");
        sb.append(unidadesVendidas);
        sb.append("\nmes: ");
        sb.append(mes);
        sb.append("\nfilial: ");
        sb.append(filial);
        sb.append("\ncodigoProduto: ");
        sb.append(codigoProduto);
        sb.append("\ncodigoCliente: ");
        sb.append(codigoCliente);
        sb.append("\nprecoUnitario");
        sb.append(precoUnitario);
        sb.append("\ntipoVenda: ");
        sb.append(tipoVenda + "\n");
        return sb.toString();
    }
    
    public Venda clone(){
        return new Venda(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
        Venda v = (Venda) o;
        return this.unidadesVendidas == v.unidadesVendidas &&
               this.mes == v.mes &&
               this.filial == v.filial &&
               ((this.codigoProduto == null && v.codigoProduto == null) || (this.codigoProduto != null && this.codigoProduto.equals(v.codigoProduto))) &&
               ((this.codigoCliente == null && v.codigoCliente == null) || (this.codigoCliente != null && this.codigoCliente.equals(v.codigoCliente))) &&
               this.precoUnitario == v.precoUnitario &&
               this.tipoVenda == v.tipoVenda;
    }
}

