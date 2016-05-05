
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
    
    public Venda(/* partes */){
        
    }
    
    public Venda(Venda v){
        
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
