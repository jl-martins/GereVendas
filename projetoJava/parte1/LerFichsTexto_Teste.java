package parte1;

import java.util.ArrayList;
import static java.lang.System.out;

public class LerFichsTexto_Teste {
    private static final String nomeFich = "data/Vendas_5M.txt";
    
    public static void main(String[] args){
        ArrayList<String> linhas;
        
        Crono.start();
        linhas = LinhasScanner.readLinesArrayWithScanner(nomeFich);
        Crono.stop();
        out.println("Ficheiro " + nomeFich + " lido usando Scanner e FileReader!");
        out.println("Lidas e guardadas " + linhas.size() + " linhas.");
        out.println("Tempo:" + Crono.print() + "\n --------------------------------");
        
        Crono.start();
        linhas = LinhasBufferedReader.readLinesWithBuff(nomeFich);
        Crono.stop();
        out.println("Ficheiro " + nomeFich + " lido usando BufferedReader!");
        out.println("Lidas e guardadas " + linhas.size() + " linhas.");
        out.println("Tempo:" + Crono.print() + "\n");
        
        out.println("Numero de caracteres:" + linhas.stream()
                                                    .reduce(0, (soma, str) -> soma + str.length(), Integer :: sum));
    }
}