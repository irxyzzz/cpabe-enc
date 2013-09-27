#include "ExLeaf.h"

ExLeaf::ExLeaf(char* value, int len){
  this->value.append((unsigned char*)value, len);
}

void ExLeaf::getValue(ByteString& bytes){
  bytes.append(this->value);
}

NodeType ExLeaf::getType(){
  return EXTENDED_LEAF;
}

int ExLeaf::toByteString(ByteString& bytes){
  this->getValue(bytes);
  return this->value.getLength();
}

int ExLeaf::getLength(){
  return this->value.getLength();
}

