import java.util.Arrays;

/**
 * Classe que representa pares com um triplo com código de cliente, quantidade comprada e valor gasto.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class TriploCliQtdGasto {
    /** Código de cliente */
    private String cli;
    /** Quantidade comprada */
    private int qtd;
    /** Total gasto */
    private double gasto;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias 
     *  de TriploCliQtdGasto sem especificar os vários dados do triplo)
     */
    private TriploCliQtdGasto(){
        cli = "";
        qtd = 0;
        gasto = 0.0;
    }
    
    /** 
     * Constrói um TriploCliQtdGasto com os dados passados como parâmetros.
     * @param cli Código de cliente.
     * @param qtd Quantidade comprada.
     * @param gasto Total gasto pelo cliente <code>cli</code>.
     */
    public TriploCliQtdGasto(String cli, int qtd, double gasto){
        this.cli = cli;
        this.qtd = qtd;
        this.gasto = gasto;
    }
    
    /**
     * Constrói uma cópia do TriploCliQtdGasto passado como parâmetro.
     * @param original TriploCliQtdGasto a copiar.
     */
    public TriploCliQtdGasto(TriploCliQtdGasto original){
        cli = original.getCli();
        qtd = original.getQtd();
        gasto = original.getGasto();
    }
    
    /** Getters */
    
    /**
     * Devolve o código de cliente registado neste TriploCliQtdGasto.
     * @return Código de cliente.
     */
    public String getCli(){
        return cli;
    }
    
    /**
     * Devolve a quantidade registada neste TriploCliQtdGasto.
     * @return Quantidade comprada.
     */
    public int getQtd(){
        return qtd;
    }
    
    /**
     * Devolve o total gasto, registado neste TriploCliQtdGasto.
     * @return Total gasto.
     */
    public double getGasto(){
        return gasto;
    }
    
    /** Setters */
    
    /** Define o código de cliente deste TriploCliQtdGasto. */
    public void setCli(String cli){
        this.cli = cli;
    }
    
    /** Define a quantidade deste TriploCliQtdGasto. */
    public void setQtd(int qtd){
        this.qtd = qtd;
    }
    
    /** Define o total gasto deste TriploCliQtdGasto. */
    public void setGasto(double gasto){
        this.gasto = gasto;
    }
    
    /** Restantes métodos */
    
    /** Adiciona o valor passado como parâmetro à quantidade registada neste TriploCliQtdGasto. */
    public void adicionaQtd(int inc){
        qtd += inc;
    }
    
    /** Adiciona o valor passado como parâmetro ao gasto total registado neste TriploProdClis. */
    public void adicionaGasto(double inc){
        gasto += inc;
    }
    
    /**
     * Cria e devolve uma cópia deste TriploCliQtdGasto.
     * @return Cópia deste TriploCliQtdGasto.
     */
    @Override
    public TriploCliQtdGasto clone(){
        return new TriploCliQtdGasto(this);
    }
    
    /**
     * Testa se este TriploCliQtdGasto é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        TriploCliQtdGasto triplo = (TriploCliQtdGasto) o;
        return cli.equals(triplo.getCli()) && qtd == triplo.getQtd() && gasto == triplo.getGasto();
    }
    
    /**
     * Gera e devolve uma representação textual deste TriploCliQtdGasto.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Código de cliente: " + (cli != null ? cli : "n/a") + separador);
        sb.append("Quantidade comprada: " + qtd + separador);
        sb.append("Valor gasto: " + String.format("%.2f", gasto) + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do hash code deste TriploCliQtdGasto.
     * @return Valor do hash code deste TriploCliQtdGasto.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{cli, qtd, gasto});
    }
}
