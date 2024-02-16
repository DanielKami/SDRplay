 

#include "pch.h"
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mirisdr.h"
#include "constants.h"

#include "devices.h"
#include "DLL.h" 
#include "hard.h"
#include "soft.h"

extern Libusb libusb;

HRESULT Sdrplay_libUSBInit()
{
    return libusb.Init();
}

 

int Sdrplay_open (sdrplay_dev_t **p, uint32_t index) {
    sdrplay_dev_t *dev = NULL;
    libusb_device **list, *device = NULL;
    struct libusb_device_descriptor dd;
    ssize_t i, i_max;
    size_t count = 0;
    int r;

    *p = NULL;

   // if (!(dev = (sdrplay_dev_t*)malloc(sizeof(sdrplay_dev_t)))) return -ENOMEM;
    dev = new sdrplay_dev_t();

    memset(dev, 0, sizeof(sdrplay_dev_t));

    /* ostatní parametry */
    dev->index = index;
    dev->ctx = NULL;
    dev->freq_correction = 0;
 

    libusb.init(&dev->ctx);

    i_max = libusb.get_device_list(dev->ctx, &list);

    for (i = 0; i < i_max; i++) {
        libusb.get_device_descriptor(list[i], &dd);

        if ((Sdrplay_device_get(dd.idVendor, dd.idProduct)) > -1 &&
            (count++ == index)) {
            device = list[i];
            break;
        }
    }

    /* nenašli jsme zařízení */
    if (!device) {
        libusb.free_device_list(list, 1);
        fprintf( stderr, "no miri device %u found\n", dev->index);
        return -1001;
    }

    //r = libusb.set_configuration(dev->dh, 0);
 
    libusb_device_descriptor* desc = new libusb_device_descriptor();
    r = libusb.get_device_descriptor(device, desc);
  
 
    /* otevření zařízení */
    if ((r = libusb.open(device, &dev->dh)) < 0) {
        libusb.free_device_list(list, 1);
        fprintf( stderr, "failed to open miri usb device %u with code %d\n", index, r);
        return -1002;
    }
 
    libusb.free_device_list(list, 1);
    //////////////////////////////////////////
  
 
    /* reset je potřeba, jinak občas zařízení odmítá komunikovat */
    r = Sdrplay_reset_buffer (dev);

    /* ještě je třeba vždy ukončit i streamování, které může být při otevření aktivní */
    r = Sdrplay_streaming_stop(dev);
    if ( r  < 0)
    {
        fprintf(stderr, "can't stop streaming %d\n", r);
    }
    r = Sdrplay_adc_stop(dev);
    if (r < 0)
    {
        fprintf(stderr, "can't stop adc %d\n", r);
    }
  
    if ((r = libusb.kernel_driver_active(dev->dh, 0)) == 1) 
    {
        dev->driver_active = 1;

#ifdef DETACH_KERNEL_DRIVER
        if (!libusb.detach_kernel_driver(dev->dh, 0)) {
            fprintf(stderr, "Detached kernel driver\n");
        } else {
            fprintf(stderr, "Detaching kernel driver failed!");
            dev->driver_active = 0;
            return -1003;
        }
#else
        fprintf(stderr, "\nKernel driver is active, or device is "
                "claimed by second instance of libmirisdr."
                "\nIn the first case, please either detach"
                " or blacklist the kernel module\n"
                "(msi001 and msi2500), or enable automatic"
                " detaching at compile time.\n\n");
#endif
    } else 
    {
        //Just set it to 0
        dev->driver_active = 0;
    }

    //Set interface, success return 0
    if ((r = libusb.claim_interface(dev->dh, 0)) < 0) {
        fprintf( stderr, "failed to claim miri usb device %u with code %d\n", dev->index, r);
        if (dev) {
            if (dev->dh) {
                libusb.release_interface(dev->dh, 0);
                libusb.close(dev->dh);
            }
            if (dev->ctx) libusb.libexit(dev->ctx);
            delete(dev);
        }
        return -1005;
    }


    //Set alternative interface to 3 because this is the bulk transfer
    r = libusb.set_interface_alt_setting(dev->dh, 0, 3);
    if (r < 0)
    {
        if (dev)
        {
            if (dev->dh)
            {
                libusb.release_interface(dev->dh, 0);
                libusb.close(dev->dh);
            }
            if (dev->ctx) libusb.libexit(dev->ctx);
            delete(dev);
        }
        return -1006;
    }


    /* inicializace tuneru */
    dev->freq = DEFAULT_FREQ;
    dev->rate = DEFAULT_RATE;
    dev->gain = DEFAULT_GAIN;
    dev->band = MIRISDR_BAND_VHF; // matches always the default frequency of 90 MHz

    dev->gain_reduction_lna = 0;
    dev->gain_reduction_mixer = 0;
    dev->gain_reduction_baseband = 43;
    dev->if_freq = if_freq_t::MIRISDR_IF_ZERO;
    dev->format  = format_t::MIRISDR_FORMAT_AUTO_ON;
    dev->bandwidth = bandwidth_t::MIRISDR_BW_8MHZ;
    dev->xtal = xtal_t::MIRISDR_XTAL_24M;
    dev->hw_flavour = MIRISDR_HW_DEFAULT;
    dev->bias = 0;

    /* ISOC is more stable but works only on Unix systems */
#if !defined (_WIN32) || defined(__MINGW32__)
    dev->transfer = MIRISDR_TRANSFER_ISOC;
#else
    dev->transfer = transfer_t::MIRISDR_TRANSFER_BULK;
#endif

    r = Sdrplay_adc_init(dev);
    if (r < 0)
    {
        fprintf(stderr, "can't init adc %d\n", index, r);
        return r;
    }
    r = mirisdr_set_hard(dev);
    if (r < 0)
    {
        fprintf(stderr, "can't iset hard %d\n", index, r);
        return r;
    }
    r = mirisdr_set_soft(dev);
    if (r < 0)
    {
        fprintf(stderr, "can't set soft %d\n", index, r);
        return r;
    }
    r = Sdrplay_set_gain(dev);
    if (r < 0)
    {
        fprintf(stderr, "can't set gain %d\n", index, r);
        return r;
    }

    *p = dev;

    return 0;
}
 

int Sdrplay_close (sdrplay_dev_t *p) {
    if (!p) return -1;

    /* ukončení async čtení okamžitě */
    Sdrplay_cancel_async_now(p);

    // similar to rtl-sdr
#if defined(_WIN32) && !defined(__MINGW32__)
            Sleep(1);
#else
            usleep(1000);
#endif

    //Stop adc
    int r = Sdrplay_adc_stop(p);
   // if (r < 0) return -1;
    /* deinicializace tuneru */
    if (p->dh)
    {
        libusb.release_interface(p->dh, 0);

#ifdef DETACH_KERNEL_DRIVER
        if (p->driver_active) {
            if (!libusb.attach_kernel_driver(p->dh, 0))
                fprintf(stderr, "Reattached kernel driver\n");
            else
                fprintf(stderr, "Reattaching kernel driver failed!\n");
        }
#endif
        if (p->async_status != async_status_t::MIRISDR_ASYNC_FAILED) {
            libusb.close(p->dh);
        }
    }

    if (p->ctx) libusb.libexit(p->ctx);

    delete(p);
    return 0;

}

int Sdrplay_reset (sdrplay_dev_t *p) {
    int r;

    if (!p) return -1;
    if (!p->dh) return -1;

    /* měli bychom uvolnit zařízení předem? */

    if ((r = libusb.reset_device(p->dh)) < 0) {
        fprintf( stderr, "failed to reset miri usb device %u with code %d\n", p->index, r);
        return r;
    }

    return 0;
}

int Sdrplay_reset_buffer (sdrplay_dev_t *p) {
    if (!p) return -1;
    if (!p->dh) return -1;
    int r;
    /* zatím není jasné k čemu by bylo, proto pouze provedeme reset async části */
    r = Sdrplay_stop_async(p);
    if (r < 0) return r;

    r = Sdrplay_start_async(p);

    return r;
}

int Sdrplay_get_usb_strings (sdrplay_dev_t *dev, char *manufact, char *product, char *serial) {
(void) dev;
    fprintf( stderr, "mirisdr_get_usb_strings not implemented yet\n");

    memset(manufact, 0, 256);
    memset(product, 0, 256);
    memset(serial, 0, 256);

    return 0;
}

int Sdrplay_set_hw_flavour (sdrplay_dev_t *p, mirisdr_hw_flavour_t hw_flavour) {
    if (!p) return -1;

    p->hw_flavour = hw_flavour;
    return 0;

}

const struct libusb_endpoint_desriptor* active_config(struct libusb_device* dev, struct libusb_device_handle* handle)
{
    struct libusb_device_handle* hDevice_req;
    struct libusb_config_descriptor* config;
   // struct libusb_endpoint_descriptor* endpoint;
    const struct libusb_endpoint_desriptor* ep=NULL;

    int altsetting_index, interface_index = 0, ret_active;
    int i, ret_print;

    hDevice_req = handle;

    ret_active = libusb.get_active_config_descriptor(dev, &config);
    //ret_print = print_configuration(hDevice_req, config);

    for (interface_index = 0; interface_index < config->bNumInterfaces; interface_index++)
    {
        const struct libusb_interface* iface = &config->interface[interface_index];
        for (altsetting_index = 0; altsetting_index < iface->num_altsetting; altsetting_index++)
        {
            const struct libusb_interface_descriptor* altsetting = &iface->altsetting[altsetting_index];

            int endpoint_index;
            for (endpoint_index = 0; endpoint_index < altsetting->bNumEndpoints; endpoint_index++)
            {
                const struct libusb_endpoint_desriptor* ep = (libusb_endpoint_desriptor*) & altsetting->endpoint[endpoint_index];
               // endpoint = ep;
               // alt_interface = altsetting->bAlternateSetting;
               // interface_number = altsetting->bInterfaceNumber;
            }

            //printf("\nEndPoint Descriptors: ");
            //printf("\n\tSize of EndPoint Descriptor: %d", endpoint->bLength);
            //printf("\n\tType of Descriptor: %d", endpoint->bDescriptorType);
            //printf("\n\tEndpoint Address: 0x0%x", endpoint->bEndpointAddress);
            //printf("\n\tMaximum Packet Size: %x", endpoint->wMaxPacketSize);
            //printf("\n\tAttributes applied to Endpoint: %d", endpoint->bmAttributes);
            //printf("\n\tInterval for Polling for data Tranfer: %d\n", endpoint->bInterval);
        }
    }
    libusb.free_config_descriptor(NULL);
    return ep;
}