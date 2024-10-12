#include "converteutf832.h"

#define BIGBOM 0xFFFE0000
#define LITTLEBOM 0x0000FEFF

//função para verificar o primeiro bit e retornar quantos deverão ser lidos
int tamBytesUtf8(unsigned int byte){
    if ((byte & 0x80) == 0){ // 0xxx xxx & 1000 0000 == 0 
        return 1;
    } else if ((byte & 0xE0) == 0xC0){ // 110x xxxx & 1110 0000 == 1100 0000
        return 2;
    } else if ((byte & 0xF0) == 0xE0){ // 1110 xxxx & 1111 0000 == 1110 0000 
        return 3;
    } else if((byte & 0xF8) == 0xF0){ // 1111 0xxx & 1111 1000 == 1111 0000 
        return 4;
    }
    return -1; //não entrou em nenhum dos casos
}

unsigned int converteBitsUtf8(unsigned int carac, int tamBytes, FILE* arquivo_entrada){ //converte todos os bytes restantes que começam com 10xx xxxx
    unsigned int aux;

    for (int i = 1; i < tamBytes; i++){
        carac = carac << 6; 

        aux = fgetc(arquivo_entrada); 
        aux = aux & 0x3F; 

        carac = carac | aux; 
    }
    return carac;
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
    if (!arquivo_entrada){ //caso o arquivo de entrada seja NULL
        fprintf(stderr,"Ocorreu algum erro na abertura do arquivo de entrada para UTF-8\n");
        printf("Ocorreu um erro na abertura do arquivo de entrada em UTF-8\n");
        return -1;
    }

    if (!arquivo_saida){ //caso o arquivo de saída seja NULL
        fprintf(stderr,"Ocorreu algum erro na abertura do arquivo de saída para UTF-32\n");
        printf("Ocorreu um erro na abertura do arquivo de saída em UTF-32\n");
        return -1;
    }
    
    int tamBytes;
    unsigned int carac;
    unsigned int BOM = 0x0000FEFF; //caractere especial BOM para o início do arquivo em UTF-32

    fwrite(&BOM, sizeof(BOM), 1, arquivo_saida);

    while(!feof(arquivo_entrada)){ 
        carac = fgetc(arquivo_entrada); //lê o primeiro byte
        tamBytes = tamBytesUtf8(carac);

        if (tamBytes == 1){
            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (tamBytes == 2){ 
            carac = carac & 0x1F; //ao realizar a operação & com 0001 1111, pegamos os 5 últimos bits
            carac = converteBitsUtf8(carac, tamBytes, arquivo_entrada);

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (tamBytes == 3){
            carac = carac & 0xF; //ao realizar a operação & com 0000 1111, pegamos os 4 últimos bits
            carac = converteBitsUtf8(carac, tamBytes, arquivo_entrada);

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (tamBytes == 4){ 
            carac = carac & 0x7; //ao realizar a operação & com 0000 0111, pegamos os 3 últimos bits
            carac = converteBitsUtf8(carac, tamBytes, arquivo_entrada);

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);
        } 
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){
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


    while(fread(&carac,sizeof(int),1,arquivo_entrada)){ // o loop continua enquanto caracteres são lidos
        if (bom == BIGBOM){ // caso o arquivo esteja em big endian
            carac = ((carac>>24)&0xff) | // inverte os bytes para que as operações sejam as mesmas do little endian
                    ((carac<<8)&0xff0000) | 
                    ((carac>>8)&0xff00) | 
                    ((carac<<24)&0xff000000);
        }
        if (carac <= 0x007F){ // caso o caractere vá ter 1 byte
            auxC[0] = (char) carac;
            auxC[0] = auxC[0] & 0x7F; // pega os 7 bits menos significativos
            fwrite(auxC, sizeof(char), 1, arquivo_saida);
        }
        else if (carac <= 0x07FF){ // caso o caractere vá ter 2 bytes
            auxC[0] = (char) (carac >> 6); // pega os bits que serão utilizados no primeiro byte
            auxC[0] = auxC[0] | 0xC0; // coloca na formatação de UTF-8
            auxC[1] = (char) (carac & 0x3F); // pega os 6 bits menos significativos
            auxC[1] = auxC[1] | 0x80;
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
            fwrite(auxC, 4, 1, arquivo_saida);
        }
    }

    return 0;
}
