#pragma once

//#include <stdint.h>
#include "main.h"


// Sending functions
void sendUSBstate();   // Send mesage, representing current state
void sendUSBpacket();  // Send prepaired measured packet

// Function for check, if get new messages from USB
void getUSBdata(uint8_t* Buf, uint32_t* Len);
