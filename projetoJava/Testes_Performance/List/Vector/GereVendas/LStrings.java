import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Vector;
import java.util.Arrays;

/**
 * Classe que representa listas de Strings navegáveis.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (06/2016)
 *     
 */

public class LStrings {
    /** Lista de strings, propriamente dita */
    private List<String> strings;
    /** Número da página atual */
    private int numPag;
    /** Número de Strings por página */
    private int tamanhoPag;
    /** Número total de páginas */
    private int totalPags;
    /** Tamanho da página por omissão */
    private static final int TAMANHO_PAG_DEFAULT = 20;
    
    /** Construtores */
    
    /** Construtor por omissão (declarado como privado para não ser possível construir uma LSTrings sem passar uma coleção de strings) */
    private LStrings(){ this(null, LStrings.TAMANHO_PAG_DEFAULT); }
    
    /**
     * Constrói uma LStrings com as Strings da coleção passada como parâmetro e com
     * um tamanho de página igual a 20 Strings por página.
     * @param strings Coleção de Strings a partir da qual será criada a LStrings.
     */
    public LStrings(Collection<String> strings){ this(strings, LStrings.TAMANHO_PAG_DEFAULT); }
    
    /**
     * Constrói uma LStrings com a coleção passada como 1º argumento e com um tamanho
     * de página igual ao valor passado como 2º argumento (se este for positivo).
     * Se o tamanho da página especificado for menor ou igual a 0, a LStrings
     * é criada com o tamanho de página por omissão, i.e.: 20 Strings por página.
     * @param strings Coleção de Strings a partir da qual será criada a LStrings.
     * @param tamanhoPag Número de Strings por página da LStrings a criar.
     */
    public LStrings(Collection<String> strings, int tamanhoPag){
        if(strings.isEmpty() || strings == null)
            this.strings = Collections.emptyList();
        else{
            this.strings = new Vector<>(strings); // os elementos do Vector nao vao ser alterados
            numPag = 1;
            this.tamanhoPag = (tamanhoPag > 0) ? tamanhoPag : LStrings.TAMANHO_PAG_DEFAULT;
            totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
        }
    }
    
    /**
     * Constrói uma cópia da LStrings passada como parâmetro.
     * @param original LStrings a copiar.
     */
    public LStrings(LStrings original){
        strings = new Vector<>(original.strings);
        numPag = original.getNumPag();
        tamanhoPag = original.getTamanhoPag();
        totalPags = original.getTotalPags();
    }
    
    /** Getters */
    
    /**
     * Devolve o número de páginas desta LStrings
     * @return Número de páginas.
     */
    public int getNumPag(){ return numPag; }
    
    /**
     * Devolve o número de entradas por página desta LStrings.
     * @return Tamanho da página.
     */
    public int getTamanhoPag(){ return tamanhoPag; }
    
    /**
     * Devolve o número total de páginas nesta LStrings.
     * @return Total de páginas.
     */
    public int getTotalPags(){ return totalPags; }
    
    /** Setters */
    
    /**
     * Define o tamanho da página desta LStrings e volta à primeira página.
     * Se o tamanho de página especificado for menor ou igual a 0, este método
     * não tem qualquer efeito.
     * @param tamanhoPag Tamanho de página a definir.
     */
    public void setTamanhoPag(int tamanhoPag){
        if(tamanhoPag > 0){
            numPag = 1;
            this.tamanhoPag = tamanhoPag;
            totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
        }
    }

    /**
     * Metodo auxiliar que calcula o numero total de páginas com base 
     * no tamanho de cada página e no número de Strings por página.
     */
    private static int calculaTotalPags(int tamanhoPag, int nstrings){
        int quociente = nstrings / tamanhoPag;
        int resto = nstrings % tamanhoPag;
        return quociente + (resto != 0 ? 1 : 0);
    }
    
    /**
     * Testa se esta LStrings está vazia.
     * @return <code>true</code> se esta LStrings estiver vazia.
     */
    public boolean estaVazia(){ return strings.isEmpty(); }

    public List<String> getPagina(){
        List<String> pagina = null;

        if(!estaVazia()){
            int inicio = (numPag - 1) * tamanhoPag;
            int fim = (inicio + tamanhoPag <= strings.size()) ? (inicio + tamanhoPag) : strings.size();
            pagina = new Vector<>(strings.subList(inicio, fim));
        }
        return pagina;
    }

    /**
     * Devolve uma String com o número de entradas, número de 
     * entradas por página e total de páginas desta LStrings.
     * @return String com informações relativas a esta LStrings.
     */
    public String getInfo(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("Número de entradas: " + strings.size() + separador);
        sb.append("Entradas por página: " + tamanhoPag + separador);
        sb.append("Total de páginas: " + totalPags + separador);
        return sb.toString();
    }
    
    /**
     * Avança para a próxima página desta LStrings, se esta não estiver na última.
     */
    public void proxPag(){
        if(numPag < totalPags) // so avancamos se nao estamos na ultima pagina
            ++numPag;
    }
    
    /**
     * Muda para a página anterior desta LStrings, se esta não estiver na 1ª página.
     */
    public void pagAnt(){
        if(numPag > 1)
            --numPag;
    }
    
    /**
     * Vai para a página passada como parâmetro, se esta for válida.
     * @para numPag Número da página desta LStrings para a qual se pretende ir.
     * @return <code>true</code> se a página passada como parâmetro for válida.
     */
    public boolean irParaPag(int numPag){
        boolean valida = (numPag >= 1 && numPag <= totalPags);
        if(valida)
            this.numPag = numPag;
        
        return valida;
    }
    
    /** Vai para a última página desta LStrings. */
    public void ultimaPag(){
        numPag = totalPags;
    }
    
    /** Vai para a primeira página desta LStrings. */
    public void primPag(){
        numPag = 1;
    }
    
    /**
     * Constrói e devolve uma cópia desta LStrings.
     * @return Cópia desta LStrings.
     */
    @Override
    public LStrings clone(){
        return new LStrings(this);
    }
    
    /**
     * Testa se esta LStrings e o objeto passado como parâmetro são iguais.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
        
        LStrings lStr = (LStrings) o;
        return strings.equals(lStr.strings) && numPag == lStr.getNumPag() &&
               tamanhoPag == lStr.getTamanhoPag() && totalPags == lStr.getTotalPags();
    }
    
    /**
     * Cria e devolve uma representação textual desta LStrings.
     * @return Representação textual desta LStrings.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("-> LStrings").append(separador);
        sb.append("Página atual: " + numPag + separador);
        sb.append("Entradas por página: " + tamanhoPag + separador);
        sb.append("Total de páginas: " + totalPags + separador);
        sb.append("--------------------------------------------------" + separador);
        for(String str : strings)
            sb.append(str).append(separador);

        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do <i>hash code</i> desta LStrings.
     * @return Valor do <i>hash code</i> desta LStrings.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{strings, numPag, tamanhoPag, totalPags});
    }
}
