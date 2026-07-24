#pragma once

//#include <stdint.h>
#include "main.h"


// Sending functions
void sendUSBpos();  // Send current position (int steps)
void sendUSBreachPos();  // Send, that reach setted position
void sendUSBreachForce();  // Send, that complete
void sendUSBstate();     // Send mesage, representing current state
void sendUSBpacket();    // Send prepaired measured packet

// Function for check, if get new messages from USB
void getUSBdata(uint8_t* Buf, uint32_t* Len);
