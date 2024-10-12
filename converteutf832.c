#include "converteutf832.h"

void dump(void *p, int n){
    unsigned char *p1 = p;
    while (n--){
        printf("%02x ", *p1);
        p1++;
    }
    putchar('\n');
}

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
        printf("utf-8 %02x\n", aux);
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
        fprintf(stderr,"Ocorreu algum erro na abertura do arquivo de saída para UTF-8\n");
        printf("Ocorreu um erro na abertura do arquivo de saída em UTF-8\n");
        return -1;
    }
    
    int tamBytes;
    unsigned int carac;
    unsigned int BOM = 0x0000FEFF; //caractere especial BOM para o início do arquivo em UTF-32

    printf("%02x\n", BOM);
    dump(&BOM, sizeof(BOM));
    fwrite(&BOM, sizeof(BOM), 1, arquivo_saida);

    while(!feof(arquivo_entrada)){ 
        carac = fgetc(arquivo_entrada); //lê o primeiro byte
        printf("utf-8 %02x\n", carac);
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
        
        printf("%02x\n", carac);
        dump(&carac, sizeof(carac));
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){
    return 0;
}