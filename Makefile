all: CryptoTest App

CXX = g++
CFLAGS = -g -wall

params.o: params.cpp params.h
	$(CXX) -c -o params.o params.cpp -lssl -lcrypto -lpthread

shamir.o: shamir.cpp shamir.h
	$(CXX) -c -o shamir.o shamir.cpp -lssl -lcrypto -lpthread

elgamal.o: elgamal.cpp elgamal.h
	$(CXX) -c -o elgamal.o elgamal.cpp -lssl -lcrypto -lpthread

CryptoTest: CryptoTest.cpp params.o shamir.o elgamal.o
	$(CXX) -o CryptoTest CryptoTest.cpp params.o shamir.o elgamal.o -lssl -lcrypto -lpthread

App: App.cpp params.o shamir.o elgamal.o
	$(CXX) -o App App.cpp params.o shamir.o elgamal.o -lssl -lcrypto -lpthread

clean:
	rm -f *.o
