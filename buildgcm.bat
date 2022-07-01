C:/mingw64/bin/gcc -I"include" -c testgcm.c -o testgcm.o
C:/mingw64/bin/gcc -I"include" -c gcm.c -o libs/gcm.o
C:/mingw64/bin/gcc libs/basics.o libs/gcm.o libs/aes.o testgcm.o -o testgcm.exe