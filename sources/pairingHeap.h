#pragma once
#include <assert.h>
#include <vector>

//https://brilliant.org/wiki/pairing-heap/
//https://en.wikipedia.org/wiki/Pairing_heap#:~:text=Pairing%20heaps%20are%20heap%2Dordered,top%20element%20of%20the%20heap.
//https://www.geeksforgeeks.org/pairing-heap/

#undef min

struct PairingHeap
{

	using Type = int;

	struct Node
	{
		Node() = default;
		Node(Type val):value(val) {};

		Node *child = nullptr;
		Node *sibling = nullptr;
		Type value = Type();

		void addChild(Node *c)
		{
			if (child == nullptr) //if there are no children just add it
			{
				child = c;
			}else // else add it to the list of siblings of this node child
			{
				c->sibling = child;
				child = c;
			}
		}
	};

	PairingHeap() = default;
	PairingHeap(Node *n):root(n) {};


	struct Internal
	{
		static Node *merge(Node *a, Node *b)
		{
			if (a == nullptr) { return b; }
			if (b == nullptr) { return a; }

			if (a->value < b->value) // the root element with the smallest value remains root
			{
				a->addChild(b);
				return a;
			}
			else
			{
				b->addChild(a);
				return b;
			}
		}

		static Node *twoPassMerge(Node *node)
		{
			if (node == nullptr || node->sibling == nullptr)
			{return node;}

			Node *a, *b, *newNode;
			a = node;
			b = node->sibling;
			newNode = node->sibling->sibling;

			a->sibling = NULL;
			b->sibling = NULL;

			return merge(merge(a, b), twoPassMerge(newNode));
		}

	};

	Node *root = nullptr;

	void build(const std::vector<Type> &v)
	{
		for(const auto &i : v)
		{
			insert(i);
		}
	}

	std::vector<Type> getSortedElements()
	{
		std::vector<Type> v;

		while (!empty())
		{
			v.push_back(root->value);
			
			deleteFirst();
		}

		return std::move(v);
	}

	Type min()
	{
		assert(root != nullptr);
		return root->value;
	}
	
	bool empty()
	{
		return root == nullptr;
	}


	void meld(PairingHeap &other)
	{
		auto newRoot = Internal::merge(this->root, other.root);
		this->root = newRoot;
		//other.root = nullptr;
	}

	void insert(Type val)
	{
		Node *n = new Node(val);
		PairingHeap heap(n);

		this->meld(heap);
	}

	void deleteFirst()
	{
		if (empty()) { return; }

		auto child = root->child;
		delete this->root;

		this-> root = Internal::twoPassMerge(child);
	}


};