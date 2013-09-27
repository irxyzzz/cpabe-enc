#include <pbc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PubParam.h"
#include "ByteString.h"
#include "Utility.h"
#include "Node.h"
#include "Leaf.h"
#include "ExLeaf.h"
#include "InternalNode.h"
#include "CipherText.h"


Policy* init_policy(char* policy_path){
  char buf[1024];
  FILE* f = fopen(policy_path, "r");
  int len = 0;
  ByteString bytes;
  while(!feof(f)) {
    len = fread(buf, 1, 1024, f);
    bytes.append((unsigned char*)buf, len);
  }
  printf("read policy file ok\n");

  len = bytes.getLength();
  unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char*) * len);
  bytes.toBytes(data);
   
  //get node number
  len = len -4;
  int node_num = Utility::str2int(data + len);
  printf("node num is %d\n", node_num);
  
  int* fathers = (int*)malloc(sizeof(int) * node_num);//to store the position of the node's father
  Node** nodes = (Node**)malloc(sizeof(Node*) * node_num);
  int i = 0;
  int pos = 0;
  int element_len = 0;
  for(i = 0; i < node_num; i++){
    int father = Utility::str2int(data + pos);
    printf("father is %d\n", father);
    pos += 4;
    fathers[i] = father;
    int type = Utility::str2int(data + pos);
    pos += 4;
    printf("father, type --> %d, %d\n", father, type);
    if(type == -1){ // internal node
      Node* node = new InternalNode(data + pos);
      pos += 8;
      nodes[i] = node;
    } else if(type > 0) {// exleaf node
      element_len = type;//variable length
      Node* node = new ExLeaf((char*)(data + pos), element_len);
      pos += element_len;
      nodes[i] = node;
    } else { // leaf node
      int k = Utility::str2int(data + pos);
      pos += 4;
      int num = Utility::str2int(data + pos);
      pos += 4;
      element_len = -type - 12;
      Node* node = new Leaf(k, num, (char*)(data + pos), element_len);
      pos += element_len;
      nodes[i] = node;
     }
  }

  Policy* policy = new Policy(nodes[0]);
  for (i = 1; i < node_num; i++){
    policy->addSon(nodes[fathers[i]], nodes[i]);
  }


  free(data);
  free(fathers);
  free(nodes);

  return policy;
}
  
/**
*@param args [path of program file path , path of policy]
*/
int main(int argc, char* args[]){
  if( argc < 3) {
    printf("please append program file path and the path of policy as parameters\n");
    return -1;
  }
  //path of param
  int param_path_len = strlen(args[1]) + strlen("/param/a1.param") + 1;
  char* param_path = (char*)malloc(param_path_len);
  strcpy(param_path, args[1]);
  strcpy(param_path + strlen(args[1]), "/param/a1.param");
  param_path[param_path_len] = '\0';
  printf("param_path: %s\n", param_path);

  //path of pub_param
  int pub_param_path_len = strlen(args[1]) + strlen("/key/abe.pub") + 1;
  char* pub_param_path = (char*)malloc(pub_param_path_len);
  strcpy(pub_param_path, args[1]);
  strcpy(pub_param_path + strlen(args[1]), "/key/abe.pub");
  pub_param_path[pub_param_path_len] = '\0';
  printf("pub_param_path: %s\n", pub_param_path);

  //path of ciphertext
  size_t ct_path_len = strlen(args[1]) + strlen("/tmp/ct.dat") + 1;
  char* ct_path = (char*)malloc(sizeof(char) * ct_path_len);
  strcpy(ct_path, args[1]);
  strcpy(ct_path + strlen(args[1]), "/tmp/ct.dat");
  ct_path[ct_path_len] = '\0';
  printf("ct_path: %s\n", ct_path);

  //path of aes encryption key
  size_t sek_path_len = strlen(args[1]) + strlen("/tmp/sek.dat") + 1;
  char* sek_path = (char*)malloc(sizeof(char) * sek_path_len);
  strcpy(sek_path, args[1]);
  strcpy(sek_path + strlen(args[1]), "/tmp/sek.dat");
  sek_path[sek_path_len] = '\0';
  printf("sek_path: %s\n", sek_path);



  pairing_t p;
  Utility::init_pairing(p, param_path);
  PubParam* pub = Utility::restore_pubParam(p, pub_param_path);
  printf("init public params ok\n"); 
  Policy* policy = init_policy(args[2]);

  //m's corresponding byte value is private key used by AES encryption
  element_t m;
  element_init_GT(m,p);
  element_random(m);

  //init Cipher
  CipherText ct(policy, &m, pub, &p);
  printf("init ciphertext ok!\n");

  //generate ct file
  ByteString bytes;
  int z = ct.toByteString(bytes);
  printf("ct bytes len %d\n", bytes.getLength());
  printf("ct to bytes ok %d\n", z);
  int len = bytes.getLength();
  unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * len);
  bytes.toBytes(buf);
  printf("byteString to bytes ok\n");
  FILE* f = fopen(ct_path, "wb");
  fwrite((void*)buf, 1, len, f);
  free(buf);
  fclose(f);
  printf("generate ciphertext (including access tree) ok\n");
  
  //generate file to store m
  len = element_length_in_bytes(m);
  buf = (unsigned char*)malloc(sizeof(unsigned char*) * len);
  element_to_bytes(buf, m);
  f = fopen(sek_path, "w");
  fwrite(buf, 1, len, f);
  fclose(f);
  free(buf);
  printf("CP-ABE encrypt success!\n");
  
  return 1;
}
