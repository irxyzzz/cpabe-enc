#ifndef OBJECT_H_
#define OBJECT_H_
#include <stdlib.h>
/*
*the root of classes that must support serializing and deserializing.
*/
class Object{

  /*return the length needed to represent this object in bytes */
  public:
    virtual size_t getSize()=0;

  /*
  *return the byte represention of the Object
  *make sure the size of buf is not less than getSize();
  */
    virtual size_t toBytes(unsigned char* buf)=0;

};
#endif
