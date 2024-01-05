#pragma once

#include <memory>
#include <tuple>

template <typename T> using Box = std::unique_ptr<T>;
template <typename T, typename U> using Tuple = std::tuple<T, U>;

class Set;
class SmallSet;
class LargeSet;
LargeSet *grow_set(SmallSet *small);

// Note: Value-based. No references to contained objects available
class Set {
public:
	virtual bool insert(int)   = 0;
	virtual bool remove(int)   = 0;
	virtual bool contains(int) = 0;
	virtual ~Set() {}

	static Box<Set> new_boxed();
};

class SmallSet : public Set {
	static constexpr size_t MAX_SIZE = 15;

	int buffer[MAX_SIZE];
	size_t nums = 0;

	friend class Set;
	friend class LargeSet;
	friend LargeSet *grow_set(SmallSet*);

	size_t find(int);

	SmallSet() {}
public:
	virtual bool insert(int)   override;
	virtual bool remove(int)   override;
	virtual bool contains(int) override;
};

struct SetNode {
	int val;
	Box<SetNode> left = nullptr;
	Box<SetNode> right = nullptr;

	SetNode(int n) : val(n) {}
};

class alignas(alignof(SmallSet)) LargeSet : public Set {
	Box<SetNode> root = nullptr;
	// char _padding[sizeof(SmallSet) - sizeof(Box<SetNode>) - sizeof(size_t)];

	friend class Set;
	friend class SmallSet;
	friend LargeSet *grow_set(SmallSet*);

	Tuple<SetNode *, SetNode *> find(int);
	LargeSet() {}
public:
	virtual bool insert(int)   override;
	virtual bool remove(int)   override;
	virtual bool contains(int) override;
};

static_assert(sizeof(SmallSet) >= sizeof(LargeSet));
static_assert(alignof(SmallSet) == alignof(LargeSet));
