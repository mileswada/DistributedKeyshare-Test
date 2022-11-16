all: CryptoTest App VrfTest

CXX = g++
CFLAGS = -g -Wall -O0 -fpermissive

params.o: params.cpp params.h
	$(CXX) $(CFLAGS) -c -o params.o params.cpp -lssl -lcrypto -lpthread

ddh.o: ddh.cpp ddh.h
	$(CXX) $(CFLAGS) -c -o ddh.o ddh.cpp -lssl -lcrypto -lpthread
	
vrf.o: vrf.cpp vrf.h
	$(CXX) $(CFLAGS) -c -o vrf.o vrf.cpp -lssl -lcrypto -lpthread
	
shamir.o: shamir.cpp shamir.h
	$(CXX) $(CFLAGS) -c -o shamir.o shamir.cpp -lssl -lcrypto -lpthread

elgamal.o: elgamal.cpp elgamal.h
	$(CXX) $(CFLAGS) -c -o elgamal.o elgamal.cpp -lssl -lcrypto -lpthread

user.o: user.cpp
	$(CXX) $(CFLAGS) -c -o user.o user.cpp -lssl -lcrypto -lpthread

CryptoTest: CryptoTest.cpp params.o ddh.o vrf.o shamir.o elgamal.o user.o
	$(CXX) $(CFLAGS) -o CryptoTest CryptoTest.cpp params.o ddh.o vrf.o shamir.o elgamal.o user.o -lssl -lcrypto -lpthread

VrfTest: vrf_test.cpp params.o vrf.o ddh.o
	$(CXX) $(CFLAGS) -o VrfTest vrf_test.cpp params.o vrf.o ddh.o -lssl -lcrypto -lpthread

App: App.cpp params.o ddh.o vrf.o shamir.o elgamal.o
	$(CXX) $(CFLAGS) -o App App.cpp params.o ddh.o vrf.o shamir.o elgamal.o -lssl -lcrypto -lpthread

clean:
	rm -f *.o
	rm CryptoTest
	rm VrfTest
