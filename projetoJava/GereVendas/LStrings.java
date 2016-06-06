import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

public class LStrings {
    private List<String> strings;
    private int numPag; // numero da pagina atual
    private int tamanhoPag; // numero de Strings por pagina
    private int totalPags;
    private static final int TAMANHO_PAG_DEFAULT = 20; // tamanho da pagina por omissao

    private LStrings(){ this(null, LStrings.TAMANHO_PAG_DEFAULT); }

    public LStrings(Collection<String> strings){ this(strings, LStrings.TAMANHO_PAG_DEFAULT); }

    public LStrings(Collection<String> strings, int tamanhoPag){
        if(strings.isEmpty() || strings == null)
            this.strings = Collections.emptyList();
        else{
            this.strings = new ArrayList<>(strings); // os elementos do ArrayList nao vao ser alterados
            numPag = 1;
            this.tamanhoPag = (tamanhoPag > 0) ? tamanhoPag : LStrings.TAMANHO_PAG_DEFAULT;
            totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
        }
    }

    public LStrings(LStrings original){
        strings = new ArrayList<>(original.strings);
        numPag = original.getNumPag();
        tamanhoPag = original.getTamanhoPag();
        totalPags = original.getTotalPags();
    }

    /** Volta ao inicio desta LStrings e altera o tamanho da pagina. */
    public void setTamanhoPag(int tamanhoPag){
        if(tamanhoPag > 0){
            numPag = 1;
            this.tamanhoPag = tamanhoPag;
            totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
        }
    }

    public int getNumPag(){ return numPag; }

    public int getTamanhoPag(){ return tamanhoPag; }

    public int getTotalPags(){ return totalPags; }

    /* Metodo auxiliar que calcula o numero total de paginas com base *
     * no tamanho de cada pagina e no numero de strings por pagina.   */
    private static int calculaTotalPags(int tamanhoPag, int nstrings){
        int quociente = nstrings / tamanhoPag;
        int resto = nstrings % tamanhoPag;
        return quociente + (resto != 0 ? 1 : 0);
    }

    public boolean estaVazia(){ return strings.isEmpty(); }

    public List<String> getPagina(){
        List<String> pagina = null;

        if(!estaVazia()){
            int inicio = (numPag - 1) * tamanhoPag;
            int fim = (inicio + tamanhoPag <= strings.size()) ? (inicio + tamanhoPag) : strings.size();
            pagina = strings.subList(inicio, fim);
        }
        return pagina;
    }

    /** @return String com o numero de entradas, paginas e entradas por pagina desta LStrings. */
    public String getInfo(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("Número de entradas: " + strings.size() + separador);
        sb.append("Entradas por página: " + tamanhoPag + separador);
        sb.append("Total de páginas: " + totalPags + separador);
        return sb.toString();
    }

    public void proxPag(){
        if(numPag < totalPags) // so avancamos se nao estamos na ultima pagina
            ++numPag;
    }

    public void pagAnt(){
        if(numPag > 1)
            --numPag;
    }

    public void irParaPag(int numPag){
        if(numPag > 0 && numPag <= totalPags)
            this.numPag = numPag;
    }

    public void ultimaPag(){
        numPag = totalPags;
    }

    public void primPag(){
        numPag = 1;
    }

    public LStrings clone(){
        return new LStrings(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
        
        LStrings lStr = (LStrings) o;
        return strings.equals(lStr.strings) && numPag == lStr.getNumPag() &&
               tamanhoPag == lStr.getTamanhoPag() && totalPags == lStr.getTotalPags();
    }
    
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

    public int hashCode(){
        int hash = 7;
        
        for(String str : strings)
            hash = 31*hash + str.hashCode();
        
        hash = 31*hash + numPag;
        hash = 31*hash + tamanhoPag;
        hash = 31*hash + totalPags;
        return hash;
    }
}
