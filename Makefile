libcmp.so: CMPSocket.o UNIXError.o
	g++ -shared -fPIC -o libcmp.so build/CMPSocket.o build/UNIXError.o
CMPSocket.o: CMPSocket.cpp CMPSocket.h UNIXError.h
	g++ -c -fPIC -o build/CMPSocket.o CMPSocket.cpp
UNIXError.o: UNIXError.cpp
	g++ -c -fPIC -o build/UNIXError.o UNIXError.cpp
fork-parent.o: fork-parent/fork-parent.h
	gcc -c -o build/fork-parent.o fork-parent/fork-parent.c
cmpnameserver.o: cmpnameserver.cpp cmpnameserver-bits/cmpnameserver-common-bits.h cmpnameserver-bits/cmpnameserver-nserver.h fork-parent/fork-parent.h
	g++ -c -o build/cmpnameserver.o cmpnameserver.cpp
cmpnameserver: cmpnameserver.o libcmp.so fork-parent.o
	g++ -o cmpd cmpnameserver.o  fork-parent.o -L. -lcmp -Wl,-rpath,`pwd`
clean:
	rm -f *.o libcmp.so
test: libcmp.so
	g++ -g -o testcmp testcmp.cpp -L. -lcmp -Wl,-rpath,`pwd`