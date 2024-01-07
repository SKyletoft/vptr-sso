abstract class Set {
	public abstract boolean insert(int n);
	public abstract boolean remove(int n);
	public abstract boolean contains(int n);
	public abstract int     size();
	public abstract String  toString();

	public static Set new_boxed() {
		// return new SmallSet();
		return new LargeSet();
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

	@Override public boolean insert(int i)   { return s.insert(i); }
	@Override public boolean remove(int i)   { return s.remove(i); }
	@Override public boolean contains(int i) { return s.contains(i); }
	@Override public int     size()          { return s.size(); }
	@Override public String  toString()      { return s.toString(); }
}

class LargeSet extends Set {
	class Node {
		int val;
		Node left;
		Node right;

		public Node(int val) {
			this.val = val;
		}
	}
	Node root = null;

	Node[] find(int n) {
		Node node = this.root;
		Node parent = null;

		for (;;) {
			if (node.val == n) {
				break;
			}
			Node next = (node.val > n)
				? node.left
				: node.right;
			if (next == null) {
				break;
			}
			parent = node;
			node = next;
		}

		return new Node[] { parent, node };
	}

	@Override
	public boolean insert(int n) {
		Node node = this.find(n)[1];
		if (node.val > n) {
			assert(node.left == null);
			node.left = new Node(n);
			return true;
		} else if (node.val < n) {
			assert(node.right == null);
			node.right = new Node(n);
			return true;
		} else {
			assert(node.val == n);
			return false;
		}
	}

	@Override
	public boolean remove(int n) {
		Node parent, node;
		{
			var parent_and_node = this.find(n);
			parent = parent_and_node[0];
			node = parent_and_node[1];
		}

		if (node.val != n) {
			return false;
		}

		if (node.left == null && node.right == null) {
			if (parent.left == node) {
				parent.left = null;
			} else {
				parent.right = null;
			}
		} else if (node.left == null && node.right != null) {
			if (parent.left == node) {
				parent.left = node.right;
			} else {
				parent.right = node.right;
			}
		} else if (node.left != null && node.right == null) {
			if (parent.left == node) {
				parent.left = node.left;
			} else {
				parent.right = node.left;
			}
		} else {
			Node replace_with = node.right;
			while (replace_with.left != null) {
				parent = replace_with;
				replace_with = replace_with.left;
			}
			node.val = replace_with.val;
			parent.left = null;
		}

		return true;
	}

	@Override
	public boolean contains(int i) { return false; }

	@Override
	public int size() { return 0; }

	@Override
	public String toString() { return ""; }
}

class SmallSet extends Set {
	static int SIZE_MAX = 15;
	int[] buffer;
	int nums;

	int find(int n) {
		int i;
		for (i = this.nums - 1; 0 <= i && i < this.nums; i--) {
			if (this.buffer[i] == n) {
				break;
			}
		}
		return i;
	}

	@Override
	public boolean insert(int n) {
		if (this.contains(n)) {
			return false;
		}
		if (this.nums >= SmallSet.SIZE_MAX) {
			assert(false);
		}
		this.buffer[this.nums++] = n;
		return true;

	}

	@Override
	public boolean remove(int n) {
		int i = this.find(n);
		if (i == -1) {
			return false;
		}
		var tmp = this.buffer[i];
		this.buffer[i] = this.buffer[this.nums - 1];
		this.buffer[this.nums - 1] = tmp;
		this.nums--;
		return true;
	}

	@Override
	public boolean contains(int n) {
		return this.find(n) != -1;
	}

	@Override
	public int size() {
		return this.nums;
	}

	@Override
	public String toString() {
		return "";
	}

	protected SmallSet() {
		super();
		this.buffer = new int[SmallSet.SIZE_MAX];
	}
}
