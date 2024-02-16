 
#pragma once
#ifndef __SDRPLAY_H
#define __SDRPLAY_H

#if COMPILING __SDRPLAY_H //BUILDING_DLL
#    define __SDR_EXPORT     __declspec(dllexport)
#else
#    define __SDR_IMPORT     __declspec(dllimport)
#  endif

 
 
// Set debug level
// 0=no debug
// 1=gain and frequency info.
// 2=extended debug
#define MIRISDR_DEBUG 0

#include <stdint.h>
#include "structs.h"
#define DETACH_KERNEL_DRIVER
 
typedef struct Sdrplay_dev_struct sdrplay_dev_t;
typedef void(*mirisdr_read_async_cb_t) (uint8_t* buf, uint32_t len, void* ctx);
 
extern "C" 
{

/* devices */
__SDR_IMPORT int32_t Sdrplay_get_device_count ();
__SDR_IMPORT const int Sdrplay_get_device_name(uint32_t, char* );
__SDR_IMPORT int Sdrplay_get_device_usb_strings (uint32_t index, char *manufact, char *product, char *serial);

/* main */
__SDR_IMPORT int Sdrplay_open (sdrplay_dev_t **p, uint32_t index);
__SDR_IMPORT int Sdrplay_close (sdrplay_dev_t *p);
__SDR_IMPORT int Sdrplay_reset (sdrplay_dev_t *p);                       /* extra */
__SDR_IMPORT int Sdrplay_reset_buffer (sdrplay_dev_t *p);
__SDR_IMPORT int Sdrplay_get_usb_strings (sdrplay_dev_t *dev, char *manufact, char *product, char *serial);
__SDR_IMPORT int Sdrplay_set_hw_flavour (sdrplay_dev_t *p, mirisdr_hw_flavour_t hw_flavour);

/* sync */
/*From libUSB. When you ask libusb to submit a bulk transfer larger than 16kb in size,
* libusb breaks it up into a number of smaller subtransfers.This is because
* the usbfs kernel interface only accepts transfers of up to 16kb in size.
* The subtransfers are submitted all at once so that the kernel can queue
* them at the hardware level, therefore maximizing bus throughput.*/
__SDR_IMPORT int Sdrplay_read_sync (sdrplay_dev_t* p, void* bufOut, uint16_t len, uint16_t* n_read, uint32_t* lost);
__SDR_IMPORT uint16_t Sdrplay_BufforSize(sdrplay_dev_t* p, uint16_t InternalBuffLenght);

/* async */
__SDR_IMPORT int Sdrplay_read_async (sdrplay_dev_t *p, mirisdr_read_async_cb_t cb, void *ctx, uint32_t num, uint32_t len);
__SDR_IMPORT int Sdrplay_cancel_async (sdrplay_dev_t *p);
__SDR_IMPORT int Sdrplay_cancel_async_now (sdrplay_dev_t *p);            /* extra */
__SDR_IMPORT int Sdrplay_start_async (sdrplay_dev_t *p);                 /* extra */
__SDR_IMPORT int Sdrplay_stop_async (sdrplay_dev_t *p);                  /* extra */

/* adc */
__SDR_IMPORT int Sdrplay_adc_init (sdrplay_dev_t *p);                    /* extra */
__SDR_IMPORT int Sdrplay_adc_stop (sdrplay_dev_t* p);

/* rate control */
__SDR_IMPORT int Sdrplay_set_sample_rate (sdrplay_dev_t *p, uint32_t rate);
__SDR_IMPORT uint32_t Sdrplay_get_sample_rate (sdrplay_dev_t *p);

/* sample format control */
__SDR_IMPORT int Sdrplay_set_sample_format (sdrplay_dev_t *p, format_t tt);  /* extra */
__SDR_IMPORT format_t Sdrplay_get_sample_format (sdrplay_dev_t *p);   /* extra */

/* streaming control */
__SDR_IMPORT int Sdrplay_streaming_start (sdrplay_dev_t *p);             /* extra */
__SDR_IMPORT int Sdrplay_streaming_stop (sdrplay_dev_t *p);              /* extra */

/* frequency */
__SDR_IMPORT int Sdrplay_set_center_freq (sdrplay_dev_t *p, uint32_t freq);
__SDR_IMPORT uint32_t Sdrplay_get_center_freq (sdrplay_dev_t *p);
__SDR_IMPORT int Sdrplay_set_if_freq (sdrplay_dev_t *p, uint32_t freq);  /* extra */
__SDR_IMPORT uint32_t Sdrplay_get_if_freq (sdrplay_dev_t *p);            /* extra */
__SDR_IMPORT int Sdrplay_set_xtal_freq (sdrplay_dev_t *p, uint32_t freq);/* extra */
__SDR_IMPORT uint32_t Sdrplay_get_xtal_freq (sdrplay_dev_t *p);          /* extra */
__SDR_IMPORT int Sdrplay_set_bandwidth (sdrplay_dev_t *p, uint32_t bw);  /* extra */
__SDR_IMPORT uint32_t Sdrplay_get_bandwidth (sdrplay_dev_t *p);          /* extra */
__SDR_IMPORT int Sdrplay_set_offset_tuning (sdrplay_dev_t *p, int on);   /* extra */
__SDR_IMPORT mirisdr_band_t Sdrplay_get_band (sdrplay_dev_t *p);         /* extra */
__SDR_IMPORT int Sdrplay_set_freq_correction (sdrplay_dev_t *p, int ppm);

/* not implemented yet */
__SDR_IMPORT int Sdrplay_set_direct_sampling (sdrplay_dev_t *p, int on);

/* transfer */
__SDR_IMPORT int Sdrplay_set_transfer (sdrplay_dev_t *p, transfer_t tr);       /* extra */
__SDR_IMPORT transfer_t Sdrplay_get_transfer (sdrplay_dev_t *p);        /* extra */

/* gain */
__SDR_IMPORT int Sdrplay_set_gain (sdrplay_dev_t *p);                    /* extra */
__SDR_IMPORT int Sdrplay_tuner_gains (sdrplay_dev_t *dev, int *gains);
__SDR_IMPORT int Sdrplay_tuner_gain (sdrplay_dev_t *p, int gain);
__SDR_IMPORT int Sdrplay_get_tuner_gain (sdrplay_dev_t *p);              /* extra */
 
__SDR_IMPORT int Sdrplay_set_tuner_gain_mode (sdrplay_dev_t *p, int mode);
__SDR_IMPORT int Sdrplay_get_tuner_gain_mode (sdrplay_dev_t *p);         /* extra */

__SDR_IMPORT int Sdrplay_get_mixer_gain(sdrplay_dev_t* p);              /* extra */
__SDR_IMPORT int Sdrplay_set_mixer_gain(sdrplay_dev_t *p, int gain);    /* extra */

__SDR_IMPORT int Sdrplay_get_mixbuffer_gain(sdrplay_dev_t* p);          /* extra */
__SDR_IMPORT int Sdrplay_set_mixbuffer_gain(sdrplay_dev_t *p, int gain);/* extra */

__SDR_IMPORT int Sdrplay_get_lna_gain(sdrplay_dev_t *p);                /* extra */
__SDR_IMPORT int Sdrplay_set_lna_gain(sdrplay_dev_t* p, int gain);       /* extra */

__SDR_IMPORT int Sdrplay_get_baseband_gain(sdrplay_dev_t* p);           /* extra */
__SDR_IMPORT int Sdrplay_set_baseband_gain(sdrplay_dev_t* p, int gain);  /* extra */

__SDR_IMPORT int Sdrplay_set_bias (sdrplay_dev_t *p, int bias);           /* extra */
__SDR_IMPORT int Sdrplay_get_bias (sdrplay_dev_t *p);                    /* extra */

 
//USB init
__SDR_IMPORT HRESULT Sdrplay_libUSBInit(HINSTANCE hDll);
}

const struct libusb_endpoint_desriptor* active_config(struct libusb_device* dev, struct libusb_device_handle* handle);

#endif /* __SDRPLAY_H */
