#include "types.h"
#include "gdt.h"
#include "interrupts.h"

void printf(char *str)
{
    static uint16_t* VideoMemory = (uint16_t *) 0xb8000;    
    for(int i = 0; str[i] != '\0'; i++)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor *ctor = &start_ctors; ctor < &end_ctors; ctor++)
        (*ctor)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{   
    printf("Hello! Welcome to SidOS");
    GlobalDescriptorTable gdt;
    InterruptManager interruptManager(&gdt);
    interruptManager.Activate(); // Enable interrupts
    while(1);
}