 
#include "pch.h" 
#include "adc.h"
#include "reg.h"
#include "mirisdr.h"

int Sdrplay_adc_init (sdrplay_dev_t *p) {
    if (!p) return -1;
    int r;
    /* inicializace - statická */
   r= mirisdr_write_reg(p, 0x08, 0x006080); /* kernel driver */
   if (r < 0) return r;
   r= mirisdr_write_reg(p, 0x05, 0x00000c);
   if (r < 0) return r;
   r= mirisdr_write_reg(p, 0x00, 0x000200);
   if (r < 0) return r;
   r= mirisdr_write_reg(p, 0x02, 0x004801);
   if (r < 0) return r;
   r= mirisdr_write_reg(p, 0x08, 0x00f380); /* kernel driver */

    return r;
}

int Sdrplay_adc_stop (sdrplay_dev_t *p) {
    if (!p) return -1;

    /* uspíme USB IF a ADC */
   return mirisdr_write_reg(p, 0x03, 0x010000);
}
