#ifndef EXLEAF_H_
#define EXLEAF_H_
#include "Node.h"
#include "ByteString.h"
class ExLeaf: public Node{
  private:
    ByteString value;
  public:
    ExLeaf(char* value, int len);
    void getValue(ByteString& bytes);
    NodeType getType();
    int toByteString(ByteString& bytes);
    int getLength();
};



#endif
