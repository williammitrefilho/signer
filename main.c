/* ========================================================================== */
/*                                                                            */
/*   Filename.c                                                               */
/*   (c) 2012 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc == 2){
        
        FILE *arq = fopen(argv[1], "rb");
        if(!arq){
            
            printf("erro\n");
            return 1;
        }
        printf("arquivo aberto");
        unsigned char ptype[4096];
        unsigned long k = 0;
        while(!feof(arq)){
            
            fread(ptype, 1, 4096, arq);
            k++;   
        }
        fclose(arq);
        printf("pagina:%d, tipo da pagina:%d\n", ++k, ptype[0]);
    }
    return 0;
}
