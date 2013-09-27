#ifndef UTILITY_H_
#define UTILITY_H_

#include <pbc.h>
#include <errno.h>
#include <stdlib.h>
#include "PubParam.h"

class Utility{
  public:

    /**
    *store byte string to the file
    *@return if success, return len, else return -2 when can't open file, or error number returned by fwrite.
    */
    static size_t store(const char* filePath, unsigned char* buf, size_t len);

    /**
    *load $len bytes  to buf, from the file
    *@return if sucess return len, else return -2 when can't open file, or error number returned by fread.
    */
    static size_t load(const char* filePath, unsigned char* buf, size_t len);

    /**
    *get size of the file in the unit of 'unsigned char'
    */
    static size_t size(const char* filePath);
    
    /**
    *exactly as it seems to be
    */
    static void int2str(unsigned char* buf,  int v);

    /**
    *exactly as it seems to be
    */
    static int str2int(unsigned char* buf);

    /**
    * just for test and debug
    */
    static void print_char(const char* str, unsigned char* buf, int len);
    
    static int init_pairing(pairing_t& p, const char* filePath);
    static PubParam* restore_pubParam(pairing_t& p, const char* filePath);
};

#endif
