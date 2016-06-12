import java.util.Arrays;
import java.util.Objects;

import static java.lang.System.out;

/**
 * Classe de menus genéricos, que permite a criação de menus com título e um
 * número arbitrário de opções. As opções de cada instância de Menu são passadas
 * num array de Strings e são numeradas pela ordem em que surgem nesse array,
 * desde 1 até à <c>length</c> do array fornecido. O utilizador também pode escolher,
 * aquando da criação de um Menu, se este tem ou não opção de sair, através de
 * um valor do tipo <c>boolean</c> passado como 3º argumento do construtor parametrizado. 
 * 
 * <p>Cada instância de Menu pode receber a mensagem executa() para apresentar as suas 
 * opções e ler uma opção do <c>stdin</c>. Depois de lida, a opção pode então ser obtida
 * através do envio da mensagem getOpcao() ao Menu que a leu.
 * 
 * <pre>
 * Utilização típica:
 * {@code
 *      Menu menu = new Menu(titulo, opcoes, true); // true indica que o menu tem opção de "Sair"
 *      menu.executa();
 *      int op = menu.getOpcao();
 *      switch(op){
 *          ...
 *      }
 * }
 * </pre>
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class Menu{
    /** Título do menu. */
    private String titulo;
    /** Separa o título das opções e a última opção da prompt. */
    private String separador;
    /** Opções do menu. */
    private final String[] opcoes;
    /** <c>true</c> se o menu tiver a opção "Sair". */
    private boolean temOpcaoSair;
    /** Número da opção atual do menu (-1 se nunca foi lida uma opção). */
    private int op;
    
    /** Construtores */
    
    /**
     * Construtor por omissão.
     * (declarado como privado para não ser possível construir menus vazios)
     */
    private Menu(){
        titulo = separador = null;
        opcoes = null;
        temOpcaoSair = false;
        op = -1;
    }
    
    /**
     * Construtor parametrizado.
     * @param titulo Título do Menu a criar.
     * @param opcoes Opções do Menu a criar.
     * @param temOpcaoSair <c>true</c> se o Menu a criar tem opção de sair.
     * @throws NullPointerException se o array de opcoes for <c>null</c>.
     */
    public Menu(String titulo, String[] opcoes, boolean temOpcaoSair){
        Objects.requireNonNull(opcoes, "opcoes não pode ser null!"); 
        
        this.titulo = (titulo != null ? titulo : "");
        this.opcoes = new String[opcoes.length];
        System.arraycopy(opcoes, 0, this.opcoes, 0, opcoes.length);
        this.separador = geraSeparador(titulo, opcoes); // gera um separador adequado a este menu.
        this.temOpcaoSair = temOpcaoSair;
        this.op = -1;
    }
    
    /**
     * Constrói uma cópia do Menu passado como parâmetro.
     * @param menu Menu a copiar.
     * @throws NullPointerException se o menu passado como parâmetro for <c>null</c>.
     */
    public Menu(Menu menu){
        this(menu.getTitulo(), menu.getOpcoes(), menu.getTemOpcaoSair());
    }
    
    /** Getters */
    
    /** 
     * Devolve o título deste menu.
     * @return Título deste menu. 
     */
    public String getTitulo(){
        return titulo;
    }
    
    /** 
     * Devolve um array de Strings com as opções deste menu.
     * @return Opções deste menu. 
     */
    public String[] getOpcoes(){
        String[] copia = new String[opcoes.length];
        System.arraycopy(opcoes, 0, copia, 0, opcoes.length);
        return copia;
    }
    
    /** 
     * Indica se este menu tem opção de sair.
     * @return <c>true</c> se este menu tiver opção de sair.
     */
    public boolean getTemOpcaoSair(){
        return temOpcaoSair;
    }
    
    /** 
     * Devolve o número da última opção lida por este menu (-1 se nunca foi lida qualquer opção)
     * @return Última opção lida.
     */
    public int getOpcao(){
        return op;
    }
    
    /** Setters */
    
    /** Altera o titulo deste menu (se a String passada como parâmetro for <c>null</c>, o título é definido como ""). */
    public void setTitulo(String titulo){
        this.titulo = (titulo != null ? titulo : "");
        separador = geraSeparador(titulo, opcoes); // recalcula o separador
    }
    
    /** Restantes métodos */
    
    /** Imprime as opções deste menu e lê uma opção do <c>stdin</c>. */
    public void executa(){
        do{
            apresentaMenu();
            op = lerOpcao();
        } while(op == -1);
    }
    
    /** Apresenta este menu no <c>stdout</c>. */
    private void apresentaMenu(){
        out.println(separador);
        out.println(titulo);
        out.println(separador);
        for(int i = 0; i < opcoes.length; ++i)
            out.printf("%2d. %s\n", i+1, opcoes[i]);
        
        if(temOpcaoSair)
            out.println(" 0. Sair");
        out.println(separador);
    }
    
    /** Gera o separador a colocar entre a última opção e a prompt. */
    private static String geraSeparador(String titulo, String[] opcoes){
        int maxLen = titulo.length();
        
        for(int i = 0; i < opcoes.length; ++i)
            if((opcoes[i].length() + 4) > maxLen) // o número da opção seguido do carater '.' e de um espaço correspondem a 4 caratéres
                maxLen = opcoes[i].length() + 4;

        return new String(new char[maxLen+5]).replace("\0", "*"); // string com '*' repetido (maxLen+5) vezes
    }
    
    /** 
     * Apresenta uma <i>prompt</i> e lê uma opção a partir do <c>stdin</c>.
     * @return A opção lida, se esta for válida; -1 caso contrário.
     */
    private int lerOpcao(){
        int op;
        
        out.print(" >>> ");
        op = Input.lerInt();

        if(op > opcoes.length || (temOpcaoSair && op < 0) || (!temOpcaoSair && op < 1)){
            out.println("Opção inválida!");
            op = -1;
        }
        return op;
    }
    
    /** 
     * Cria e devolve uma cópia deste menu.
     * @return Cópia deste Menu. 
     */
    public Menu clone(){
        return new Menu(this);
    }
    
    /** 
     * Testa se este menu é igual ao objeto passado como parâmetro.
     * @param o Objeto a comparar com este menu.
     * @return <c>true</c> se os objetos comparados forem iguais.
     */
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return false;
        Menu menu = (Menu) o;

        return titulo.equals(menu.getTitulo()) && Arrays.equals(opcoes, menu.getOpcoes()) && temOpcaoSair == menu.getTemOpcaoSair();
    }
    
    /** 
     * Cria e devolve uma representação textual deste menu.
     * @return Representação textual dos campos deste menu.
     */
    public String toString(){
        StringBuilder sb = new StringBuilder("Menu:\n");
        
        sb.append("Título: " + titulo);
        sb.append("\nOpções:\n");
        for(int i = 0; i < opcoes.length; ++i)
            sb.append(opcoes[i]).append("\n");
        
        if(temOpcaoSair)
            sb.append("Sair\n");
        
        sb.append("Número de opções: " + opcoes.length);
        sb.append("Opção atual: " + op);
        return sb.toString();
    }
    
    /** 
     * Calcula e devolve o <i>hash code</i> deste menu.
     * @return Valor do hash code deste menu.
     */
    public int hashCode(){
        return Arrays.hashCode(new Object[]{titulo, separador, opcoes, temOpcaoSair, op});
    } 
}
