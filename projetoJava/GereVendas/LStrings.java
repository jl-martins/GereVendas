import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class LStrings {
	private List<String> strings;
	private int pag; // indice da primeira String da pagina atual
	private int tamanhoPag; // numero de Strings por pagina
	private int totalPags;
	private static final int TAMANHO_PAG_DEFAULT = 20;

	/** Constroi uma LStrings vazia. */
	public LStrings(){ this(null); }

	public LStrings(Collection<String> strings){ this(strings, LStrings.TAMANHO_PAG_DEFAULT); }

	public LStrings(Collection<String> strings, int tamanhoPag){
		if(strings.isEmpty() || strings == null)
			this.strings = Collections.emptyList();
		else{
			this.strings = new CopyOnWriteArrayList<>(strings); // os elementos do ArrayList nao vao ser alterados
			pag = 1;
			this.tamanhoPag = (tamanhoPag > 0) ? tamanhoPag : LStrings.TAMANHO_PAG_DEFAULT;
			totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
		}
	}

	/** Volta ao inicio desta LStrings e altera o tamanho da pagina. */
	public void setTamanhoPag(int tamanhoPag){
		if(tamanhoPag > 0){
			pag = 1;
			this.tamanhoPag = tamanhoPag;
			totalPags = LStrings.calculaTotalPags(tamanhoPag, strings.size());
		}
	}

	public int getNumPagina(){ return pag; }

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
			int inicio = (pag - 1) * tamanhoPag;
			int fim = (inicio + tamanhoPag <= strings.size()) ? (inicio + tamanhoPag) : strings.size();
			pagina = strings.subList(inicio, fim);
		}
		return pagina;
	}

	public void proxPag(){
		if(pag + 1 < totalPags) // so avancamos se nao estamos na ultima pagina
			++pag;
	}

	public void pagAnt(){
		if(pag > 1)
			--pag;
	}

	public void irParaPag(int pag){
		if(pag > 0 && pag <= totalPags)
			this.pag = pag;
	}

	public void ultimaPag(){
		pag = totalPags;
	}

	public void primPag(){
		pag = 1;
	}
}