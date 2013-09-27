#include "InternalNode.h"
#include "Node.h"
#include "ByteString.h"
#include "Utility.h"

InternalNode::InternalNode(int k, int num){
  this->k = k;
  this->num = num;
}

InternalNode::InternalNode(unsigned char* buf){
  this->k = Utility::str2int(buf);
  this->num = Utility::str2int(buf + 4);
}

int InternalNode::getK(){
  return this->k;
}

int InternalNode::getNum(){
  return this->num;
}

NodeType InternalNode::getType(){
  return INTERNAL_NODE;
}

int InternalNode::toByteString(ByteString& bytes){
  //convert k and num into byte formate
  unsigned char buf[4];
  Utility::int2str(buf, this->k);
  bytes.append(buf, 4);
  Utility::int2str(buf, this->num);
  bytes.append(buf, 4);
  return 4 + 4;
}
