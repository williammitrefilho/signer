C:/mingw64/bin/gcc -I"include" -c testsha.c -o testsha.o
C:/mingw64/bin/gcc -I"include" -c sha.c -o libs/sha.o
C:/mingw64/bin/gcc libs/basics.o libs/sha.o testsha.o -o testsha.exe