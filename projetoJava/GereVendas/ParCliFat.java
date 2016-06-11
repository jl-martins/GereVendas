import java.util.Arrays;

/**
 * Classe que representa pares compostos por um código de cliente e pelo total faturado com esse cliente.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class ParCliFat {
    /** Código de cliente */
    private String cli;
    /** Total gasto pelo cliente */
    private double fat;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias de
     *  ParCliFat sem especificar o código de cliente e o total gasto pelo mesmo)
     */
    private ParCliFat(){
        cli = "";
        fat = 0.0;
    }
    
    /** 
     * Constrói um ParCliFat com os dados passados como parâmetros.
     * @param cli Código de cliente.
     * @param fat Total faturado com o cliente <code>cli</code>
     */
    public ParCliFat(String cli, double fat){
        this.cli = cli;
        this.fat = fat;
    }
    
    /**
     * Constrói uma cópia do ParCliFat passado como parâmetro.
     * @param original ParCliFat a copiar.
     */
    public ParCliFat(ParCliFat original){
        cli = original.getCli();
        fat = original.getFat();
    }
    
    /** Getters */
    
    /**
     * Devolve o código de cliente deste ParCliFat.
     * @return Código de cliente,
     */
    public String getCli(){
        return cli;
    }
    
    /**
     * Total gasto pelo cliente a quem este ParCliFat diz respeito.
     * @return 
     */
    public double getFat(){
        return fat;
    }
    
    /** Setters */
    
    /** Define o código de cliente deste ParCliFat. */
    public void setCli(String cli){
        this.cli = cli;
    }
    
    /** Define o total gasto pelo cliente deste ParCliFat. */
    public void setFat(double fat){
        this.fat = fat;
    }
    
    /**
     * Adiciona o valor passado como parâmetro, ao total gasto deste ParCliFat.
     * @param inc Valor a adicionar ao total gasto.
     */
    public void adicionaFat(double inc){
        fat += inc;
    }
    
    /**
     * Constrói e devolve uma cópia deste ParCliFat
     * @return Cópia da instância que invocou o método.
     */
    @Override
    public ParCliFat clone(){
        return new ParCliFat(this);
    }
    
    /**
     * Testa se este ParCliFat é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        ParCliFat par = (ParCliFat) o;
        return cli.equals(par.getCli()) && fat == par.getFat();
    }
    
    /**
     * Gera e devolve uma representação textual deste ParCliFat.
     * @return Representação textual deste ParCliFat.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Código de cliente: " + (cli != null ? cli : "n/a") + separador);
        sb.append("Total faturado: " + String.format("%.2f", fat) + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o <i>hash code</i> deste ParCliFat.
     * @return Valor do <i>hash code</i> deste ParCliFat.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{cli, fat});
    }
}