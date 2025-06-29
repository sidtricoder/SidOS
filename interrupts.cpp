#include "interrupts.h"
#include "types.h"
#include "gdt.h" // Include the Global Descriptor Table header

void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256]; // Initialize the interrupt descriptor table


void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber, 
    uint16_t gdt_codeSegmentSelectorOffset, 
    void (*handler)(), 
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
) // Method to set an entry in the interrupt descriptor table
{
    const uint8_t IDT_DESC_PRESENT = 0x80; // Present bit
    // Set the low bits of the handler address
    interruptDescriptorTable[interruptNumber].handlerAddressedLowBits = ((uint32_t)handler & 0xFFFF);
    
    // Set the GDT code segment selector
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = gdt_codeSegmentSelectorOffset;
    
    // Reserved byte is set to 0
    interruptDescriptorTable[interruptNumber].reserved = 0;
    
    // Set the access byte with the appropriate flags
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel&3)<<5) | DescriptorType;
    
    // Set the high bits of the handler address
    interruptDescriptorTable[interruptNumber].handlerAddressedHighBits = (((uint32_t)handler >> 16) & 0xFFFF);
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
{
    uint32_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE; // Interrupt gate type
    for(uint16_t i = 0; i < 256; i++) {
        // Initialize each entry in the interrupt descriptor table
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    // Set up the IDT pointer structure
    interruptDescriptorTablePointer idtPointer;
    idtPointer.size = sizeof(GateDescriptor) * 256 - 1; // Size of the IDT in bytes
    idtPointer.base = (uint32_t)interruptDescriptorTable; // Base address of the IDT
    // Load the IDT using the lidt instruction
    asm volatile("lidt %0" : : "m"(idtPointer));
}
InterruptManager::~InterruptManager(){}

void InterruptManager::Activate() 
{
    // Activate the interrupt manager by enabling interrupts
    asm volatile("sti"); // Set the interrupt flag to enable interrupts
}
uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    // Handle the specified interrupt
    // This is a placeholder implementation; actual handling logic will depend on the system architecture and requirements
    // For example, you might want to save the state of the CPU, call an interrupt handler function, etc.
    printf(" INTERRUPT");
    // For now, just return the stack pointer unchanged
    return esp;
}