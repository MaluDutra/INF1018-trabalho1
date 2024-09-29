#include "converteutf832.h"

void dump(void *p, int n){
    unsigned char *p1 = p;
    while (n--){
        printf("%02x ", *p1);
        p1++;
    }
    putchar('\n');
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
    //se der erro -- emitir em stderr, printf de qual tipo de erro e retornar
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
    
    unsigned int carac;
    unsigned int aux;
    unsigned int BOM = 65279;

    printf("%02x\n", BOM);
    dump(&BOM, sizeof(BOM));

    while(!feof(arquivo_entrada)){ //iniciando leitura do arquivo de entrada
        carac = fgetc(arquivo_entrada); //lê o primeiro byte

        if (carac <= 0x7F){ //primeiro caso: 1 byte (de 0x00 a 0x7F)
            carac = carac;
        } else if (carac <= 0xDF){ //segundo caso: 2 bytes (de 0x80 a 0x7FF)
            aux = carac;

            aux = aux & 0x1F; //ao realizar a operação & com 0001 1111, pegamos os 5 últimos bits
            aux = aux << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = carac | aux; //une as duas partes (char) em um só int

        } else if (carac <= 0xEF){ //terceiro caso: 3 bytes (de 0x800 a oxFFFF)
            aux = carac;

            aux = aux & 0xF; //ao realizar a operação & com 0000 1111, pegamos os 4 últimos bits
            aux = aux << 12; //é realizado o shift à esquerda, para podermos depois inserir os 12 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            carac = carac << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = carac | aux; //une as duas partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o terceiro byte
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = carac | aux; //une todas as partes (char) em um só int

        } else if (carac <= 0xF7){ //quarto caso: 4 bytes (de 0x10000 a 0x10FFFF)
            aux = carac;

            aux = aux & 0x7; //ao realizar a operação & com 0000 0111, pegamos os 3 últimos bits
            aux = aux << 18; //é realizado o shift à esquerda, para podermos depois inserir os 18 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            carac = carac << 12; //é realizado o shift à esquerda, para podermos depois inserir os 12 bits que faltam no final

            carac = carac | aux; //une as duas partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o terceiro byte
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            aux = aux << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = carac | aux; //une todas as três partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o quarto byte
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = carac | aux; //une todas as partes (char) em um só int
        } 
        
        printf("%02x\n", carac);
        dump(&carac, sizeof(carac));
    }


    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){
    return 0;
}

