#include "PubParam.h"
#include "Utility.h"

PubParam::PubParam(pairing_t& p, element_t& alpha, element_t* t){
  //initial g1
  element_init_G1(g1, p);
  element_random(g1);
  //initial g2
  element_init_G2(g2, p);
  element_random(g2);
  
  //calculate T[j]
  int j = 0;
  for(j=1;j<=N;j++){
    element_init_G1(T[j], p);
    element_pow_zn(T[j], g1, *(t+j));

  }
  //initial y
  element_init_GT(y, p);
  pairing_apply(y, g1, g2, p);
  element_pow_zn(y, y, alpha);

}

PubParam::PubParam(pairing_t& p, unsigned char* buf){
  int i = 0;
  
  element_init_G1(g1, p);
  element_init_G2(g2, p);
  for(i=1;i<=N;i++)
    element_init_G1(T[i], p);
  element_init_GT(y, p);

  int len = 0;
  element_from_bytes(g1, buf + len);
  
  len += element_length_in_bytes(g1);
  element_from_bytes(g2, buf + len);

  len += element_length_in_bytes(g2);
  for(i=1;i<=N;i++){
    element_from_bytes(T[i], buf + len);
    len += element_length_in_bytes(T[i]);
    
  }
  putchar('\n');
  element_from_bytes(y, buf + len);
}

/**
*@overload
*/
size_t PubParam::getSize(){
   size_t temp = 0;
   for(int i=1;i<=N;i++)
     temp += element_length_in_bytes(T[i]);
	
   return element_length_in_bytes(g1)
         + element_length_in_bytes(g2)
         + element_length_in_bytes(y)
         + temp;
}

/**
*@overload
*/
size_t PubParam::toBytes(unsigned char* buf){
  size_t len = 0;
  len += element_to_bytes(buf, g1);
  len += element_to_bytes(buf + len, g2);
  for(int i=1;i<=N;i++)
     len += element_to_bytes(buf + len, T[i]);
  len += element_to_bytes(buf + len, y);
 
  return len;
}

PubParam::~PubParam(){
  element_clear(g1);
  element_clear(g2);
  for(int i=1;i<=N;i++)
    element_clear(T[i]);
  element_clear(y);
}
