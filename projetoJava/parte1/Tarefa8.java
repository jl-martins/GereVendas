package parte1;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.Comparator;

public class Tarefa8{
    static Comparator<ParStringDouble> ordemString = (p1, p2) -> p1.fst().compareTo(p2.fst());
    static Comparator<ParStringDouble> ordemDouble = (p1, p2) -> p1.snd().compareTo(p2.snd());
   
    public static void main(){
        ArrayList<ParStringDouble> paresSD = new ArrayList<>();
        paresSD.add(new ParStringDouble("X500", 20.75));
        paresSD.add(new ParStringDouble("Z398", 11.45));
        paresSD.add(new ParStringDouble("A11", 2.5));
        paresSD.add(new ParStringDouble("W455", 12.5));
           
        TreeSet<ParStringDouble> ordStr = new TreeSet<>(ordemString);
        TreeSet<ParStringDouble> ordDbl = new TreeSet<>(ordemDouble);
        
        /* Criacao dos TreeSet's com os comparators respectivos, ordStr ordena por String, ordDbl ordena por Double */
        ordStr.addAll(paresSD);
        ordDbl.addAll(paresSD);
    }
}
