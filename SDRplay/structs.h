#include "libusb.h"
 

#ifndef __STRUCT_DEF_H
#define __STRUCT_DEF_H

typedef void(*mirisdr_read_async_cb_t) (uint8_t* buf, uint32_t len, void* ctx);

typedef enum
{
    MIRISDR_BAND_AM1,
    MIRISDR_BAND_AM2,
    MIRISDR_BAND_VHF,
    MIRISDR_BAND_3,
    MIRISDR_BAND_45,
    MIRISDR_BAND_L,
} mirisdr_band_t;

typedef enum
{
    MIRISDR_HW_DEFAULT,
    MIRISDR_HW_SDRPLAY,
} mirisdr_hw_flavour_t;

typedef enum
{
    MIRISDR_TRANSFER_BULK = 0,
    MIRISDR_TRANSFER_ISOC
} transfer_t;

 
typedef enum
{
    MIRISDR_FORMAT_252_S16 = 0,
    MIRISDR_FORMAT_336_S16,
    MIRISDR_FORMAT_384_S16,
    MIRISDR_FORMAT_504_S16,
    MIRISDR_FORMAT_504_S8,
    MIRISDR_FORMAT_AUTO_ON
} format_t;

typedef enum
{
    MIRISDR_BW_200KHZ = 0,
    MIRISDR_BW_300KHZ,
    MIRISDR_BW_600KHZ,
    MIRISDR_BW_1536KHZ,
    MIRISDR_BW_5MHZ,
    MIRISDR_BW_6MHZ,
    MIRISDR_BW_7MHZ,
    MIRISDR_BW_8MHZ
} bandwidth_t;


typedef struct mirisdr_device 
{
    uint16_t            vid;
    uint16_t            pid;
    const char          *name;
    const char          *manufacturer;
    const char          *product;
} SDRplayr_device_t;


typedef enum
{
    MIRISDR_IF_ZERO = 0,
    MIRISDR_IF_450KHZ,
    MIRISDR_IF_1620KHZ,
    MIRISDR_IF_2048KHZ
} if_freq_t;

typedef enum
{
    MIRISDR_XTAL_19_2M = 0,
    MIRISDR_XTAL_22M,
    MIRISDR_XTAL_24M,
    MIRISDR_XTAL_24_576M,
    MIRISDR_XTAL_26M,
    MIRISDR_XTAL_38_4M
} xtal_t;

typedef enum
{
    MIRISDR_ASYNC_INACTIVE = 0,
    MIRISDR_ASYNC_CANCELING,
    MIRISDR_ASYNC_RUNNING,
    MIRISDR_ASYNC_PAUSED,
    MIRISDR_ASYNC_FAILED
} async_status_t;


struct Sdrplay_dev_struct 
{
    libusb_context      *ctx;
    struct libusb_device_handle *dh;

    /* parametry */
    uint32_t            index;
    uint32_t            freq;
    uint32_t            freq_correction;
    uint32_t            rate;
    int                 gain;
    int                 gain_reduction_lna;
    int                 gain_reduction_mixbuffer;
    int                 gain_reduction_mixer;
    int                 gain_reduction_baseband;
   
    mirisdr_hw_flavour_t hw_flavour;
    mirisdr_band_t      band;
    transfer_t transfer;
    bandwidth_t bandwidth;
    format_t format;
    bool auto_format;
    xtal_t xtal;
    if_freq_t if_freq;

    async_status_t async_status;
    mirisdr_read_async_cb_t cb;
    void                *cb_ctx;
    size_t              xfer_buf_num;
    struct libusb_transfer **xfer;
    uint8_t       **xfer_buf;
    size_t              xfer_out_len;
    size_t              xfer_out_pos;
    uint8_t       *xfer_out;
    uint32_t            addr;
    int                 driver_active;
    int                 bias;
    int                 reg8;
};

#endif

