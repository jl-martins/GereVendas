/* Fazer descricao */

import java.util.ArrayList;
import java.util.Scanner;
import java.io.FileReader;
import java.io.IOException;
import static java.lang.System.out;

public class TesteScanner
{
      public static ArrayList<String>
            readLinesArrayWithScanner(String ficheiro) {
        ArrayList<String> linhas = new ArrayList<>();
        Scanner scanFile = null;
        try {
            scanFile = new Scanner(new FileReader(ficheiro));
            scanFile.useDelimiter("\n\r");
            while(scanFile.hasNext()) 
                linhas.add(scanFile.nextLine());
        }
        catch(IOException ioExc)
             { out.println(ioExc.getMessage()); return null; }
        finally { if(scanFile != null) scanFile.close(); }
        return linhas;
     }
   
   public static void main(String args[]) {
        ArrayList<String> linhas;
        Crono.start();
        linhas = readLinesArrayWithScanner("Vendas_3M.txt");
        out.println(Crono.print());
    }
}
