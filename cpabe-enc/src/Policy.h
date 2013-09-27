#ifndef POLICY_H_
#define POLICY_H_
#include "Object.h"
#include "Node.h"
#include "ByteString.h"
#include <pbc.h>

class Policy {
private:
	int nodeNum;
	Node* root;
	int nodeIndex;
public:
	Policy(Node* root);
	Policy(unsigned char* buf, pairing_t* p);
	int addSon(Node* node, Node* son);
	Node* getRoot();
	int getNodeNum();
	int toByteString(ByteString& bytes);
private:
	int nodeToByteString(int father, Node* node, ByteString& bytes);
};
#endif
