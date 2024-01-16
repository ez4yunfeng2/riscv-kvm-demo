PREFIX=riscv64-unknown-linux-gnu-
CC=$(PREFIX)gcc
CFLAGS+=-Wall -static
kernel:
	$(PREFIX)gcc -nostartfiles -T demo.lds demo.c -o output/demo
	$(PREFIX)objcopy -O binary --strip-all output/demo output/demo.bin

demo:
	$(PREFIX)gcc -mcmodel=medany -nostartfiles -T demo.lds demo.S -o output/demo
	$(PREFIX)objdump -d output/demo > output/demo.dis
	$(PREFIX)objcopy -O binary --strip-all output/demo output/demo.bin

build: kvm
	@echo Build OK

fs: build demo
	cp -f $(FILE) ../busybox-1.33.1-kvm-riscv64/_install/apps
	cd ../busybox-1.33.1-kvm-riscv64/_install; find ./ | cpio -o -H newc > ../../rootfs_kvm_riscv64.img; cd -

run:
	../qemu/build/qemu-system-riscv64 \
		-cpu rv64 -M virt -m 512M -nographic \
		-kernel ../build-riscv64/arch/riscv/boot/Image \
		-initrd ../rootfs_kvm_riscv64.img \
		-append "root=/dev/ram rw console=ttyS0 earlycon=sbi"