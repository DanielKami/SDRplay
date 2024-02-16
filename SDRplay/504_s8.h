//#pragma once

#include "mirisdr.h"
uint16_t  mirisdr_samples_convert_504_s8(sdrplay_dev_t* p, uint8_t* src, uint8_t* dst, uint16_t cnt, uint32_t* lost);
