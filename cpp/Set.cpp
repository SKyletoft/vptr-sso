#include <cstddef>
#include <cstdio>
#include <cassert>

#include <functional>
#include <iostream>
#include <sstream>

#include "Set.h"

// Convert a Smallset to a LargeSet in place
auto grow_set(SmallSet *small) -> LargeSet * {
	SmallSet local_copy = *small;

	small->~SmallSet();
	LargeSet *large = new (small) LargeSet {};

	assert(local_copy.nums == SmallSet::MAX_SIZE);

	// Hardcoded index order instead of acually having a balanced tree
	static_assert(SmallSet::MAX_SIZE == 15);
	std::sort(std::begin(local_copy.buffer), std::end(local_copy.buffer));
	large->root =
		std::make_unique<SetNode>(SetNode {
			.val = local_copy.buffer[7]
		});

	static constexpr size_t INDICIES[] = {3, 1, 0, 2, 5, 4, 6, 11, 9, 8, 10, 13, 12, 14};
	for (size_t i : INDICIES) {
		large->insert(local_copy.buffer[i]);
	}

	return large;
}

auto Set::new_boxed() -> Box<Set> {
	return std::make_unique<SmallSet>(SmallSet {});
}

auto SmallSet::find(int n) -> size_t {
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

auto SmallSet::insert(int n) -> bool {
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

auto SmallSet::remove(int n) -> bool {
	size_t i = this->find(n);

	if (i == SIZE_MAX) {
		return false;
	}

	std::swap(this->buffer[i], this->buffer[this->nums - 1]);
	this->nums--;

	return true;
}

auto SmallSet::contains(int n) -> bool {
	return this->find(n) != SIZE_MAX;
}

auto SmallSet::size() -> size_t {
	return this->nums;
}

auto SmallSet::to_string() -> std::string {
	std::ostringstream s;
	for (size_t i = 0; i < this->nums; ++i) {
		s << this->buffer[i] << ", ";
	}
	std::string ret = s.str();
	ret.pop_back();
	ret.pop_back();
	return ret;
}

auto LargeSet::find(int n) -> Tuple<SetNode *, SetNode *> {
	SetNode *node = this->root.get();
	SetNode *parent = nullptr;
	for (;;) {
		if (node->val == n) {
			break;
		}
		SetNode *next = (node->val > n)
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

auto LargeSet::insert(int n) -> bool {
	auto [_, node] = this->find(n);
	if (node->val > n) {
		assert(node->left == nullptr);
		node->left = std::make_unique<SetNode>(n);
		return true;
	} else if (node->val < n) {
		assert(node->right == nullptr);
		node->right = std::make_unique<SetNode>(n);
		return true;
	} else {
		assert(node->val == n);
		return false;
	}
}

auto LargeSet::remove(int n) -> bool {
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

auto LargeSet::contains(int n) -> bool {
	auto [_, res] = this->find(n);
	return res->val == n;
}

auto LargeSet::size() -> size_t {
	std::function<size_t(SetNode *)> f;
	f = [&](SetNode *node) -> size_t {
		if (node == nullptr) {
			return 0;
		} else {
			return 1 + f(node->left.get()) + f(node->right.get());
		}
	};

	return f(this->root.get());
}

auto LargeSet::to_string() -> std::string {
	std::ostringstream s {};
	std::function<void(SetNode *)> f;
	f = [&](SetNode *node) {
		if (node != nullptr) {
			f(node->left.get());
			s << node->val << ", ";
			f(node->right.get());
		}
	};

	f(this->root.get());
	std::string ret = s.str();
	ret.pop_back();
	ret.pop_back();
	return ret;
}

auto inorder(SetNode* root) -> void {
	if (root != nullptr) {
		inorder(root->left.get());
		printf("%d ", root->val);
		inorder(root->right.get());
	}
}

auto main() -> int {
	auto set = Set::new_boxed();
	for (int i : {5, 3, 7, 2, 5, 4, 6, 8}) {
		set->insert(i);
	}

	{
		SmallSet &small_set = dynamic_cast<SmallSet &>(*set);
		std::cout << "Content: {" << small_set.to_string() << "} | Total size: " << small_set.size() << '\n';
	}

	for (int i = 0; i < 20; ++i) {
		set->insert(i);
	}

	{
		LargeSet &large_set = dynamic_cast<LargeSet &>(*set);
		std::cout << "Content: {" << large_set.to_string() << "} | Total size: " << large_set.size() << '\n';
	}
}
