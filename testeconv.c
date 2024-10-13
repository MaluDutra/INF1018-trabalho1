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

    arquivo_entrada = fopen("arquivo_utf8.txt", "rb");
    arquivo_saida = fopen("aarquivo_utf32_conv.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("2 - Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("arquivo_utf8_complex.txt", "rb");
    arquivo_saida = fopen("arquivo_utf32_le_bom_complex_conv.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("3 - Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("utf8_grande_verdade.txt", "rb");
    arquivo_saida = fopen("utf8_grande_verdade_conv.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("4 - Texto decodificado de UTF-8 grande para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("utf32_grande_big_verdade.txt", "rb");
    arquivo_saida = fopen("utf32_grande_big_conv.txt", "wb");
    
    if (convUtf32p8(arquivo_entrada, arquivo_saida) == 0){
        printf("5 - Texto decodificado de UTF-32 big grande para UTF-8 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("utf32_grande_little_verdade.txt", "rb");
    arquivo_saida = fopen("utf32_grande_little_conv.txt", "wb");
    
    if (convUtf32p8(arquivo_entrada, arquivo_saida) == 0){
        printf("6 - Texto decodificado de UTF-32 little grande para UTF-8 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    printf("Conversão de UTF-32 para UTF-8 para UTF-32 de volta\n");

    arquivo_entrada = fopen("utf32_peq.txt", "rb");
    arquivo_saida = fopen("utf32_p_utf8_peq1.txt", "wb");
    
    if (convUtf32p8(arquivo_entrada, arquivo_saida) == 0){
        printf("Texto decodificado de UTF-32 para UTF-8 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    arquivo_entrada = fopen("utf32_p_utf8_peq1.txt", "rb");
    arquivo_saida = fopen("utf8_p_utf32_peq1.txt", "wb");
    
    if (convUtf8p32(arquivo_entrada, arquivo_saida) == 0){
        printf("Texto decodificado de UTF-8 para UTF-32 com sucesso!\n");
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);


    return 0;
}