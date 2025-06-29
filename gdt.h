#ifndef __GDT_H
#define __GDT_H

#include "types.h"

class GlobalDescriptorTable 
{
    public:
        class SegmentDescriptor
        {
            private:
                uint16_t limit_low; // Lower 16 bits of the segment limit
                uint16_t base_low;  // Lower 16 bits of the base address
                uint8_t base_hi; // Next 8 bits of the base address
                uint8_t type;      // Access flags
                uint8_t flags_limit_hi; 
                uint8_t base_vhi;   // Upper 8 bits of the base address
            public:
                SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t Base();
                uint32_t Limit();
        } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();
        uint16_t CodeSegmentSelector();
        uint16_t DataSegmentSelector();
};

#endif // __GDT_H