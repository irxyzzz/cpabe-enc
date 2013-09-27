#include "ByteString.h"
#include <stdlib.h>
#include <string.h>


static const int INIT_SIZE = 20;

ByteString::ByteString(int size){
  this->init(size);
}

ByteString::ByteString(){
  this->init(INIT_SIZE);
}

void ByteString::init(int size){
  this->size = size;
  this->content = (unsigned char*) malloc (sizeof(unsigned char) * (this->size));
  this->length = 0;
}

int ByteString::append(unsigned char* buf, int len){
  this->ensureCapacity(len);
  memcpy((void*)(this->content + this->length),(void*) buf, len);
  this->length += len;
  return len;
}
 
int ByteString::append(ByteString& bytes){
  unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * bytes.getLength());
  int len = bytes.toBytes(buf);
  this->append(buf, len);
  free(buf);
  return len;
}

void ByteString::clear(){
  this->length = 0;
}

  
 
int ByteString::subString(unsigned char* buf, int begin, int len){
  if (begin >= this->length || len <= 0){
    return -1;
  }
  if (begin + len > this->length) {
    len = this->length - begin;
  }
  memcpy((void*)buf, (void*)(this->content + begin), len);
  return len;
}

int ByteString::getLength(){
  return this->length;
}

int ByteString::toBytes(unsigned char* buf){
  memcpy(buf, this->content, this->length);
  return this->length;
}

void ByteString::ensureCapacity(int len){
  while ( this->size < this->length + len) {
    this->size *= 2;
    this->content = (unsigned char*)realloc(this->content, this->size);
  }
}

ByteString::~ByteString(){
  free(this->content);
}
