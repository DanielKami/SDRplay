
//#pragma once

#include "mirisdr.h"

uint16_t  mirisdr_samples_convert_252_s16(sdrplay_dev_t* p, uint8_t* buf, uint8_t* dst8, uint16_t cnt, uint32_t* lost);