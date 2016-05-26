import java.util.Arrays;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.Comparator;

public class Tarefa8
{
	public static void main()
	{
		ArrayList<Par<String, Double>> paresSD = new ArrayList<>();
		    paresSD.add(new Par("X500", new Double(20.75)));
			paresSD.add(new Par("Z398", new Double(11.45)));
			paresSD.add(new Par("A11", new Double(2.5)));
		    paresSD.add(new Par("W455", new Double(12.5)));
		
		/**
		 * Criacao dos TreeSet's com os comparators respectivos, ordStr ordena por String, ordDbl ordena por Double
		 */
		TreeSet<Par<String,Double>> ordStr = new TreeSet<>(new Comparator<Par<String, Double>>(){
			public int compare(Par<String, Double> p1, Par<String, Double> p2)
			{
				return p1.fst().compareTo(p2.fst());
			}
		});

		TreeSet<Par<String, Double>> ordDbl = new TreeSet<>(new Comparator<Par<String,Double>>(){
			public int compare(Par<String, Double> p1, Par<String, Double> p2)
			{
				return p1.snd().compareTo(p2.snd());
			}
		});

		ordStr.addAll(paresSD);
		ordDbl.addAll(paresSD);
	}
}
