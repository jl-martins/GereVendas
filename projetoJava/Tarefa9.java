import java.util.ArrayList;
import static java.lang.System.out;

/**
 * Write a description of class Tarefa9 here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Tarefa9{
    private static final String NOME_FICH = "data/Vendas_3M.txt";
    
    public static void main(){
        ArrayList<String> linhas;
        ArrayList<Venda> vendas;
        long total;
        
        linhas = LinhasBufferedReader.readLinesWithBuff(NOME_FICH);
        vendas = ParserVendas.parseAllLinhas(linhas);
        // Calcula total de compras de FILIAL sem usar Streams
        Crono.start();
        total = TestesVendas.totalComprasPrecoNulo(vendas);
        Crono.stop();
        out.printf("Total de compras por filial calculado sem Streams: %d.\nTempo: %s.\n", total, Crono.print());
        // Calcula total de compras de FILIAL usando Streams
        Crono.start();
        total = vendas.stream()
                      .filter(v -> v.getPrecoUnitario() == 0.0)
                      .count();
        Crono.stop();
        out.printf("Total de compras por filial calculado com Streams: %d.\nTempo: %s.\n", total, Crono.print());
    }
}
