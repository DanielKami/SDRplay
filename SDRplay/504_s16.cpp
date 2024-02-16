
/*
 * 8 bitový formát
 * 1024 bajtů odpovídá 1008 hodnotám
 * struktura:
 *   16b hlavička
 *  1008b bloků obsahujících 1008 8b hodnot
 */
#include "pch.h" 
#include "504_s16.h"
#include <cstdio>

uint16_t  mirisdr_samples_convert_504_s16 (sdrplay_dev_t *p, uint8_t* buf, uint8_t *dst8, uint16_t cnt, uint32_t* lost) {
    uint16_t i, i_max, j, ret = 0;
    *lost = 0;
    uint32_t addr = 0;
    uint8_t *src = buf;
    int16_t *dst = (int16_t*) dst8;

    /* dostáváme 1-3 1024 bytů dlouhé bloky */
    for (i_max = cnt >> 10, i = 0; i < i_max; i++, src+= 1008) {
        /* pozice hlavičky */
        addr = src[3] << 24 | src[2] << 16 | src[1] << 8 | src[0] << 0;

#if MIRISDR_DEBUG >= 1
        /* potenciálně ztracená data */
        if ((i == 0) && (addr != p->addr)) {
            fprintf(stderr, "%u samples lost, %d, %08x:%08x\n", addr - p->addr, cnt, p->addr, addr);
        }
#endif
       *lost += addr - p->addr;
        /* přeskočíme hlavičku 16 bitů, 504 I+Q párů */
        for (src+= 16, j = 0; j < 1008; j+= 2, ret+= 2) {
            /* bitovým posunem zajistíme plný rozsah a zároveň správné znaménko */
            dst[ret + 0] = src[j + 0] << 8;
            dst[ret + 1] = src[j + 1] << 8;
        }
    }

    p->addr = addr + 504;

    /* total used bytes */
    return ret * 2;
}
