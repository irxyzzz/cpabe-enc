#include "CipherText.h"
#include <openssl/sha.h>
#include <pbc.h>
#include <string.h>
#include "Node.h"
#include "InternalNode.h"
#include "Leaf.h"
#include "ExLeaf.h"

const int ELEMENT_LENGTH = 260;  //remove it for rubust

CipherText::CipherText(Policy* policy, element_t* m,  PubParam* pub, pairing_t* p){
  this->pub = pub;
  this->p = p;
  this->m = m;
  this->policy = policy;

  //init random s
  this->s = (element_t*)malloc(sizeof(element_t));
  element_init_Zr(*(this->s),*(this->p));
  element_random(*(this->s));
 // printf("excute before init_c1\n");

  //init c1
  init_c1();
//  printf("ciphertext::init_c1() ok\n");

  //init c0
  init_c0();
 // printf("ciphertext::init_c0() ok\n");
 
  apply_policy();
  printf("ciphertext: apply_policy ok\n");
  
}

CipherText::CipherText(unsigned char* buf, pairing_t* p){
  this->p = p;
  this->m = NULL;
  this->s = NULL;
  element_init_GT(this->c1, *p);
  element_init_G1(this->c0, *p);
  int pos = 0;
  pos += element_from_bytes(this->c1, buf + pos);
  element_printf("c1:%B\n", this->c1);
  pos += element_from_bytes(this->c0, buf + pos);
  element_printf("c0:%B\n", this->c0);
  printf("pos: %d -> reconstruct c1 and c0 ok\n", pos);
  this->policy = new Policy(buf + pos, p);
  printf("reconstruct policy ok\n");
}


Policy* CipherText::getPolicy(){
  return this->policy;
}

int CipherText::toByteString(ByteString& bytes){
  int begin = bytes.getLength();
  unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * ELEMENT_LENGTH);
  element_to_bytes(buf, this->c1);
  bytes.append(buf, ELEMENT_LENGTH);
  element_to_bytes(buf, this->c0);
  bytes.append(buf, ELEMENT_LENGTH);
  free(buf);
  this->policy->toByteString(bytes);
  int end = bytes.getLength();
  return end - begin;
}
void CipherText::init_c0(){
  element_init_G1(this->c0,*(this->p));
  element_pow_zn(this->c0, this->pub->g1, *(this->s));//g^s
//  element_printf("c0:%B\n", this->c0);
}

void CipherText::init_c1(){
  element_init_GT(this->c1, *(this->p));//e(g1,g2)
  element_pow_zn(this->c1, this->pub->y, *(this->s));//e(g1,g2)^alpha*s    y = e^(g1,g2)^alpha
  element_mul(this->c1, *(this->m), this->c1);//m*e(g1,g2)^alpha*s
//  element_printf("c1:%B\n", this->c1);
}

void CipherText::apply_policy(){
  compute_node(*(this->s), this->policy->getRoot());
}

void CipherText::compute_node(element_t& v, Node* node){//v amounts to s
  if(node->getType() == LEAF){
    Leaf* leaf = (Leaf*)node;
    leaf->compute(&v, this->pub, this->p);
  //  printf("leaf: %d, %d, computed\n", leaf->getK(), leaf->getNum());

  } else if (node->getType() == INTERNAL_NODE){

    InternalNode* internalNode = (InternalNode*)node;
    int num = internalNode->getNum();
    int k = internalNode->getK();
    Node** sons = internalNode->getSons();//??

  //  printf("internal Node: %d, %d\n", k, num);

    element_t* ys = (element_t*)malloc(sizeof(element_t) *  (num + 1));      
    element_init_Zr(ys[0], *(this->p));
    element_set(ys[0], v);                          //set ys[0] to v
    computePoints(ys, k, num);       //compute other num point, 
    int i = 1;
    for (i = 1; i <= num; i++){
      compute_node(ys[i], sons[i - 1]);
    }
  }
}
void CipherText::computePoints(element_t* ys, int k, int num){
  int i = 1;
  //select  k - 1 random points in polynomial curve. Positions are (1, random1),(2, random2) ... (k - 1, random(k - 1))
  for (i = 1; i < k ; i++){
    element_init_Zr(ys[i], *(this->p));
    element_random(ys[i]);
  }

  //compute other num -k + 1 points
  for( i = k; i <= num; i ++){
    langrange(ys, i, k, num);
  }
}

void CipherText::langrange(element_t* ys, int index, int k, int num){
  element_t delta;
  element_t numerator;
  element_t denominator;
  element_t temp;
  element_init_Zr(delta, *(this->p));
  element_init_Zr(numerator, *(this->p));
  element_init_Zr(denominator, *(this->p));
  element_init_Zr(temp, *(this->p));
  element_init_Zr(ys[index], *(this->p));
  element_set0(ys[index]);
  int i, j;
  for(i = 0; i < k; i++){
    //compute the langrange coefficent l
    element_set1(delta);
    for(j = 0; j < k; j++){
      if( j != i){
        element_set_si(numerator, index - j);
        element_set_si(denominator, i - j);
        element_div(numerator, numerator, denominator);
        element_mul(delta, delta, numerator);
      }
    }

    element_mul(temp, ys[i], delta);
    element_add(ys[index], ys[index], temp);
  }
}
