#include "port.h"

Port::Port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

Port::~Port()
{
    // Destructor does nothing
}

Port8Bit::Port8Bit(uint16_t portnumber) : Port(portnumber)
{
    // Constructor does nothing
}

Port8Bit::~Port8Bit()
{
    // Destructor does nothing
}

uint8_t Port8Bit::Read()
{
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}

void Port8Bit::Write(uint8_t data)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(portnumber));
}

Port8BitSlow::Port8BitSlow(uint16_t portnumber) : Port8Bit(portnumber)
{
    // Constructor does nothing
}

Port8BitSlow::~Port8BitSlow()
{
    // Destructor does nothing
}

uint8_t Port8BitSlow::Read()
{
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}

void Port8BitSlow::Write(uint8_t data)
{
    __asm__ __volatile__("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a"(data), "Nd"(portnumber));
    // Add a small delay to ensure the port is ready for the next operation
}

Port16Bit::Port16Bit(uint16_t portnumber) : Port(portnumber)
{
    // Constructor does nothing
}

Port16Bit::~Port16Bit()
{
    // Destructor does nothing
}

uint16_t Port16Bit::Read()
{
    uint16_t result;
    __asm__ __volatile__("inw %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}

void Port16Bit::Write(uint16_t data)
{
    __asm__ __volatile__("outw %0, %1" : : "a"(data), "Nd"(portnumber));
}

Port32Bit::Port32Bit(uint16_t portnumber) : Port(portnumber)
{
    // Constructor does nothing
}

Port32Bit::~Port32Bit()
{
    // Destructor does nothing
}

uint32_t Port32Bit::Read()
{
    uint32_t result;
    __asm__ __volatile__("inl %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}

void Port32Bit::Write(uint32_t data)
{
    __asm__ __volatile__("outl %0, %1" : : "a"(data), "Nd"(portnumber));
}

