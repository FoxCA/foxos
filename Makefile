version = 0.0.2
arch ?= x86_64
kernel := build/kernel-$(arch).bin
kernel_fullname := kernel-$(arch).bin
iso := build/fox-$(version)-$(arch).iso
img := build/fox-$(version)-$(arch).img
dirs = $(shell find src/arch/$(arch)/ -type d -print)
includedirs :=  $(sort $(foreach dir, $(foreach dir1, $(dirs), $(shell dirname $(dir1))), $(wildcard $(dir)/include)))
linker_script := src/arch/$(arch)/link.ld
grub_cfg := src/arch/$(arch)/grub/menu.lst
grub_cfg_img := src/arch/$(arch)/grub/grub.cfg
grub_timeout := 10

CFLAGS= -m32 -Wall -O -fno-pie -fstrength-reduce -fomit-frame-pointer	\
        -finline-functions -nostdinc -fno-builtin -ffreestanding		\
        -fno-stack-protector -c -Wno-unused-variable -Wno-maybe-uninitialized -Wno-error=varargs \
        -Wno-error=discarded-qualifiers -Wno-error=unused-function
# Wunused-variable will be ignored!

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

runold: $(kernel)
	@echo starting emulator...
	@qemu-system-x86_64 -m 1G -kernel $(kernel) -device isa-debug-exit,iobase=0xf4,iosize=0x04 -hda ext2_hda.img -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img -serial stdio

boot:
	@echo starting emulator...
	@qemu-system-x86_64  -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -hda $(img) -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img

run: $(img)
	@echo starting emulator...
	@qemu-system-x86_64 -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -hda $(img) -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img

runnoreboot: $(img)
	@echo starting emulator...
	@qemu-system-x86_64  -m 1G -no-reboot -device isa-debug-exit,iobase=0xf4,iosize=0x04 -hda $(img) -hdb ext2_hdb.img -hdc ext2_hdc.img -hdd ext2_hdd.img

rundebug: $(kernel)
	@echo starting emulator...
	@qemu-system-x86_64 -s -S -m 400M -device isa-debug-exit,iobase=0xf4,iosize=0x04

runv: $(img)
	@virtualbox $(img)

$(img): $(kernel) config
	@./mkext2image.sh
	@./mkimg.sh $(img)
	sudo cp $(kernel) /mnt/boot
	sudo cp $(grub_cfg_img) /mnt/boot/grub
	@sudo umount /mnt

iso: $(iso)

config:
	@echo generating config... 
	# @echo 'set color_highlight=magenta/blue\nset color_normal=white/blue\nset timeout=$(grub_timeout)\nset default=0\nmenuentry "foxos" {\n    multiboot /boot/$(kernel_fullname)\n}\nmenuentry "Shutdown" --class shutdown {\n    halt\n}\nmenuentry "Reboot" --class shutdown {\n    reboot\n}\n' > $(grub_cfg_img)
	@echo 'set color_highlight=black/cyan\nset color_normal=white/cyan\nset timeout=$(grub_timeout)\nset default=0\nmenuentry "foxos" {\n    multiboot /boot/$(kernel_fullname)\n}\nmenuentry "Shutdown" --class shutdown {\n    halt\n}\nmenuentry "Reboot" --class shutdown {\n    reboot\n}\n' > $(grub_cfg_img)

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
