import java.util.Arrays;

/**
 * Classe que representa pares com a quantidade comprada de um produto e o valor gasto no mesmo.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class ParQtdValor{
    /** Quantidade comprada */
    private int qtd;
    /** Valor gasto */
    private double valor;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias de 
     *  ParQtdValor sem especificar a quantidade vendida e o valor gasto)
     */
    private ParQtdValor(){
        qtd = 0;
        valor = 0.0;
    }
    
    /**
     * Constrói um ParQtdValor com os dados passados como parâmetros.
     * @param qtd Quantidade comprada.
     * @param valor Valor gasto.
     */
    public ParQtdValor(int qtd, double valor){
        this.qtd = qtd;
        this.valor = valor;
    }
    
    /**
     * Constrói uma cópia do ParQtdValor passado como parâmetro.
     * @param original ParQtdValor a copiar.
     */
    public ParQtdValor(ParQtdValor original){
        qtd = original.getQtd();
        valor = original.getValor();
    }
    
    /** Getters */
    
    /**
     * Devolve a quantidade comprada, registada neste ParQtdValor.
     * @return Quantidade comprada.
     */
    public int getQtd(){
        return qtd;
    }
    
    /**
     * Devolve o valor gasto, registado neste ParQtdValor.
     * @return Valor gasto.
     */
    public double getValor(){
        return valor;
    }
    
    /** Setters */
    
    /** Define a quantidade comprada. */
    public void setQtd(int qtd){
        this.qtd = qtd;
    }
    
    /** Define o valor total gasto. */
    public void setValor(double valor){
        this.valor = valor;
    }
    
    /** Restantes métodos */
    
    /**
     * Adiciona cada um dos campos do ParQtdValor passado como parâmetro
     * aos respeitvos campos do ParQtdValor que invocou o método.
     */
    public void adiciona(ParQtdValor par){
        qtd += par.getQtd();
        valor += par.getValor();
    }
    
    /**
     * Adiciona a quantidade passada como parâmetro à 
     * quantidade comprada registada neste ParQtdValor.
     * @param inc Quantidade a adicionar.
     */
    public void adicionaQtd(int inc){
        qtd += inc;
    }
    
    /**
     * Adiciona o valor passado como parâmetro
     * ao valor registada neste ParQtdValor.
     * @param inc Valor a adicionar.
     */
    public void adicionaValor(double inc){
        valor += inc;
    }
    
    /**
     * Cria uma cópia deste ParQtdValor e devolve-a.
     * @return Cópia deste ParQtdValor.
     */
    @Override
    public ParQtdValor clone(){
        return new ParQtdValor(this);
    }
    
    /**
     * Testa se este ParQtdValor é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        ParQtdValor par = (ParQtdValor) o;
        return qtd == par.getQtd() && valor == par.getValor();
    }
    
    /**
     * Gera e devolve uma representação textual deste ParQtdValor.
     * @return Representação textual deste ParQtdValor.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Quantidade: " + qtd + separador);
        sb.append("Valor: " + String.format("%.2f", valor) + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o hash code deste ParQtdValor.
     * @return Valor do hash code deste ParQtdValor.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{qtd, valor});
    }
}
