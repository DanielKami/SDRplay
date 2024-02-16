#pragma once

#include "mirisdr.h"

int Sdrplay_read_sync(sdrplay_dev_t* p, void* buf, int len, int* n_read);