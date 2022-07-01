C:/mingw64/bin/gcc -I"include" -c testprf.c -o testprf.o
C:/mingw64/bin/gcc -I"include" -c sha.c -o libs/sha.o
C:/mingw64/bin/gcc libs/basics.o libs/sha.o testprf.o -o testprf.exe