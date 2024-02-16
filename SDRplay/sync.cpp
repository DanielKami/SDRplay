#include "pch.h" 
#include "mirisdr.h"
#include "async.h"
#include "DLL.h"
#include "252_s16.h"
#include "336_s16.h"
#include "384_s16.h"
#include "504_s8.h"
#include "504_s16.h"
#include "reg.h"
extern Libusb libusb;

uint8_t* bufIn;
uint16_t InternalBuffLenght;

//This function return the buffer size of the data after converison. The converters works great but it is imposible to change the
//size in oposite way (alweys it is to much or not enought place in buffer).
//The len in Sdrplay_read_sync must have the size calculated for particular compresion!
//Everytime the format is changed the new arrays size has to be recalculated and applied
uint16_t Sdrplay_BufforSize(sdrplay_dev_t* p, uint16_t _InternalBuffLenght = DEFAULT_BULK_BUFFER)
{
    uint16_t lenOut = 0;
    InternalBuffLenght = _InternalBuffLenght;

    switch (p->format)
    {
    case format_t::MIRISDR_FORMAT_252_S16:
        lenOut = (InternalBuffLenght / 1024) * 1008;
        break;

    case format_t::MIRISDR_FORMAT_336_S16:
        lenOut = (InternalBuffLenght / 1024) * 1344;
        break;

    case format_t::MIRISDR_FORMAT_384_S16:
        lenOut = (InternalBuffLenght / 1024) * 1536;
        break;

    case format_t::MIRISDR_FORMAT_504_S16:
        lenOut = (InternalBuffLenght / 1024) * 2016;
        break;

    case format_t::MIRISDR_FORMAT_504_S8:
        lenOut = (InternalBuffLenght / 1024) * 1008;
        break;
    }

    bufIn = new uint8_t[InternalBuffLenght];

    return lenOut;
}

/* pouze pro bulk transfer a je třeba doplnit konverze formátů */
int Sdrplay_read_sync(sdrplay_dev_t* p, void* bufOut, uint16_t len, uint16_t* n_read, uint32_t* lost) {
    if (!p) return -101;

    int r ;
    uint16_t read;
 
    format_t format;
 
    r = libusb.bulk_transfer(p->dh, 0x81, bufIn, InternalBuffLenght, n_read, CTRL_TIMEOUT);
    if (r < 0)
        return r+1000;

    //convert
    switch (p->format)
    {
    case format_t::MIRISDR_FORMAT_252_S16:
        read = mirisdr_samples_convert_252_s16(p, bufIn, (uint8_t*)bufOut, InternalBuffLenght, lost);
        break;

    case format_t::MIRISDR_FORMAT_336_S16:
        read = mirisdr_samples_convert_336_s16(p, bufIn, (uint8_t*)bufOut, InternalBuffLenght, lost);
        break;

    case format_t::MIRISDR_FORMAT_384_S16:
        read = mirisdr_samples_convert_384_s16(p, bufIn, (uint8_t*)bufOut, InternalBuffLenght, lost);
        break;

    case format_t::MIRISDR_FORMAT_504_S16:
        read = mirisdr_samples_convert_504_s16(p, bufIn, (uint8_t*)bufOut, InternalBuffLenght, lost);
        break;

    case format_t::MIRISDR_FORMAT_504_S8:
        read = mirisdr_samples_convert_504_s8(p, bufIn, (uint8_t*)bufOut, InternalBuffLenght, lost);
        break;
    }

 

    if (read > 0)
        return read;
    return r;
}
