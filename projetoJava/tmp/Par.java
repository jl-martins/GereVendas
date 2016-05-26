import java.util.Arrays;

public class Par<X, Y>
{
	private X var1;
	private Y var2;

	public Par(X var1, Y var2)
	{
		this.var1 = var1;
		this.var2 = var2;
	}

	public Par()
	{
		var1 = null;
		var2 = null;
	}

	public Par(Par<X, Y> p)
	{
		this(p.fst(), p.snd());
	}

	public X fst() { return var1; }
	public Y snd() { return var2; }

	public boolean equals(Object o)
	{
		if(this == o) return true;
		if((o == null) || (this.getClass() == o.getClass())) return false;

		Par<X, Y> p = (Par<X, Y>) o;
		return var1.equals(p.fst()) && var2.equals(p.snd());
	}

	public Par<X, Y> clone()
	{
		return new Par(this);
	}

	public String toString()
	{
		StringBuilder str = new StringBuilder();
		str.append("("); str.append(var1.toString());
		str.append(",");
		str.append(var2.toString()); str.append(")");
		return str.toString();
	}

	public int hashCode()
	{
		return Arrays.hashCode(new Object[] { var1, var2 });
	}
}
