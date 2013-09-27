#ifndef LEAF_H_
#define LEAF_H_

#include "Node.h"
#include "ByteString.h"
#include "PubParam.h"
#include <pbc.h>
#include <openssl/sha.h>

class Leaf: public Node{
  private:
    int k;
    int num;
    unsigned char value[SHA_DIGEST_LENGTH];
    bool computed;

  public:
    element_t cji;

  public:
    Leaf(int k, int num, char* value, int len);

    /**
    *reconstruct Leaf from bytes form.
    */
    Leaf(unsigned char* buf, pairing_t* p);
    int getK();
    int getNum();
    NodeType getType();

    /**
    *whether cji is computed
    */
    bool isComputed();

    bool valueEquals(unsigned char* value);

    /**
    *compute cji
    */
    void compute(element_t* v, PubParam* pub_p, pairing_t* p);

    /**
    *convert k, num, hash value into bytes
    *if leaf is computed, append cy and cyt
    */
    int toByteString(ByteString& bytes);
};
#endif
