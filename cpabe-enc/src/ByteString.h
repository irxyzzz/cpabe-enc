#ifndef BYTESTRING_H_
#define BYTESTRING_H_

class ByteString{
  private:
    int size;
    int length;
    unsigned char* content;
  public:
    ByteString(int size);
    ByteString();
    int append(unsigned char* buf, int len);
    int append(ByteString& bytes);
    void clear();
    int subString(unsigned char* buf, int begin, int len);
    int getLength();
    int toBytes(unsigned char* buf);
    ~ByteString();
  private:
    void ensureCapacity(int len);
    void init(int size);
};
#endif
