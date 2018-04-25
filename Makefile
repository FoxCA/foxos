version = 0.0.2
arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/fox-$(version)-$(arch).iso
dirs = $(shell find src/arch/$(arch)/ -type d -print)
includedirs :=  $(sort $(foreach dir, $(foreach dir1, $(dirs), $(shell dirname $(dir1))), $(wildcard $(dir)/include)))
linker_script := src/arch/$(arch)/link.ld
grub_cfg := src/arch/$(arch)/grub/menu.lst

CFLAGS= -m32 -Wall -O -fno-pie -fstrength-reduce -fomit-frame-pointer	\
        -finline-functions -nostdinc -fno-builtin -ffreestanding		\
        -fno-stack-protector -c

CFLAGS += $(foreach dir, $(includedirs), -I./$(dir))

assembly_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.asm))
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, \
    build/arch/$(arch)/%.o, $(assembly_source_files))

c_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
c_object_files := $(patsubst src/arch/$(arch)/%.c, \
    build/arch/$(arch)/%.o, $(c_source_files))

.PHONY: all clean run runrel iso

all: $(kernel)

install:
	@sudo apt-get install qemu nasm

install-all:
	@sudo apt-get install qemu nasm virtualbox

clean:
	@rm -r build

run: $(iso)
	@echo starting emulator...
	@qemu-system-x86_64 -m 400M -cdrom $(iso) -no-reboot -device isa-debug-exit,iobase=0xf4,iosize=0x04

runrel: $(iso)
	@echo starting emulator...
	@qemu-system-x86_64  -m 65536k -cdrom $(iso) -device isa-debug-exit,iobase=0xf4,iosize=0x04

runv: $(iso)
	@virtualbox $(iso)

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@echo generating iso file...
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@cp src/arch/$(arch)/grub/stage2_eltorito build/isofiles/boot/grub
	@mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -quiet -input-charset utf8 -boot-load-size 4 -boot-info-table -o $(iso) build/isofiles
	@rm -r build/isofiles

$(kernel): $(assembly_object_files) $(c_object_files) $(linker_script)
	@echo linking...
	@ld -m -nostdlib -m elf_i386 -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@nasm -i./src/arch/$(arch)/ -felf32 $< -o $@

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.c
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@gcc $(CFLAGS) $< -o $@
