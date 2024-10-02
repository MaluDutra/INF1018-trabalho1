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
    //se der erro -- emitir em stderr, printf de qual tipo de erro e retornar
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
    
    unsigned int carac;
    unsigned int aux;
    unsigned int BOM = 0x0000FEFF; //caractere especial BOM para o início do arquivo em UTF-32

    printf("%02x\n", BOM);
    dump(&BOM, sizeof(BOM));
    fwrite(&BOM, sizeof(BOM), 1, arquivo_saida);

    while(!feof(arquivo_entrada)){ //iniciando leitura do arquivo de entrada
        //verificar para cada fgetc se de fato foi lido algo?!!?!?!!?!?!?!?!?!?!?!?!??!
        carac = fgetc(arquivo_entrada); //lê o primeiro byte
        printf("utf-8 %02x\n", carac);

        if (carac <= 0x7F){ //primeiro caso: 1 byte (de 0x00 a 0x7F)
            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (carac <= 0xDF){ //segundo caso: 2 bytes (de 0x80 a 0x7FF)
            aux = carac;

            aux = aux & 0x1F; //ao realizar a operação & com 0001 1111, pegamos os 5 últimos bits
            aux = aux << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            printf("utf-8 %02x\n", carac);
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = aux | carac; //une as duas partes (char) em um só int

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (carac <= 0xEF){ //terceiro caso: 3 bytes (de 0x800 a oxFFFF)
            aux = carac;

            aux = aux & 0xF; //ao realizar a operação & com 0000 1111, pegamos os 4 últimos bits
            aux = aux << 12; //é realizado o shift à esquerda, para podermos depois inserir os 12 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            printf("utf-8 %02x\n", carac);
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            carac = carac << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = aux | carac; //une as duas partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o terceiro byte
            printf("utf-8 %02x\n", aux);
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = carac | aux; //une todas as partes (char) em um só int

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } else if (carac <= 0xF7){ //quarto caso: 4 bytes (de 0x10000 a 0x10FFFF)
            aux = carac;

            aux = aux & 0x7; //ao realizar a operação & com 0000 0111, pegamos os 3 últimos bits
            aux = aux << 18; //é realizado o shift à esquerda, para podermos depois inserir os 18 bits que faltam no final

            carac = fgetc(arquivo_entrada); //para ler o segundo byte
            printf("utf-8 %02x\n", carac);
            carac = carac & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            carac = carac << 12; //é realizado o shift à esquerda, para podermos depois inserir os 12 bits que faltam no final

            carac = aux | carac; //une as duas partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o terceiro byte
            printf("utf-8 %02x\n", aux);
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits
            aux = aux << 6; //é realizado o shift à esquerda, para podermos depois inserir os 6 bits que faltam no final

            carac = carac | aux; //une todas as três partes (char) em um só int

            aux = fgetc(arquivo_entrada); //para ler o quarto byte
            printf("utf-8 %02x\n", aux);
            aux = aux & 0x3F; //ao realizar a operação & com 0011 1111, pegamos os 6 últimos bits

            carac = carac | aux; //une todas as partes (char) em um só int

            fwrite(&carac, sizeof(carac), 1, arquivo_saida);

        } 
        
        printf("%02x\n", carac);
        dump(&carac, sizeof(carac));
    }

    //os arquivos precisam estar em .bin ou .txt??
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

    fread(&bom,sizeof(int),1,arquivo_entrada);
    if ((bom != LITTLEBOM) && (bom != BIGBOM)){
        fprintf(stderr,"Ocorreu um erro ao identificar o BOM\n");
        printf("Ocorreu um erro ao identificar o BOM\n");
        return -1;
    }


    if (bom == LITTLEBOM){ // caso o arquivo esteja em little endian
        printf("O arquivo está em little endian\n");
        while(fread(&carac,sizeof(int),1,arquivo_entrada)){ // o loop continua enquanto caracteres são lidos
            printf("utf-32 %08x\n", carac);
            if (carac <= 0x007F){ // caso o caractere tenha um byte
                auxC[0] = (char) carac;
                auxC[0] = auxC[0] & 0x7F;
                dump(auxC,1);
                fwrite(auxC, sizeof(char), 1, arquivo_saida);
            }
            else if (carac <= 0x07FF){
                auxC[0] = (char) (carac >> 6);
                auxC[0] = auxC[0] | 0xC0;
                auxC[1] = (char) (carac & 0x3F);
                auxC[1] = auxC[1] | 0x80;
                dump(auxC,2);
                fwrite(auxC, 2, 1, arquivo_saida);
            }
            else if (carac <= 0xFFFF){
                auxC[0] = (char) (carac >> 12);
                auxC[0] = auxC[0] | 0xE0;
                auxC[1] = (char) (carac >> 6);
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac & 0x3F);
                auxC[2] = auxC[2] | 0x80;
                dump(auxC,3);
                fwrite(auxC, 3, 1, arquivo_saida);
            }
            else if (carac <= 0x10FFFF){
                auxC[0] = (char) (carac >> 18);
                auxC[0] = auxC[0] | 0xF0;
                auxC[1] = (char) (carac >> 12);
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac >> 6);
                auxC[2] = auxC[2] & 0x3F;
                auxC[2] = auxC[2] | 0x80;
                auxC[3] = (char) (carac & 0x3F);
                auxC[3] = auxC[3] | 0x80;
                dump(auxC,4);
                fwrite(auxC, 4, 1, arquivo_saida);
            }
        }
    }
    else if (bom == BIGBOM){
        printf("big endian\n");
        while(fread(&carac,sizeof(int),1,arquivo_entrada)){
            carac = ((carac>>24)&0xff) | // move byte 3 to byte 0
                    ((carac<<8)&0xff0000) | // move byte 1 to byte 2
                    ((carac>>8)&0xff00) | // move byte 2 to byte 1
                    ((carac<<24)&0xff000000); // byte 0 to byte 3
            printf("utf-32 %08x\n", carac);
            if (carac <= 0x007F){
                auxC[0] = (char) carac;
                auxC[0] = auxC[0] & 0x7F;
                dump(auxC,1);
                fwrite(auxC, sizeof(char), 1, arquivo_saida);
            }
            else if (carac <= 0x07FF){
                auxC[0] = (char) (carac >> 6);
                auxC[0] = auxC[0] | 0xC0;
                auxC[1] = (char) (carac & 0x3F);
                auxC[1] = auxC[1] | 0x80;
                dump(auxC,2);
                fwrite(auxC, 2, 1, arquivo_saida);
            }
            else if (carac <= 0xFFFF){
                auxC[0] = (char) (carac >> 12);
                auxC[0] = auxC[0] | 0xE0;
                auxC[1] = (char) (carac >> 6);
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac & 0x3F);
                auxC[2] = auxC[2] | 0x80;
                dump(auxC,3);
                fwrite(auxC, 3, 1, arquivo_saida);
            }
            else if (carac <= 0x10FFFF){
                auxC[0] = (char) (carac >> 18);
                auxC[0] = auxC[0] | 0xF0;
                auxC[1] = (char) (carac >> 12);
                auxC[1] = auxC[1] & 0x3F;
                auxC[1] = auxC[1] | 0x80;
                auxC[2] = (char) (carac >> 6);
                auxC[2] = auxC[2] & 0x3F;
                auxC[2] = auxC[2] | 0x80;
                auxC[3] = (char) (carac & 0x3F);
                auxC[3] = auxC[3] | 0x80;
                dump(auxC,4);
                fwrite(auxC, 4, 1, arquivo_saida);
            }
        }
    }
    return 0;
}

