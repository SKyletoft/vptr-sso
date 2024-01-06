#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cassert>

#include <functional>
#include <memory>

#include "Set.h"

static int depth(SetNode *node) {
	if (node == nullptr) {
		return 0;
	}
	return 1 + std::max(
		depth(node->left.get()),
		depth(node->right.get())
	);
}

// Convert a Smallset to a LargeSet in place
LargeSet *grow_set(SmallSet *small) {
	SmallSet local_copy = *small;

	small->~SmallSet();
	LargeSet *large = new (small) LargeSet {};

	assert(local_copy.nums == SmallSet::MAX_SIZE);

	// Hardcoded index order instead of acually having a balanced tree
	static_assert(SmallSet::MAX_SIZE == 15);
	std::sort(std::begin(local_copy.buffer), std::end(local_copy.buffer));
	static constexpr size_t INDICIES[] = {7, 3, 1, 0, 2, 5, 4, 6, 11, 9, 8, 10, 13, 12, 14};
	for (size_t i : INDICIES) {
		large->insert(local_copy.buffer[i]);
	}

	return large;
}

Box<Set> Set::new_boxed() {
	return std::make_unique<SmallSet>(SmallSet {});
}

size_t SmallSet::find(int n) {
	size_t i;
	// size_t is unsigned, so can't check against 0. Underflow is well defined as of recently
	for (i = this->nums-1; i < this->nums; i--) {
		if (this->buffer[i] == n) {
			break;
		}
	}

	if (i >= this->nums) {
		return SIZE_MAX;
	}

	return i;
}

bool SmallSet::insert(int n) {
	if (this->contains(n)) {
		return false;
	}
	if (this->nums >= SmallSet::MAX_SIZE) {
		LargeSet *large = grow_set(this);
		return large->insert(n);
	}

	this->buffer[this->nums++] = n;
	return true;
}

bool SmallSet::remove(int n) {
	size_t i = this->find(n);

	if (i == SIZE_MAX) {
		return false;
	}

	std::swap(this->buffer[i], this->buffer[this->nums - 1]);
	this->nums--;

	return true;
}

bool SmallSet::contains(int n) {
	return this->find(n) == SIZE_MAX;
}

Tuple<SetNode *, SetNode *> LargeSet::find(int n) {
	SetNode *node = this->root.get();
	SetNode *parent = nullptr;
	for (;;) {
		if (node->val == n) {
			break;
		}
		SetNode *next = (node->val < n)
			? node->left.get()
			: node->right.get();
		if (next == nullptr) {
			break;
		}
		parent = node;
		node = next;
	}
	return {parent, node};
}

bool LargeSet::insert(int n) {
	auto [_, node] = this->find(n);
	int val = node->val;
	if (val == n) {
		return false;
	}

	((n < val) ? node->left : node->right) = std::make_unique<SetNode>(n);

	return true;
}

bool LargeSet::remove(int n) {
	auto [parent, node] = this->find(n);

	if (node->val != n) {
		return false;
	}

	Box<SetNode> &parent_ref = parent->left.get() == node
		? parent->left
		: parent->right;

	int combined = ((node->left == nullptr) << 1) | (node->right == nullptr);
	switch (combined) {
	case 0b00: {
		parent_ref = nullptr;
	} break;
	case 0b01: {
		std::swap(parent_ref, node->right);
	} break;
	case 0b10: {
		std::swap(parent_ref, node->left);
	} break;
	case 0b11: {
		SetNode *replace_with = node->right.get();
		SetNode *parent_of = parent_ref.get();
		while (replace_with->left.get() != nullptr) {
			parent_of = replace_with;
			replace_with = replace_with->left.get();
		}
		node->val = replace_with->val;
		parent_of->left = nullptr;
	} break;
	}

	return true;
}

bool LargeSet::contains(int n) {
	auto [_, res] = this->find(n);
	return res->val == n;
}

int main() {
	auto set = Set::new_boxed();
	for (int i = 0; i < 1024; i++) {
		set->insert(i);
	}

	std::printf("Hi\n");

	// for (int i : set) {
	//	std::printf("%d, ", i);
	// }
	// std::puts("");
}
