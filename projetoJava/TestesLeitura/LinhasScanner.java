import java.util.ArrayList;
import java.util.Scanner;
import java.io.FileReader;
import java.io.IOException;
import static java.lang.System.out;

/**
 * Classe com métodos static que permitem ler linhas de um ficheiro de vendas,
 * utilizando instâncias de Scanner e FileReader para esse fim.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class LinhasScanner {
    public static ArrayList<String> readLinesArrayWithScanner(String ficheiro) {
        ArrayList<String> linhas = new ArrayList<>();
        Scanner scanFile = null;
        try {
            scanFile = new Scanner(new FileReader(ficheiro));
            scanFile.useDelimiter("\n\r");
            while(scanFile.hasNext()) 
                linhas.add(scanFile.nextLine());
        }
        catch(IOException ioExc){
            out.println(ioExc.getMessage()); 
            return null;
        }
        finally { 
            if(scanFile != null) 
                scanFile.close(); 
        }
        return linhas;
    }
}
