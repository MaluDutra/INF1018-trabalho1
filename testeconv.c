#include "converteutf832.h"

int main(void){
    //testar conversão de UTF8 para UTF32
    FILE* arquivo_entrada = fopen("utf8_peq.txt", "rb");
    FILE* arquivo_saida = fopen("utf32_conv.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("1 - Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("utf8_grande.txt", "rb");
    arquivo_saida = fopen("utf32_conv_grande.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("2 - Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    return 0;
}