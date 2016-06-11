import java.io.File;
import java.util.Arrays;
import java.util.ArrayList;

import static java.lang.System.out;

public class LerFichsTexto_Teste {
    private static final String sep = File.separator;
    private static final String NOME_FICH = ".." + sep + "GereVendas" + sep + "data" + sep + "Vendas_1M.txt";
    
    public static void main(String[] args){
        ArrayList<String> linhas;
        String ls = System.getProperty("line.separator");
        
        Crono.start();
        linhas = LinhasScanner.readLinesArrayWithScanner(NOME_FICH);
        Crono.stop();
        if(linhas == null)
            out.println("Não foi possível encontrar o ficheiro " + NOME_FICH);
        else{
            out.println("Ficheiro " + NOME_FICH + " lido usando Scanner e FileReader!");
            out.println("Lidas e guardadas " + linhas.size() + " linhas.");
            out.println("Tempo:" + Crono.print() + ls + "--------------------------------");
            // Lê linhas com BufferedReader e FileReader
            Crono.start();
            linhas = LinhasBufferedReader.readLinesWithBuff(NOME_FICH);
            Crono.stop();
            out.println("Ficheiro " + NOME_FICH + " lido usando BufferedReader!");
            out.println("Lidas e guardadas " + linhas.size() + " linhas.");
            out.println("Tempo:" + Crono.print() + ls);
            
            out.println("Numero de caracteres: " + linhas.stream()
                                                         .reduce(0, (soma, str) -> soma + str.length(), Integer :: sum));
        }
    }
}