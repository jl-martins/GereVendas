
/**
 * Write a description of class Venda here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */

import java.io.Serializable;

public class Venda implements Serializable 
{
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
    
    public getPrecoUnitario(){
        return precoUnitario;
    }
    
    public TipoVenda getTipoVenda(){
        return tipoVenda;
    }
     
}
