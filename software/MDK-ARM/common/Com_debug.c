#include "Com_debug.h"


// ÖØÓ³Éäº¯Êý => printf
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}
