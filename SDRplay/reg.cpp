#include "pch.h" 
//#include <cstdint>
#include "mirisdr.h"
#include "constants.h"
#include "reg.h"
#include "DLL.h"

extern Libusb libusb;

 
int mirisdr_write_reg (sdrplay_dev_t *p, uint8_t reg, uint32_t val) {
    uint16_t value = (val & 0xff) << 8 | reg;   //make negative of val in bites and shift it left for 8 bits (byte)
    uint16_t index = (val >> 8) & 0xffff; //only second (higher two  bytes). What left, shift right and negative
 

    if (!p) return -1;
    if (!p->dh) return -1;

#if MIRISDR_DEBUG >= 2
    fprintf( stderr, "write reg: 0x%02x, val 0x%08x\n", reg, val);
#endif

    int r = libusb.control_transfer(p->dh, CTRL_OUT, CMD_WREG, value, index, NULL, 0, CTRL_TIMEOUT);

	//uint16_t reg_r = miri_read_reg(p, reg, val);

    return r;
}


//Partly works
uint16_t miri_read_reg(sdrplay_dev_t* p, uint8_t reg, uint16_t val)
{
	int r;
	uint8_t data[4];
	uint8_t data2[256];


	uint16_t value = (val & 0xff) << 8 | reg;
	//uint16_t index = (val >> 8) & 0xffff;
	//uint16_t index = page;
	//uint16_t reg;

			//                                                    value
	r = libusb.control_transfer(p->dh, CTRL_IN, CMD_RREG, reg, 0, data, sizeof(data), CTRL_TIMEOUT);

#if MIRISDR_DEBUG >= 2
	if (r < 0)
	{
		fprintf(stderr, "%s failed with %d\n", __FUNCTION__, r);
	}
#endif

	uint16_t res1 = (data[1] << 8) | data[0];
	uint16_t res2 = (data[3] << 8) | data[2];

	return res1;
}