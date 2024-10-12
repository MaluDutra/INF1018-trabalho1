#include "converteutf832.h"

#define BIGBOM 0xFFFE0000
#define LITTLEBOM 0x0000FEFF

void dump(void *p, int n){
    unsigned char *p1 = p;
    while (n--){
        printf("%02x ", *p1);
        p1++;
    }
    putchar('\n');
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){
    //se der erro -- emitir em stderr, printf de qual tipo de erro e retornar
    if (!arquivo_entrada){ //caso o arquivo de entrada seja NULL
        fprintf(stderr,"Ocorreu algum erro na abertura do arquivo de entrada para UTF-32\n");
        printf("Ocorreu um erro na abertura do arquivo de entrada em UTF-32\n");
        return -1;
    }

    if (!arquivo_saida){ //caso o arquivo de saída seja NULL
        fprintf(stderr,"Ocorreu algum erro na abertura do arquivo de saída para UTF-8\n");
        printf("Ocorreu um erro na abertura do arquivo de saída em UTF-8\n");
        return -1;
    }

    unsigned int carac; 
    unsigned int bom; 
    char auxC[4]; // variável para manipular os bits

    fread(&bom,sizeof(int),1,arquivo_entrada); // lê o primeiro caractere, equivalente ao BOM
    if ((bom != LITTLEBOM) && (bom != BIGBOM)){ // erro caso o BOM não seja reconhecido
        fprintf(stderr,"Ocorreu um erro ao identificar o BOM\n");
        printf("Ocorreu um erro ao identificar o BOM\n");
        return -1;
    }


    if (bom == LITTLEBOM){ // caso o arquivo esteja em little endian
        printf("O arquivo está em little endian\n");
        while(fread(&carac,sizeof(int),1,arquivo_entrada)){ // o loop continua enquanto caracteres são lidos
            printf("utf-32 %08x\n", carac);
            if (carac <= 0x007F){ // caso o caractere vá ter 1 byte
                auxC[0] = (char) carac;
                auxC[0] = auxC[0] & 0x7F; // pega os 7 bits menos significativos
                dump(auxC,1);
                fwrite(auxC, sizeof(char), 1, arquivo_saida);
            }
            else if (carac <= 0x07FF){ // caso o caractere vá ter 2 bytes
                auxC[0] = (char) (carac >> 6); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xC0; // coloca na formatação de UTF-8
                auxC[1] = (char) (carac & 0x3F); // pega os 6 bits menos significativos
                auxC[1] = auxC[1] | 0x80;
                dump(auxC,2);
                fwrite(auxC, 2, 1, arquivo_saida);
            }
            else if (carac <= 0xFFFF){ // caso o caractere vá ter 3 bytes
                auxC[0] = (char) (carac >> 12); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xE0;
                auxC[1] = (char) (carac >> 6); // pega os bits que serão utilizados no segundo byte
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac & 0x3F); // pega os bits que serão utilizados no terceito byte
                auxC[2] = auxC[2] | 0x80;
                dump(auxC,3);
                fwrite(auxC, 3, 1, arquivo_saida);
            }
            else if (carac <= 0x10FFFF){ // caso o caractere vá ter 4 bytes
                auxC[0] = (char) (carac >> 18); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xF0;
                auxC[1] = (char) (carac >> 12); // pega os bits que serão utilizados no segundo byte
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac >> 6); // pega os bits que serão utilizados no terceiro byte
                auxC[2] = auxC[2] & 0x3F;
                auxC[2] = auxC[2] | 0x80;
                auxC[3] = (char) (carac & 0x3F); // pega os bits que serão utilizados no quarto byte
                auxC[3] = auxC[3] | 0x80;
                dump(auxC,4);
                fwrite(auxC, 4, 1, arquivo_saida);
            }
        }
    }
    else if (bom == BIGBOM){ // caso o arquivo esteja em big endian
        printf("big endian\n");
        while(fread(&carac,sizeof(int),1,arquivo_entrada)){
            carac = ((carac>>24)&0xff) | // inverte os bytes para que as operações sejam as mesmas do little endian
                    ((carac<<8)&0xff0000) | 
                    ((carac>>8)&0xff00) | 
                    ((carac<<24)&0xff000000);
            printf("utf-32 %08x\n", carac);
            if (carac <= 0x007F){ // caso o caractere vá ter 1 byte
                auxC[0] = (char) carac;
                auxC[0] = auxC[0] & 0x7F; // pega os 7 bits menos significativos
                dump(auxC,1);
                fwrite(auxC, sizeof(char), 1, arquivo_saida);
            }
            else if (carac <= 0x07FF){ // caso o caractere vá ter 2 bytes
                auxC[0] = (char) (carac >> 6); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xC0; // coloca na formatação de UTF-8
                auxC[1] = (char) (carac & 0x3F); // pega os 6 bits menos significativos
                auxC[1] = auxC[1] | 0x80;
                dump(auxC,2);
                fwrite(auxC, 2, 1, arquivo_saida);
            }
            else if (carac <= 0xFFFF){ // caso o caractere vá ter 3 bytes
                auxC[0] = (char) (carac >> 12); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xE0;
                auxC[1] = (char) (carac >> 6); // pega os bits que serão utilizados no segundo byte
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac & 0x3F); // pega os bits que serão utilizados no terceito byte
                auxC[2] = auxC[2] | 0x80;
                dump(auxC,3);
                fwrite(auxC, 3, 1, arquivo_saida);
            }
            else if (carac <= 0x10FFFF){ // caso o caractere vá ter 4 bytes
                auxC[0] = (char) (carac >> 18); // pega os bits que serão utilizados no primeiro byte
                auxC[0] = auxC[0] | 0xF0;
                auxC[1] = (char) (carac >> 12); // pega os bits que serão utilizados no segundo byte
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac >> 6); // pega os bits que serão utilizados no terceiro byte
                auxC[2] = auxC[2] & 0x3F;
                auxC[2] = auxC[2] | 0x80;
                auxC[3] = (char) (carac & 0x3F); // pega os bits que serão utilizados no quarto byte
                auxC[3] = auxC[3] | 0x80;
                dump(auxC,4);
                fwrite(auxC, 4, 1, arquivo_saida);
            }
        }
    }
    return 0;
}

