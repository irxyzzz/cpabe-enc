#include "Leaf.h"
#include "ByteString.h"
#include <pbc.h>
#include "Node.h"
#include <openssl/sha.h>
#include "Utility.h"
#include <string.h>

Leaf::Leaf(unsigned char* buf, pairing_t* p){
  int pos = 0;
  this->k = Utility::str2int(buf);
  pos += 4;
  this->num = Utility::str2int(buf + pos);
  pos += 4;

  memcpy((void*)this->value, buf + pos, SHA_DIGEST_LENGTH);
  pos += SHA_DIGEST_LENGTH;

  element_init_G1(this->cji, *p);
  pos += element_from_bytes(this->cji, buf + pos);
  

}

Leaf::Leaf(int k, int num, char* value, int len){
  this->k = k;
  this->num = num;
  SHA1((unsigned char*)value, len, this->value);
  this->computed = false;
}

int Leaf::getK(){
  return this->k;
}

int Leaf::getNum(){
  return this->num;
}

NodeType Leaf::getType(){
  return LEAF;
}

bool Leaf::isComputed(){
  return this->computed;
}

void Leaf::compute(element_t* v, PubParam* pub_p, pairing_t* p){
  //read the attribute set from file "/home/easy/work/eabe/param/attr_hash.dat"
  unsigned char buf_hash[N+1][SHA_DIGEST_LENGTH];
  const char* attr_hash_path = "../param/attr_hash.dat";
  int i;
  FILE *fp = fopen(attr_hash_path, "rb");
  if (fp  == NULL){
//to do: return errno
    printf("can't open file %s for writing\n", attr_hash_path);
    exit(1);
  }
  fseek(fp, 0, SEEK_SET);

  for(i=1;i<=N;i++)
    fread(buf_hash[i],sizeof(unsigned char),SHA_DIGEST_LENGTH,fp);
 // printf("%d\n/////////////////////",N);

  fclose(fp);

  //find the position of the leaf atttribute in the big omega set
  for(i=1; i<=N; i++){
 //   for(j=0;j<SHA_DIGEST_LENGTH;j++)
 //    printf("%d %d\n",this->value[j],buf_hash[i][j]);
    if(strncmp((const char*)this->value,(const char*)buf_hash[i],SHA_DIGEST_LENGTH) == 0)
      break;

    
  }
// printf("%d\n",i);
  //calculate the cji with the position above
  element_init_G1(this->cji, *p);
  element_pow_zn(this->cji, pub_p->T[i], *v);

  
  computed = true;
}

bool Leaf::valueEquals(unsigned char* value){
  if(strncmp((const char*)(this->value), (const char*)value, SHA_DIGEST_LENGTH) == 0){
    return true;
  } else {
    return false;
  }
}

int Leaf::toByteString(ByteString& bytes){
  int size = 0;
  unsigned  char buf[4];
  Utility::int2str(buf, this->k);
  bytes.append(buf, 4);
  size += 4;
  Utility::int2str(buf, this->num);
  bytes.append(buf, 4);
  size += 4;

  size += bytes.append(this->value, SHA_DIGEST_LENGTH);
  
  if(isComputed()){
    int len = element_length_in_bytes(this->cji);
    unsigned char* t = (unsigned char*)malloc(sizeof(unsigned char) * len);
    element_to_bytes(t, this->cji);
    bytes.append(t, len);
    free(t);
    size += len;

    
  }

  return size;
}
