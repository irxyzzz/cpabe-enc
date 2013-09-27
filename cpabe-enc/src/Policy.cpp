#include "Policy.h"
#include "Node.h"
#include <stdlib.h>
#include "Utility.h"
#include <pbc.h>
#include "InternalNode.h"
#include "Leaf.h"
#include "ExLeaf.h"

static const int LEAF_SIZE = 288; //4(k) + 4(num) + 20(attribute hash value) + 260(element cji)
static const int INTERNAL_NODE_SIZE = 8; // 4 + 4

Policy::Policy(Node* root) {
	this->root = root;
	this->nodeNum = 1;
}

Policy::Policy(unsigned char *buf, pairing_t* p) {
	//get nodeNum
	int pos = 0;
	int numberOfNode = Utility::str2int(buf);
	pos += 4;

//  printf("node num is %d\n", numberOfNode);
	this->nodeNum = numberOfNode;

	//reconstruct access tree form bytes
	Node** nodes = (Node**) malloc(sizeof(Node*) * this->nodeNum); //store all nodes
	int index = 0;
	Node* node = NULL;
	int nodeSize = 0;
	for (index = 0; index < numberOfNode; index++) {
		printf("index: %d\n", index);
		//get father position
		int father = Utility::str2int(buf + pos);

		pos += 4;
		printf("father is %d\t", father);
		//get node size to indentify node type
		nodeSize = Utility::str2int(buf + pos);
		pos += 4;
		//printf("node size is %d\t", nodeSize);

		if (nodeSize == -1) {
			nodeSize = INTERNAL_NODE_SIZE;
			node = new InternalNode(buf + pos);
			printf("internal node\t");
			InternalNode* t = (InternalNode*) node;
			printf("%d, %d\t", t->getK(), t->getNum());
		} else if (nodeSize == -2) {
			nodeSize = LEAF_SIZE;
			node = new Leaf(buf + pos, p);
			// printf("leaf\t");
		} else {
			node = new ExLeaf((char*) (buf + pos), nodeSize);
			//  printf("ex leaf\t");
		}
		pos += nodeSize;
		nodes[index] = node;

		if (father == -1) {
			this->root = node;
		} else {
			this->addSon(nodes[father], node);
		}
		// printf("add son to node %d\n", father);
	}

//  printf("reconstruct node ok\n");
	free(nodes);
}

int Policy::addSon(Node* node, Node* son) {
	this->nodeNum += 1;
	return node->addSon(son);
}

Node* Policy::getRoot() {
	return this->root;
}

int Policy::toByteString(ByteString& bytes) {
	this->nodeIndex = -1;
	unsigned char buf[4];
	Utility::int2str(buf, this->nodeNum);
	bytes.append(buf, 4);
	return 4 + nodeToByteString(-1, this->root, bytes);
}

int Policy::nodeToByteString(int father, Node* node, ByteString& bytes) {
	if (node == NULL) {
		return 0;
	}

	//find a node
	this->nodeIndex += 1;
	int sonFather = this->nodeIndex;
	int size = 0;

	//append father position
	unsigned char buf[4];
	Utility::int2str(buf, father);
	bytes.append(buf, 4);
	size += 4;

	//length of bytes of node, -1 represents internal node, -2 represents leaf, >0 represents the real length of exLeaf
	int node_len = -1;
	if (node->getType() == LEAF) {
		node_len = -2;
		////  printf("leaf node");
	} else if (node->getType() == EXTENDED_LEAF) {
		ExLeaf* tempNode = (ExLeaf*) node;
		node_len = tempNode->getLength();
		// printf("exLeaf node");
	}

	//append len info
	Utility::int2str(buf, node_len);
	bytes.append(buf, 4);
	size += 4;

	//append node itself info
	size += node->toByteString(bytes);

//  printf("\tnode size: %d\n", size);

	//handle sons of node
	if (node->getSonNum() > 0) {
		int i = 0;
		Node** sons = node->getSons();
		for (i = 0; i < node->getSonNum(); i++) {
			size += nodeToByteString(sonFather, sons[i], bytes);
		}
	}

	return size;
}

int Policy::getNodeNum() {
	return this->nodeNum;
}

