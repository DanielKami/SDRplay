#pragma once

#ifndef __DEVICES_H
#define __DEVICES_H


#include "structs.h"
#include "mirisdr.h"

static SDRplayr_device_t SDRplay_devices[] = {
    { 0x1df7, 0x2500, "Mirics MSi2500 default (e.g. VTX3D card)", "Mirics", "MSi2500"},
    { 0x1df7, 0x3000, "SDRplay RSP1A", "SDRPlay", "RSP1A"},
    { 0x1df7, 0x3010, "SDRplay RSP1A", "SDRPlay", "RSP2"},
    { 0x2040, 0xd300, "Hauppauge WinTV 133559 LF", "Hauppauge", "WinTV 133559 LF"},
    { 0x07ca, 0x8591, "AverMedia A859 Pure DVBT", "AverTV", "A859 Pure DVBT"},
    { 0x04bb, 0x0537, "IO-DATA GV-TV100 stick", "IO-DATA", "GV-TV100"},
    { 0x0511, 0x0037, "Logitec LDT-1S310U/J", "Logitec", "LDT-1S310U/J"},
    //    { 0x1DF7, 0x2500, "SDRplay RSP1C", "China", "RSP1C"}
    { 0x0bda, 0x2838, "* Generic RTL2832U",  "Logitec", "LDT-1S310U/J" },
};

int Sdrplay_device_get(uint16_t vid, uint16_t pid);
#endif 