#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
: nullSegmentSelector(0, 0, 0), // Null segment
unusedSegmentSelector(0, 0, 0), // Unused segment
codeSegmentSelector(0x00000000, 64*1024*1024, 0x9A), // Code segment
dataSegmentSelector(0x00000000, 64*1024*1024, 0x92) // Data segment
{
    uint32_t i[2];
    i[0] = (uint32_t)this;
    i[1] = sizeof(GlobalDescriptorTable)<< 16;

    asm volatile("lgdt (%0)" : : "p"(((uint8_t *)i)+2)); // Load the GDT
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{
    // Destructor does not need to do anything in this case
}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t* target = (uint8_t*)this;
    if(limit <= 65536)
    {
        target[6] = 0x40;
    }
    else
    {
        if((limit & 0xFFF) != 0xFFF)
        {
            limit = (limit >> 12)-1;
        }
        else 
        {
            limit = (limit >> 12);
        }
        target[6] = 0xC0;
    }
    target[0] = limit & 0xFF; // Lower 8 bits of the limit
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF; // Upper 4 bits of the limit
    
    target[2] = base & 0xFF; // Lower 8 bits of the base address
    target[3] = (base >> 8) & 0xFF; //
    target[4] = (base >> 16) & 0xFF; // Next 8 bits of the base address
    target[7] = (base >> 24) & 0xFF; 
    
    target[5] = flags; // Access flags
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    if((target[6] & 0xC0) == 0xC0)
    {
        result = (result << 12) | 0xFFF; // Convert to byte limit
    }
    return result;
}