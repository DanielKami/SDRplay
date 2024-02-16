#pragma once

#ifndef __REG_H
#define __REG_H

#include "mirisdr.h"

#define CMD_RESET              0x40
#define CMD_WREG               0x41
#define CMD_RREG               0x42
#define CMD_START_STREAMING    0x43
#define CMD_DOWNLOAD           0x44
#define CMD_STOP_STREAMING     0x45
//WValue = Addr?
#define CMD_REEPROM            0x46
//WValue = Addr?
#define CMD_WEEPROM            0x47
#define CMD_READ_UNKNOWN       0x48
//wValue = gpio << 8 | val
#define CMD_WGPIO              0x49
#define CMD_EXT_WGPIO_BASE     0x4b

#define CTRL_TIMEOUT           1000
#define CTRL_IN		           (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT	           (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)



/*
RSP1
GPIO(0x13) & 0x01 = DSB_NOTCH
GPIO(0x13) & 0x04 = BROADCAST_NOTCH
*/

int mirisdr_write_reg(sdrplay_dev_t* p, uint8_t reg, uint32_t val);
uint16_t miri_read_reg(sdrplay_dev_t* p, uint8_t page, uint16_t addr);


#endif 