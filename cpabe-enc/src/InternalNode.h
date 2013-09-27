#ifndef INTERNALNODE_H_
#define INTERNALNODE_H_

#include "Node.h"
#include "ByteString.h"

class InternalNode: public Node{
  private:
    int k;
    int num;
  public:
    InternalNode(int k, int num);

    /**
    *reconstruct InternalNode from bytes form.
    */
    InternalNode(unsigned char* buf);
    int getK();
    int getNum();
    NodeType getType();

    /**
    *convert k and num into bytes
    */
    int toByteString(ByteString& bytes);
};




#endif
