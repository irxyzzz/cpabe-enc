#ifndef CIPHERTEXT_H_
#define CIPHERtEXT_H_
#include "Object.h"
#include "PubParam.h"
#include "Policy.h"
#include "ByteString.h"


#include <pbc.h>
#include <stdlib.h>

class CipherText{
  public:
    //to-do:update to access Tree
    element_t c1;   // c1 = M*e(g, g)^(alpha*s) GT
    element_t c0;   // c0 = g^s                G1
    Policy* policy;
    pairing_t* p;
/*    element_t cy;  //cy = g^q(0)               G1
    element_t cyt;  //ca = H(att(y))^q(0)       G2
*/
  public:
    CipherText(Policy* policy, element_t* m, PubParam* pub, pairing_t* p);
    CipherText(unsigned char* buf, pairing_t* p);
    int toByteString(ByteString& bytes);
    Policy* getPolicy();

  private:
    PubParam* pub;
    element_t* s;
    element_t* m;
  private:
    void init_c0();
    void init_c1();
    void apply_policy();
    void compute_node(element_t& v, Node* node);
    void computePoints(element_t* ys, int k, int num);
    void langrange(element_t* ys,int index, int k, int num);
};
#endif
