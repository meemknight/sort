#pragma once
#include <assert.h>

//https://brilliant.org/wiki/pairing-heap/
//https://en.wikipedia.org/wiki/Pairing_heap#:~:text=Pairing%20heaps%20are%20heap%2Dordered,top%20element%20of%20the%20heap.
//https://www.geeksforgeeks.org/pairing-heap/

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
	};

	Node *root = nullptr;

	
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

};