
/**
 * Write a description of class TesteBufferedReader here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import static java.lang.System.out;

public class TesteBufferedReader
{
      public static ArrayList<String> readLinesWithBuff(String fich) {
      ArrayList<String> linhas = new ArrayList<>();
      BufferedReader inStream = null; 
      String linha = null;
      try {
            inStream = new BufferedReader(new FileReader(fich));
            while( (linha = inStream.readLine()) != null )
				              linhas.add(linha);
      }
      catch(IOException e) 
          { out.println(e.getMessage()); return null; };
      return linhas;  
   }
   
   public static void main(String args[]) {
        ArrayList<String> linhas;
        Crono.start();
        linhas = readLinesWithBuff("Vendas_3M.txt");
        out.println(Crono.print());
    }
}


