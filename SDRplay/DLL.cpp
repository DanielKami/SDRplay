
#include "pch.h" 
#include "DLL.h"


HRESULT Libusb::Init()
{
 
     hDll = LoadLibraryW(L"libusb-1.0.dll");

    if (!hDll || hDll == INVALID_HANDLE_VALUE)
    {
        return E_HANDLE;
    }
    else
        LoadFunctions();
    return S_OK;
}




//Load all the neccesary libusb functions to our library
HRESULT Libusb::LoadFunctions()
{

        init = (LPlibusb_init)GetProcAddress(hDll, "libusb_init");
        if (NULL == init)
            return ERROR_DELAY_LOAD_FAILED;

        get_device_list = (LPlibusb_get_device_list)GetProcAddress(hDll, "libusb_get_device_list");
        if (NULL == get_device_list)
            return ERROR_DELAY_LOAD_FAILED;

        get_device_descriptor = (LPlibusb_get_device_descriptor)GetProcAddress(hDll, "libusb_get_device_descriptor");
        if (NULL == get_device_descriptor)
            return ERROR_DELAY_LOAD_FAILED;

        free_device_list = (LPlibusb_free_device_list)GetProcAddress(hDll, "libusb_free_device_list");
        if (NULL == free_device_list)
            return ERROR_DELAY_LOAD_FAILED;

        libexit = (LPlibusb_exit)GetProcAddress(hDll, "libusb_exit");
        if (NULL == libexit)
            return ERROR_DELAY_LOAD_FAILED;

        kernel_driver_active = (LPlibusb_kernel_driver_active)GetProcAddress(hDll, "libusb_kernel_driver_active");
        if (NULL == kernel_driver_active)
            return ERROR_DELAY_LOAD_FAILED;

        claim_interface = (LPlibusb_claim_interface)GetProcAddress(hDll, "libusb_claim_interface");
        if (NULL == claim_interface)
            return ERROR_DELAY_LOAD_FAILED;

        release_interface = (LPlibusb_release_interface)GetProcAddress(hDll, "libusb_release_interface");
        if (NULL == release_interface)
            return ERROR_DELAY_LOAD_FAILED;

        close = (LPlibusb_close)GetProcAddress(hDll, "libusb_close");
        if (NULL == close)
            return ERROR_DELAY_LOAD_FAILED;

        reset_device = (LPlibusb_reset_device)GetProcAddress(hDll, "libusb_reset_device");
        if (NULL == reset_device)
            return ERROR_DELAY_LOAD_FAILED;

        open = (LPlibusb_open)GetProcAddress(hDll, "libusb_open");
        if (NULL == open)
            return ERROR_DELAY_LOAD_FAILED;

        control_transfer = (LPlibusb_control_transfer)GetProcAddress(hDll, "libusb_control_transfer");
        if (NULL == control_transfer)
            return ERROR_DELAY_LOAD_FAILED;

        bulk_transfer = (LPlibusb_bulk_transfer)GetProcAddress(hDll, "libusb_bulk_transfer");
        if (NULL == bulk_transfer)
            return ERROR_DELAY_LOAD_FAILED;
        
        alloc_transfer = (LPlibusb_alloc_transfer)GetProcAddress(hDll, "libusb_alloc_transfer");
        if (NULL == alloc_transfer)
            return ERROR_DELAY_LOAD_FAILED;

        cancel_transfer = (LPlibusb_cancel_transfer)GetProcAddress(hDll, "libusb_cancel_transfer");
        if (NULL == cancel_transfer)
            return ERROR_DELAY_LOAD_FAILED;
        
        free_transfer = (LPlibusb_free_transfer)GetProcAddress(hDll, "libusb_free_transfer");
        if (NULL == free_transfer)
            return ERROR_DELAY_LOAD_FAILED;

        handle_events_timeout = (LPlibusb_handle_events_timeout)GetProcAddress(hDll, "libusb_handle_events_timeout");
        if (NULL == handle_events_timeout)
            return ERROR_DELAY_LOAD_FAILED;
        
        set_interface_alt_setting = (LPlibusb_set_interface_alt_setting)GetProcAddress(hDll, "libusb_set_interface_alt_setting");
        if (NULL == set_interface_alt_setting)
            return ERROR_DELAY_LOAD_FAILED;

        submit_transfer = (LPlibusb_submit_transfer)GetProcAddress(hDll, "libusb_submit_transfer");
        if (NULL == submit_transfer)
            return ERROR_DELAY_LOAD_FAILED;

        fill_bulk_transfer = (LPlibusb_fill_bulk_transfer)GetProcAddress(hDll, "libusb_fill_bulk_transfer");
        if (NULL == fill_bulk_transfer)
            return ERROR_DELAY_LOAD_FAILED;
        
        fill_iso_transfer = (LPlibusb_fill_iso_transfer)GetProcAddress(hDll, "libusb_fill_iso_transfer");
        if (NULL == fill_iso_transfer)
            return ERROR_DELAY_LOAD_FAILED;

        set_iso_packet_lengths = (LPlibusb_set_iso_packet_lengths)GetProcAddress(hDll, "libusb_set_iso_packet_lengths");
        if (NULL == set_iso_packet_lengths)
            return ERROR_DELAY_LOAD_FAILED;

        detach_kernel_driver = (LPlibusb_detach_kernel_driver)GetProcAddress(hDll, "libusb_detach_kernel_driver");
        if (NULL == detach_kernel_driver)
            return ERROR_DELAY_LOAD_FAILED;

        attach_kernel_driver = (LPattach_kernel_driver)GetProcAddress(hDll, "libusb_attach_kernel_driver");
        if (NULL == attach_kernel_driver)
            return ERROR_DELAY_LOAD_FAILED;
        
        get_active_config_descriptor = (LPlibusb_get_active_config_descriptor)GetProcAddress(hDll, "libusb_get_active_config_descriptor");
        if (NULL == get_active_config_descriptor)
            return ERROR_DELAY_LOAD_FAILED;

        free_config_descriptor = (LPlibusb_free_config_descriptor)GetProcAddress(hDll, "libusb_free_config_descriptor");
        if (NULL == free_config_descriptor)
            return ERROR_DELAY_LOAD_FAILED;

        get_configuration = (LPlibusb_get_configuration)GetProcAddress(hDll, "libusb_get_configuration");
        if (NULL == get_configuration)
            return ERROR_DELAY_LOAD_FAILED;
 
       set_configuration = (LPlibusb_set_configuration)GetProcAddress(hDll, "libusb_set_configuration");
        if (NULL == set_configuration)
            return ERROR_DELAY_LOAD_FAILED;

       control_setup = (LPlibusb_control_setup)GetProcAddress(hDll, " libusb_control_setup");
        if (NULL == control_setup)
            return ERROR_DELAY_LOAD_FAILED;
 
     return NO_ERROR;

}
