#pragma once

#ifndef __DLL_DEF_H
#define __DLL_DEF_H


#include "windows.h"
#include "libusb.h"



//int LIBUSB_CALL libusb_init(libusb_context** ctx); 
typedef int (CALLBACK* LPlibusb_init)(libusb_context** ctx);

//ssize_t LIBUSB_CALL libusb_get_device_list(libusb_context* ctx,	libusb_device*** list);
typedef ssize_t(CALLBACK* LPlibusb_get_device_list)(libusb_context* ctx, libusb_device*** list);

//int LIBUSB_CALL libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor* desc);
typedef int(CALLBACK* LPlibusb_get_device_descriptor)(libusb_device* dev, struct libusb_device_descriptor* desc);

//void LIBUSB_CALL libusb_free_device_list(libusb_device **list, int unref_devices);
typedef void(CALLBACK* LPlibusb_free_device_list)(libusb_device** list, int unref_devices);

//void LIBUSB_CALL libusb_exit(libusb_context *ctx);
typedef void(CALLBACK* LPlibusb_exit)(libusb_context* ctx);

//int LIBUSB_CALL libusb_kernel_driver_active(libusb_device_handle *dev, int interface_number);
typedef int(CALLBACK* LPlibusb_kernel_driver_active)(libusb_device_handle* dev, int interface_number);

//int LIBUSB_CALL libusb_claim_interface(libusb_device_handle *dev, int interface_number);
typedef int(CALLBACK* LPlibusb_claim_interface)(libusb_device_handle* dev, int interface_number);

//int LIBUSB_CALL libusb_release_interface(libusb_device_handle *dev, int interface_number);
typedef int(CALLBACK* LPlibusb_release_interface)(libusb_device_handle* dev, int interface_number);

//int LIBUSB_CALL libusb_detach_kernel_driver(libusb_device_handle *dev_handle, int interface_number);
typedef int(CALLBACK* LPlibusb_detach_kernel_driver)(libusb_device_handle* dev, int interface_number);

//int LIBUSB_CALL libusb_attach_kernel_driver(libusb_device_handle* dev_handle, int interface_number);
typedef int(CALLBACK* LPattach_kernel_driver)(libusb_device_handle* dev, int interface_number);

//void LIBUSB_CALL libusb_close(libusb_device_handle *dev_handle);
typedef void(CALLBACK* LPlibusb_close)(libusb_device_handle* dev_handle);
 
//int LIBUSB_CALL libusb_reset_device(libusb_device_handle *dev);
typedef int(CALLBACK* LPlibusb_reset_device)(libusb_device_handle* dev);

//int LIBUSB_CALL libusb_open(libusb_device *dev, libusb_device_handle **handle);
typedef int(CALLBACK* LPlibusb_open)(libusb_device* dev, libusb_device_handle** dev_handle);

//int LIBUSB_CALL libusb_control_transfer(libusb_device_handle* dev_handle,uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint8_t* data, uint16_t wLength, uint16_t timeout);
typedef int(CALLBACK* LPlibusb_control_transfer)(libusb_device_handle* dev_handle, uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint8_t* data, uint16_t wLength, uint32_t timeout);

//int LIBUSB_CALL libusb_bulk_transfer(libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length, int* actual_length, uint16_t timeout);
typedef int(CALLBACK* LPlibusb_bulk_transfer)(libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, uint16_t length, uint16_t* actual_length, uint16_t timeout);

//static inline void libusb_fill_bulk_transfer(struct libusb_transfer* transfer, libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* buffer, int length, libusb_transfer_cb_fn callback, void* user_data, uint16_t timeout)
typedef void (CALLBACK* LPlibusb_fill_bulk_transfer)(struct libusb_transfer* transfer, libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* buffer, uint16_t length, libusb_transfer_cb_fn callback, void* user_data, uint16_t timeout);

//struct libusb_transfer* LIBUSB_CALL libusb_alloc_transfer(int iso_packets);
typedef struct LPlibusb_transfer* (CALLBACK* LPlibusb_alloc_transfer)(int iso_packets);

//int LIBUSB_CALL libusb_cancel_transfer(struct libusb_transfer *transfer);
typedef int (CALLBACK* LPlibusb_cancel_transfer)(struct libusb_transfer* transfer);

//void LIBUSB_CALL libusb_free_transfer(struct libusb_transfer *transfer);
typedef void (CALLBACK* LPlibusb_free_transfer)(struct libusb_transfer* transfer);

//int LIBUSB_CALL libusb_handle_events_timeout(libusb_context* ctx, struct timeval* tv);
typedef int (CALLBACK* LPlibusb_handle_events_timeout)(libusb_context* ctx, struct timeval* tv);

//int LIBUSB_CALL libusb_set_interface_alt_setting(libusb_device_handle *dev,int interface_number, int alternate_setting);
typedef int (CALLBACK* LPlibusb_set_interface_alt_setting)(libusb_device_handle* dev, int interface_number, int alternate_setting);

//int LIBUSB_CALL libusb_submit_transfer(struct libusb_transfer *transfer);
typedef int (CALLBACK* LPlibusb_submit_transfer)(struct libusb_transfer* transfer);

//static inline void libusb_fill_iso_transfer(struct libusb_transfer* transfer,libusb_device_handle* dev_handle, uint8_t endpoint,uint8_t* buffer, int length, int num_iso_packets,libusb_transfer_cb_fn callback, void* user_data, uint16_t timeout)
typedef int (CALLBACK* LPlibusb_fill_iso_transfer)(struct libusb_transfer* transfer, libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* buffer, uint16_t length, int num_iso_packets, libusb_transfer_cb_fn callback, void* user_data, uint16_t timeout);

//static inline void libusb_set_iso_packet_lengths(struct libusb_transfer* transfer, uint16_t length)
typedef int (CALLBACK* LPlibusb_set_iso_packet_lengths)(struct libusb_transfer* transfer, uint16_t length);

//int LIBUSB_CALL libusb_get_active_config_descriptor(libusb_device* dev, struct libusb_config_descriptor** config);
typedef int (CALLBACK* LPlibusb_get_active_config_descriptor)(libusb_device* dev, struct libusb_config_descriptor** config);

//void LIBUSB_CALL libusb_free_config_descriptor( struct libusb_config_descriptor* config);
typedef void (CALLBACK* LPlibusb_free_config_descriptor)(struct libusb_config_descriptor* config);

//int LIBUSB_CALL libusb_get_configuration(libusb_device_handle* dev,	int* config);
typedef int (CALLBACK* LPlibusb_get_configuration)(libusb_device_handle* dev, int* config);

//int LIBUSB_CALL libusb_set_configuration(libusb_device_handle* dev_handle, int configuration);
typedef int (CALLBACK* LPlibusb_set_configuration)(libusb_device_handle* dev_handle, int configuration);

//int LIBUSB_CALL libusb_set_configuration(libusb_device_handle* dev_handle, int configuration);
typedef int (CALLBACK* LPlibusb_set_configuration)(libusb_device_handle* dev_handle, int configuration);

//static inline struct libusb_control_setup* libusb_control_transfer_get_setup( struct libusb_transfer* transfer)
typedef struct libusb_control_setup* (CALLBACK* LPlibusb_control_setup)(struct libusb_transfer* transfer);


class Libusb
{
 
public:

	LPlibusb_init init;
	LPlibusb_get_device_list get_device_list;
	LPlibusb_get_device_descriptor get_device_descriptor;
	LPlibusb_free_device_list free_device_list;
	LPlibusb_exit libexit;
	LPlibusb_kernel_driver_active kernel_driver_active;
	LPlibusb_detach_kernel_driver detach_kernel_driver;
	LPattach_kernel_driver attach_kernel_driver;	
	LPlibusb_claim_interface claim_interface;
	LPlibusb_release_interface release_interface;
	LPlibusb_close close;
	LPlibusb_reset_device reset_device;
	LPlibusb_open open;
	LPlibusb_control_transfer control_transfer;
	LPlibusb_bulk_transfer bulk_transfer;
	LPlibusb_alloc_transfer alloc_transfer;
	LPlibusb_cancel_transfer cancel_transfer;
	LPlibusb_free_transfer free_transfer;
	LPlibusb_handle_events_timeout handle_events_timeout;
	LPlibusb_set_interface_alt_setting set_interface_alt_setting;
	LPlibusb_submit_transfer submit_transfer;
	LPlibusb_fill_bulk_transfer fill_bulk_transfer;
	LPlibusb_fill_iso_transfer  fill_iso_transfer;
	LPlibusb_set_iso_packet_lengths set_iso_packet_lengths;
	LPlibusb_get_active_config_descriptor get_active_config_descriptor;
	LPlibusb_free_config_descriptor free_config_descriptor;
	LPlibusb_get_configuration get_configuration;
	LPlibusb_set_configuration  set_configuration;
	LPlibusb_control_setup control_setup;
	 

	HINSTANCE hDll = NULL; //Handle to DLL file libusb-1.0.dll

//private:
	HRESULT Init();
	HRESULT LoadFunctions();

	Libusb() {
	 Init();
	}

	~Libusb() {

		FreeLibrary(hDll);
	}
};


#endif