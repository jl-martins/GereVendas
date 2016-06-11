import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import static java.lang.System.out;

/**
 * Classe com métodos static responsáveis por ler linhas de um ficheiro de vendas,
 * utilizando instâncias de BufferedReader e FileReader para esse fim.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class LinhasBufferedReader{
    public static ArrayList<String> readLinesWithBuff(String fich) {
        ArrayList<String> linhas = new ArrayList<>();
        BufferedReader inStream = null; 
        String linha = null;
        
        try {
            inStream = new BufferedReader(new FileReader(fich));
            while( (linha = inStream.readLine()) != null )
                linhas.add(linha);
        }
        catch(IOException e) { out.println(e.getMessage()); return null; };
        
        return linhas;  
    }
    
    public static ArrayList<Venda> readVendasWithBuff(String fich){
        ArrayList<String> lista = readLinesWithBuff(fich);
        return ParserVendas.parseAllLinhas(lista);
    }
}

