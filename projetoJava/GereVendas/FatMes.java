import java.io.Serializable;
import java.util.Map;
import java.util.TreeMap;

public class FatMes implements Serializable {
    private final int mes;
    private int totalVendas;
    private double totalFaturado;
    private Map<String, FatProdMes> fatProds;

    /** Constrói uma faturação vazia para o mês passado como parâmetro. */
    public FatMes(int mes) {
        this.mes = mes;
        totalVendas = 0;
        totalFaturado = 0.0;
        fatProds = new TreeMap<>();
    }

    /** Constrói uma cópia da faturação do mês passada como parâmetro. */
    public FatMes(FatMes fatMes) {
        mes = fatMes.getMes();
        totalVendas = fatMes.getTotalVendas();
        totalFaturado = fatMes.getTotalFaturado();
        fatProds = fatMes.getFatProds();
    }
    
    /** @return Mês a que esta faturação diz respeito. */
    public int getMes(){
        return mes;
    }
    
    /** @return Total de vendas registadas nesta faturação do mês. */
    public int getTotalVendas(){
        return totalVendas;
    }

    /** @return Faturação total registada nesta faturação do mês. */
    public double getTotalFaturado(){
        return totalFaturado;
    }

    private Map<String, FatProdMes> getFatProds(){
        Map<String, FatProdMes> copia = new TreeMap<>();

        for(Map.Entry<String, FatProdMes> fProd : fatProds.entrySet())
            copia.put(fProd.getKey(), fProd.getValue().clone());

        return copia;
    }

    /**
     * @return Se nesta faturação do mês existirem vendas do produto cujo código foi passado parâmetro,
     *         então é devolvida a faturação desse produto no mês em causa; caso contrário é devolvido null.
     */
    public FatProdMes getFatProdMes(String codigoProduto){
        FatProdMes fProdMes = fatProds.get(codigoProduto);

        return (fProdMes != null) ? fProdMes.clone() : null;
    }

    /**
     * Regista uma venda nesta faturação do mês.
     * @param v Venda a registar.
     */
    public void registaVenda(Venda v){
        double faturado;
        int mes, unidadesVendidas, filial;
        String codigoProduto = v.getCodigoProduto();
        
        mes = v.getMes();
        unidadesVendidas = v.getUnidadesVendidas();
        filial = v.getFilial();
        faturado = unidadesVendidas * v.getPrecoUnitario();
        
        ++totalVendas;
        totalFaturado += faturado;
        
        FatProdMes fProdMes = fatProds.get(codigoProduto);
        if(fProdMes != null)
            fProdMes.adiciona(unidadesVendidas, faturado, filial);
        else
            fatProds.put(codigoProduto, new FatProdMes(mes, codigoProduto, unidadesVendidas, faturado, filial));
    }

    /** @return Cópia desta faturação do mês. */
    public FatMes clone(){
        return new FatMes(this);
    }

    /** @return true se esta faturação do mês for igual ao objeto passado como parâmetro. */
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
    
        FatMes fatMes = (FatMes) o;
        return totalVendas == fatMes.getTotalVendas() &&
               totalFaturado == fatMes.getTotalFaturado() &&
               fatProds.equals(fatMes.getFatProds());
    }

    /** @return Representação textual desta faturação do mês. */
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separadorLinhas = System.getProperty("line.separator");

        sb.append("-> Faturação do mês " + mes + separadorLinhas);
        sb.append("Total de vendas: " + totalVendas + separadorLinhas);
        sb.append("Total faturado: " + totalFaturado + separadorLinhas);
        sb.append("Faturação dos produtos: " + separadorLinhas);
        for(FatProdMes fProdMes : fatProds.values())
            sb.append(fProdMes.toString());

        return sb.toString();
    }

    /** @return Valor do hash code desta faturação do mês. */
    public int hashCode(){
        int hash = 7;
        long aux;

        hash = 31*hash + totalVendas;
        aux = Double.doubleToLongBits(totalFaturado);
        hash = 31*hash + (int) (aux ^ (aux >>> 32));
        hash = 31*hash + fatProds.hashCode();
        return hash;
    }
}
