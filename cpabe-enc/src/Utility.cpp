#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t Utility::store(const char* filePath, unsigned char* buf, size_t len){
  FILE* f = fopen(filePath, "wb");
  if (f  == NULL){
//to do: return errno
//    printf("can't open file %s for writing\n", filePath);
    return -2;
  }

  unsigned char size_str[4];
  int2str(size_str, len);
  //write size info into this file
  fwrite(size_str, sizeof(unsigned char), 4, f);

  size_t size=fwrite(buf, sizeof(unsigned char), len, f);
  fclose(f);
  return size;
}


size_t Utility::load(const char* filePath, unsigned char* buf, size_t len){
  FILE* f = fopen(filePath, "rb");
  if(f == NULL){  //open file error
//to-do: change to return errno
 //   printf("can't open file %s\n", filePath);
    return -2;
   }
   fseek(f, 4 * sizeof(unsigned char), SEEK_CUR);
   size_t size = fread(buf, sizeof(unsigned char), len, f);
   fclose(f);
   return size;
}

size_t Utility::size(const char* filePath){
  FILE* f = fopen(filePath, "rb");
  if(f == NULL){
    return -2;
  }

  unsigned char buf[4];
  fread(buf, sizeof(unsigned char), 4, f);
  fclose(f);
  int len =0;
  len = str2int(buf);
  return len;
}

void Utility::int2str(unsigned char* buf, int v){
  int value = 0;
  if(v < 0){
    value = -v;
  } else {
    value = v;
  }

  int i = 0;
  for(i = 0; i<4; i++){
    buf[i] = (unsigned char) (value %256);
    value = value/256;
  }
  if(v < 0){
    buf[3] = (unsigned char)(buf[3] | 0x80);
  }
}

int Utility::str2int(unsigned char* buf){
  int flag = 1;
  if((buf[3] & 0x80) != 0){
    flag = -1;
    buf[3] = (unsigned char)(buf[3] & 0x7F);
  }
  int i = 0;
  int v = 0;
  int factor = 1;
  for(i = 0; i < 4; i++){
    v +=(int)buf[i] * factor;
    factor *= 256;
  }
  v *= flag;
  return v;
}

void Utility::print_char(const char* str, unsigned char* buf, int len){
  printf("%s", str);
  int i = 0;
  for(i = 0; i<len; i++){
    printf("%d ",(int)buf[i]);
  }
  printf("\n");
}


int Utility::init_pairing(pairing_t& p, const char* filePath){
  char param[1024];

  FILE* f = fopen(filePath, "rb");
  if ( f == NULL ){
    printf("///");
    return errno;
  }

  //read param file from param file
  size_t count = fread(param, 1, 1024, f);
  if (count == 0){
    pbc_die("read init param error");
  }
  fclose(f);
  //initial a pairing
  pairing_init_set_buf(p, param, count);
  return 0;
}

PubParam* Utility::restore_pubParam(pairing_t& p, const char* filePath){
  size_t len = size(filePath);
  if (len <= 0) {
    return NULL;
  }
  unsigned char* buf = (unsigned char*)malloc(sizeof(char) * len);
  load(filePath, buf, len);
  // impove it, remove the 'new' operator, causing it not neccessay to free memory outside of this file
  PubParam* pub = new PubParam(p, buf);
  free(buf);
  return pub;
}
