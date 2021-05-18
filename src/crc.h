#ifndef CRC_H
#define CRC_H

#include <stdint.h>

uint16_t CRC16_Calc(uint8_t *src, uint16_t len, uint16_t crc);
uint32_t CRC32_Calc(uint8_t *buf, uint32_t len, uint32_t crc);

#endif // CRC_H
