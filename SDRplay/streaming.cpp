 
#include "pch.h" 
//#include <cstddef>
#include "constants.h"
#include "DLL.h"
#include "reg.h"

extern Libusb libusb;

int Sdrplay_streaming_start (sdrplay_dev_t *p) {
    if (!p) return -1;
    if (!p->dh) return -1;
    //                                    0x42
    return libusb.control_transfer(p->dh, CTRL_OUT, CMD_START_STREAMING, 0x0, 0x0, NULL, 0, CTRL_TIMEOUT);
}

int Sdrplay_streaming_stop (sdrplay_dev_t *p) {
    if (!p) return -1;
    if (!p->dh) return -1;

    return libusb.control_transfer(p->dh, CTRL_OUT, CMD_STOP_STREAMING, 0x0, 0x0, NULL, 0, CTRL_TIMEOUT);
}

