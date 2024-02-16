
#include "pch.h" 
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include "devices.h"
#include "DLL.h"
 
extern Libusb libusb;

//extern sdrplay_dev_t* devPointerp = NULL;
 
//Not success return -1
int Sdrplay_device_get (uint16_t vid, uint16_t pid) {
    int i;
 

    for (i = 0; i < sizeof(SDRplay_devices) / sizeof(SDRplayr_device_t); i++) 
    {
        if ((SDRplay_devices[i].vid == vid) && (SDRplay_devices[i].pid == pid)) 
            return i;
    }

    return -1;
}

//Return number of SDR devices
int32_t Sdrplay_get_device_count () 
{
    __int64 i, i_max;
    int32_t ret = 0;
    libusb_context *ctx = NULL;
    libusb_device **list;
    struct libusb_device_descriptor dd;
    int  r;

    if(ctx==NULL)
        r =  libusb.init(&ctx);
    if (!ctx)
        return r;

    i_max = libusb.get_device_list(ctx, &list);

    for (i = 0; i < i_max; i++) {
        libusb.get_device_descriptor(list[i], &dd);

        if (Sdrplay_device_get(dd.idVendor, dd.idProduct) > -1)
        {
            ret++;
        }
    }

    libusb.free_device_list(list, 1);

    libusb.libexit(ctx);

    return ret;

}

//Not success return empty string char
const int Sdrplay_get_device_name (uint32_t index,  char* text) {
    long long i, i_max;
    unsigned __int64 j = 0;
    libusb_context *ctx;
    libusb_device **list;
    struct libusb_device_descriptor dd;
    //SDRplayr_device_t *device = NULL;
    int device;

    libusb.init(&ctx);
    i_max = libusb.get_device_list(ctx, &list);

    for (i = 0; i < i_max; i++) {
        libusb.get_device_descriptor(list[i], &dd);

        if ((device = Sdrplay_device_get(dd.idVendor, dd.idProduct)) > -1 &&
            (j++ == index)) {
            libusb.free_device_list(list, 1);
            libusb.libexit(ctx);
            strcpy_s(text, 256, SDRplay_devices[device].name);
            
            return 0;
        }
    }

    libusb.free_device_list(list, 1);
    libusb.libexit(ctx);

    return -1;
}

//Not success return -1
int Sdrplay_get_device_usb_strings(uint32_t index, char *manufact, char *product, char *serial)
{
    ssize_t i, i_max;
    size_t j = 0;
    libusb_context *ctx;
    libusb_device **list;
    struct libusb_device_descriptor dd;
    int device;

    libusb.init(&ctx);
    if (!ctx)
        return -1;

    i_max = libusb.get_device_list(ctx, &list);

    for (i = 0; i < i_max; i++) {
        libusb.get_device_descriptor(list[i], &dd);

        if ((device = Sdrplay_device_get(dd.idVendor, dd.idProduct)) > -1 &&
            (j++ == index)) {
            strcpy_s(manufact, 256, SDRplay_devices[device].manufacturer);
            strcpy_s(product, 256, SDRplay_devices[device].product);
            sprintf_s(serial, 256, "%08u", index + 1);
            libusb.free_device_list(list, 1);
            libusb.libexit(ctx);
            return 0;
        }
    }

    memset(manufact, 0, 256);
    memset(product, 0, 256);
    memset(serial, 0, 256);

    libusb.free_device_list(list, 1);
    libusb.libexit(ctx);

    return -1;
}
