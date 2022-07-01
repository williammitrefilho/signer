C:/mingw64/bin/gcc -I"include" -c tlsbase.c -o libs/tlsbase.o
C:/mingw64/bin/gcc -I"include" -c bm256.c -o libs/bm256.o
C:/mingw64/bin/gcc -I"include" -c x25519.c -o libs/x25519.o
C:/mingw64/bin/gcc -I"include" -c converters.c -o libs/converters.o
C:/mingw64/bin/gcc -I"include" -c p256.c -o libs/p256.o
C:/mingw64/bin/gcc -I"include" -c t25519.c -o t25519.o
C:/mingw64/bin/gcc libs/bm256.o libs/x25519.o libs/converters.o libs/tlsbase.o libs/basics.o libs/p256.o libs/sha.o libs/aes.o libs/cbc.o libs/ber_entity.o t25519.o -o t25519.exe -lws2_32