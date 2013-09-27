#ifndef NODE_H_
#define NODE_H_

#include "ByteString.h"
#include <stdlib.h>

typedef enum NodeType0{
  INTERNAL_NODE,
  LEAF,
  EXTENDED_LEAF
} NodeType;
  

class Node{
  private:
    Node** sons;
    int sonNum;
    int sonSize;
  public:
    /**
     *convert the object of Node into byte string
    */
    Node(){
      this->sonNum = 0;
      this->sons = NULL;
      this->sonSize = 0;
    }


    virtual int toByteString(ByteString& bytes) = 0;

    /**
    * return the node type.
    */
    virtual NodeType getType() = 0;

    /**
    *@deprecate. ***don't use it, use addSon instead. I'll remove it ***
    */
    int addSons(Node** sons, int num){
      this->sonNum = num;
      this->sons = (Node**)malloc(sizeof(Node*) * num);
      int i = 0;
      for(i = 0; i < num; i++){
        this->sons[i] = sons[i];
      }
      return num;
    }

   int addSon(Node* son){
     ensureCapacity();
     this->sons[this->sonNum++] = son;
     return 1;
   }

   virtual ~Node(){
     if(this->sons != NULL){
       free(this->sons);
     }
   }

   Node** getSons(){
     return this->sons;
   }

   int getSonNum(){
     return this->sonNum;
   }

  private:
    void  ensureCapacity(){
      if(this->sonSize == 0){
        this->sons = (Node**)malloc(sizeof(Node*) * 4);
        this->sonSize = 4;
      } else if (this->sonSize <= this->sonNum){
        this->sonSize += 4;
        this->sons = (Node**)realloc((void*)(this->sons), sizeof(Node*) * (this->sonSize));
      }
    }
};
#endif
