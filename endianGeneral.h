#ifndef ENDIAN_GENERAL_H
#define ENDIAN_GENERAL_H

#include <cstdint>

static const uint8_t BYTE_SIZE = 8;
static const uint8_t WORD_SIZE = 16;

extern uint16_t lesthtons(uint16_t val);
extern uint32_t lesthtonl(uint32_t val);
extern uint64_t lesthtonll(uint64_t val);

#endif