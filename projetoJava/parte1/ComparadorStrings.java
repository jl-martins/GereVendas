import java.util.Comparator;

/**
 * Write a description of class ComparadorCodigos here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class ComparadorStrings implements Comparator<String> {
    public int compare(String str1, String str2){
        return str1.compareTo(str2);
    }
}
