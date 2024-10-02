#include "converteutf832.h"

int main(void){
    //testar conversão de UTF32 para UTF8
    FILE* arquivo_entradaUTF32 = fopen("utf32_peq.txt", "rb");
    FILE* arquivo_saidaUTF8 = fopen("utf8_conv.txt", "wb");

    //testar conversão de UTF8 para UTF32
    FILE* arquivo_entradaUTF8 = fopen("utf8_peq.txt", "rb");
    FILE* arquivo_saidaUTF32 = fopen("utf32_conv.txt", "wb");
    
    if (convUtf32p8(arquivo_entradaUTF32, arquivo_saidaUTF8) == 0){
        printf("Texto decodificado de UTF-32 para UTF-8 com sucesso!\n");
    }

    if (convUtf8p32(arquivo_entradaUTF8, arquivo_saidaUTF32) == 0){
        printf("Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entradaUTF32);
    fclose(arquivo_saidaUTF8);
    fclose(arquivo_entradaUTF8);
    fclose(arquivo_saidaUTF32);

    return 0;
}
