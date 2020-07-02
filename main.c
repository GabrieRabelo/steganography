#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <stdbool.h>    // Para usar boolean

// SOIL é a biblioteca para leitura das imagens
#include "include/SOIL.h"

// Um pixel RGB
typedef struct
{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct
{
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
void decToBin(int dec);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (RGB*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: %s (%s)\n", SOIL_last_result(), name);
        exit(1);
    }
    printf("Load: %s (%d x %d x %d)\n", name, pic->width, pic->height, chan);
}

void decToBin(int dec)
{
    if( dec > 4095)
    {
        printf("Valor invalido para conversao, o valor maximo eh 4095");
        exit(1);
    }
    int i = 0;
    int binValue[8];
    while (dec > 0)
    {

        binValue[i] = dec % 2;
        dec = dec / 2;
        i++;
    }

    static int binReverse[8];

    for(int j = i; j<8; j++)
    {
        binValue[j] = 0;
    }

    for(int j = 0; j<8; j++)
    {
        int k = 8-1-j;
        binReverse[j] = binValue[k];
    }

    printf("\n[");
    for(int j = 0; j<8; j++)
    {
        printf("%d", binReverse[j]);
    }
    printf("]\n");


}
int main(int argc, char** argv)
{
    Img base, secreta;
    if(argc < 2)
    {
        printf("loader [img base] [img secreta]\n");
        exit(1);
    }

    if (argc == 2)
    {
        printf("decode");

        Img saida;
        //start code
        load(argv[1], &base);
        int width;
        int height;

        width |= ((base.img[0].r) & 0b00000011);
        width = width << 2;

        width |= ((base.img[0].g) & 0b00000011);
        width = width << 2;

        width |= ((base.img[0].b) & 0b00000011);
        width = width << 2;

        width |= ((base.img[1].r) & 0b00000011);
        width = width << 2;

        width |= ((base.img[1].g) & 0b00000011);
        width = width << 2;

        width |= ((base.img[1].b) & 0b00000011);
        printf("WIDTH 6 %d\n", width);


        height |= ((base.img[2].r) & 0b00000011);
        height = height << 2;

        height |= ((base.img[2].g) & 0b00000011);
        height = height << 2;

        height |= ((base.img[2].b) & 0b00000011);
        height = height << 2;

        height |= ((base.img[3].r) & 0b00000011);
        height = height << 2;

        height |= ((base.img[3].g) & 0b00000011);
        height = height << 2;

        height |= ((base.img[3].b) & 0b00000011);
        printf("height 6 %d\n", height);

        printf("debug1");

        saida.width = width;
        saida.height = height;

        RGB* arr[width * height * 3];
        saida.img = arr;

        printf("debug\n");
        int y = 4;
        int i = 0;
        for (; i < (saida.width * saida.height); y += 4, i ++)
        {
            //r
            saida.img[i].r |= ((base.img[y].r) & 0b00000011);
            saida.img[i].r = saida.img[i].r << 2;

            saida.img[i].r |= ((base.img[y].g) & 0b00000011);
            saida.img[i].r = saida.img[i].r << 2;

            saida.img[i].r |= ((base.img[y].b) & 0b00000011);
            saida.img[i].r = saida.img[i].r << 2;

            saida.img[i].r |= ((base.img[y + 1].r) & 0b00000011);
            saida.img[i].r = saida.img[i].r << 2;

            //g
            saida.img[i].g |= ((base.img[y + 1].g) & 0b00000011);
            saida.img[i].g = saida.img[i].g << 2;

            saida.img[i].g |= ((base.img[y + 1].b) & 0b00000011);
            saida.img[i].g = saida.img[i].g << 2;

            saida.img[i].g |= ((base.img[y + 2].r) & 0b00000011);
            saida.img[i].g = saida.img[i].g << 2;

            saida.img[i].g |= ((base.img[y + 2].g) & 0b00000011);
            saida.img[i].g = saida.img[i].g << 2;

            //b
            saida.img[i].b |= ((base.img[y + 2].b) & 0b00000011);
            saida.img[i].b = saida.img[i].b << 2;

            saida.img[i].b |= ((base.img[y + 3].r) & 0b00000011);
            saida.img[i].b = saida.img[i].b << 2;

            saida.img[i].b |= ((base.img[y + 3].g) & 0b00000011);
            saida.img[i].b = saida.img[i].b << 2;

            saida.img[i].b |= ((base.img[y + 3].b) & 0b00000011);
            saida.img[i].b = saida.img[i].b << 2;
        }

        printf("%d\n", i);

        printf("Teste: gravando imagem base em unload.bmp\n");
        SOIL_save_image("result.bmp", SOIL_SAVE_TYPE_BMP,
        saida.width, saida.height, 3, (const unsigned char*) saida.img);

        free(saida.img);
        free(base.img);
        //end code
        exit(1);
    }

    load(argv[1], &base);
    load(argv[2], &secreta);
    //start codigo

    int areaBase = base.height * base.width;
    int areaSecreta = secreta.height * secreta.width;

    if(areaBase/4 < areaSecreta + 24)
    {
        printf("Imagem invalida, a imagem secreta deve ter no maximo 1/4 do tamanho da imagem base");
        exit(1);
    }

    for(int i = 0; i < areaBase; i++)
    {
        base.img[i].r &= 0b11111100;
        base.img[i].g &= 0b11111100;
        base.img[i].b &= 0b11111100;
    }

    printf("Primeiros 10 pixels da imagem base ZERADA:\n");
    for(int i=0; i<10; i++)
        printf("%02X,%02X,%02X", base.img[i].r, base.img[i].g, base.img[i].b);
    printf("\n\n");

    printf("Width: %d \n", base.width);
    printf("Height: %d \n", base.height);
    printf("Area: %d \n", base.width * base.height);
    int y = 0;
    int i = 0;
    int k = 0;

    base.img[i].r |= secreta.width >> 10;
    base.img[i].g |= (secreta.width >> 8) & 0b000000000011;
    base.img[i].b |= (secreta.width >> 6) & 0b000000000011;
    base.img[i + 1].r |= (secreta.width >> 4) & 0b000000000011;
    base.img[i + 1].g |= (secreta.width >> 2) & 0b000000000011;
    base.img[i + 1].b |= (secreta.width) & 0b000000000011;

    base.img[i + 2].r |= secreta.height >> 10;
    base.img[i + 2].g |= (secreta.height >> 8) & 0b000000000011;
    base.img[i + 2].b |= (secreta.height >> 6) & 0b000000000011;
    base.img[i + 3].r |= (secreta.height >> 4) & 0b000000000011;
    base.img[i + 3].g |= (secreta.height >> 2) & 0b000000000011;
    base.img[i + 3].b |= (secreta.height) & 0b000000000011;


    i = 4;

    for (; y < (secreta.width * secreta.height); i += 4)
    {
        base.img[i].r |= secreta.img[y].r >> 6;
        base.img[i].g |= ((secreta.img[y].r >> 4) & 0b00000011);
        base.img[i].b |= ((secreta.img[y].r >> 2) & 0b00000011);
        base.img[i + 1].r |= ((secreta.img[y].r) & 0b00000011);

        base.img[i + 1].g |= secreta.img[y].g >> 6;
        base.img[i + 1].b |= ((secreta.img[y].g >> 4) & 0b00000011);
        base.img[i + 2].r |= ((secreta.img[y].g >> 2) & 0b00000011);
        base.img[i + 2].g |= ((secreta.img[y].g) & 0b00000011);

        base.img[i + 2].b |= secreta.img[y].b >> 6;
        base.img[i + 3].r |= ((secreta.img[y].b >> 4) & 0b00000011);
        base.img[i + 3].g |= ((secreta.img[y].b >> 2) & 0b00000011);
        base.img[i + 3].b |= ((secreta.img[y].b) & 0b00000011);
        y++;
    }

    printf("largura %d, altura %d\n", base.width, base.height);

    //end codigo

    printf("Primeiros 10 pixels da imagem base DEPOIS:\n");
    for(int i=0; i<10; i++)
        printf("%02X %02X,%02X", base.img[i].r, base.img[i].g, base.img[i].b);
    printf("\n\n");

    for(int i = 2; i < 4; i ++)
    {
        decToBin(base.img[i].r);
        decToBin(base.img[i].g);
        decToBin(base.img[i].b);
    }

//    printf("secreta:\n");
//
//    for(int i = 0; i < 3; i ++) {
//        decToBin(secreta.img[i].r);
//        decToBin(secreta.img[i].g);
//        decToBin(secreta.img[i].b);
//    }

    printf("Teste: gravando imagem base em saida.bmp\n");
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP,
                    base.width, base.height, 3, (const unsigned char*) base.img);

    free(base.img);
    free(secreta.img);
}
