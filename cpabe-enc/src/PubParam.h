#ifndef PUBPARAM_H_
#define PUBPARAM_H_

#include "Object.h"
#include <pbc.h>


#define N 170  //the number of the attributes in the big omega set
#define NN 50  //the longest length of the attributes in the big omega set



class PubParam:public Object{
  public:
    element_t g1;  //G1
    element_t g2;  //G2
    element_t y;   // y = e^(g1,g2)^alpha
    element_t T[N+1];// T[j]=g1^tj
    
  public:

    /*init pubParam using paring and one component of master key--beta */
    PubParam(pairing_t& p, element_t& alpha, element_t* t);
    
    /*init pubParam by pubPram file and pairing */
    PubParam(pairing_t& p, unsigned char* buf);

    ~PubParam();
    
    size_t getSize();
    size_t toBytes(unsigned char*  buf);

};

#endif
