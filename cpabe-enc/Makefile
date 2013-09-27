cc=g++
HEADERS=-I D:/develop-env/dev/include
CFLAGS=-c $(HEADERS)
LIBSSL=D:/develop-env/dev/lib/libcrypto.a D:/develop-env/dev/lib/libssl.a
LIBPBC=D:/develop-env/dev/lib/libpbc.a
OBJFLAGS=-lpbc -lgmp

encrypt:encrypt.o  PubParam.o  Utility.o ByteString.o Leaf.o ExLeaf.o InternalNode.o Policy.o  CipherText.o
	$(cc) -o $@ encrypt.o  PubParam.o  Utility.o  ByteString.o Leaf.o InternalNode.o ExLeaf.o  Policy.o  CipherText.o  $(LIBSSL) $(LIBPBC) $(OBJFLAGS)

encrypt.o:encrypt.cpp
	$(cc) $(CFLAGS)  encrypt.cpp

CipherText.o: CipherText.cpp
	$(cc) $(CFLAGS) CipherText.cpp

PubParam.o:PubParam.cpp
	$(cc) $(CFLAGS) -o $@ PubParam.cpp
Leaf.o:Leaf.cpp
	$(cc) $(CFLAGS) -o $@ Leaf.cpp 
InternalNode.o: InternalNode.cpp
	$(cc) $(CFLAGS) -o $@ InternalNode.cpp
ExLeaf.o: ExLeaf.cpp
	$(cc) $(CFLAGS) -o $@ ExLeaf.cpp

Policy.o: Policy.cpp
	$(cc) $(CFLAGS) -o $@ Policy.cpp
Utility.o:Utility.cpp
	$(cc) $(CFLAGS) -o $@ Utility.cpp
ByteString.o: ByteString.cpp
	$(cc) $(CFLAGS) -o $@ ByteString.cpp
clean:
	rm *.o

