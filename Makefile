GPPPARAMS = -m32 -fno-exceptions -fno-rtti -nostdlib -fno-builtin -fno-use-cxa-atexit -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -m elf_i386

objects = kernel.o gdt.o port.o loader.o interrupts.o interruptstubs.o

%.o : %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o : %.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

clean:
	rm -f $(objects) mykernel.bin mykernel.iso
	rm -rf iso

mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SidOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso

run: mykernel.iso
	make loader.o
	make kernel.o
	make mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "SidOS" &

.PHONY: clean install