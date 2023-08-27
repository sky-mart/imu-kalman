#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <stdint.h>

#define CONSOLE_INPUT_SIZE 128U

struct ConsoleInput
{
    char data[CONSOLE_INPUT_SIZE];
    uint8_t size;
    uint8_t ready;
    uint8_t cur;
};

#endif /* CONSOLE_INPUT_H */
