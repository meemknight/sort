#include "pairingHeap.h"
#include <cstdlib>
#include <iostream>

int main()
{

	PairingHeap heap1;
	PairingHeap heap2;

	heap2.root = new PairingHeap::Node(4);

	heap1.root = new PairingHeap::Node(1);
	heap1.root->child = new PairingHeap::Node(2);
	heap1.root->child->sibling = new PairingHeap::Node(3);
	heap1.root->child->child = new PairingHeap::Node(6);
	heap1.root->child->child->sibling = new PairingHeap::Node(5);


	heap1.meld(heap2);



	std::cout << "hello\n";

	system("pause");
}