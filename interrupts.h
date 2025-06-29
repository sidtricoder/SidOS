#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "types.h"

class InterruptManager 
{

protected:

    struct GateDescriptor
    {
        uint16_t handlerAddressedLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressedHighBits;
    } __attribute__((packed)); // Define a packed structure for GateDescriptor
    static GateDescriptor interruptDescriptorTable[256]; // Array to hold the interrupt descriptor table

    static void SetInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t gdt_codeSegmentSelectorOffset, void (*handler)(), uint8_t DescriptorPrivilegeLevel); // Method to set an entry in the interrupt descriptor table


public:
    // Method to handle an interrupt
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp); // Code to handle the specified interrupt

    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();

};

#endif