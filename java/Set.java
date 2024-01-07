abstract class Set {
	public abstract boolean insert(int i);
	public abstract boolean remove(int i);
	public abstract boolean contains(int i);
	public abstract int     size();
	public abstract String  toString();

	public static Set new_boxed() {
		return new SmallSet();
	}

	public static void main(String[] args) {
		var set = Set.new_boxed();
		// var set = new InheritanceSet();
		for (int i : new int[] {5, 3, 7, 2, 5, 4, 6, 8}) {
			set.insert(i);
		}

		System.out.printf("Contents: {%s} | Size: %d%n", set.toString(), set.size());

		for (int i = 0; i < 20; i++) {
			set.insert(i);
		}

		System.out.printf("Contents: {%s} | Size: %d%n", set.toString(), set.size());
	}
}

class InheritanceSet extends Set {
	Set s;

	@Override
	public boolean insert(int i) { return s.insert(i); }
	@Override
	public boolean remove(int i) { return s.remove(i); }
	@Override
	public boolean contains(int i) { return s.contains(i); }
	@Override
	public int size() { return s.size(); }
	@Override
	public String toString() { return s.toString(); }
}

class LargeSet extends Set {
	class Node {}
	Node root = null;

	@Override
	public boolean insert(int i) { return false; }
	@Override
	public boolean remove(int i) { return false; }
	@Override
	public boolean contains(int i) { return false; }
	@Override
	public int size() { return 0; }
	@Override
	public String toString() { return ""; }
}

class SmallSet extends Set {
	int[] buffer;
	int nums;

	@Override
	public boolean insert(int i) { return false; }
	@Override
	public boolean remove(int i) { return false; }
	@Override
	public boolean contains(int i) { return false; }
	@Override
	public int size() { return 0; }
	@Override
	public String toString() { return ""; }

	protected SmallSet() {
		super();
	}
}
