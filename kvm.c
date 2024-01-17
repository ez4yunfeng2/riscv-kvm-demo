#include "kvm.h"
#include <stdio.h>
#include <unistd.h>

void initram(void *ram) {
    struct kvm_userspace_memory_region mem = {
        .slot = 0,
        .flags = 0,
        .guest_phys_addr = 0,
        .memory_size = 1024 * 1024 * 100,
        .userspace_addr = (uint64_t)ram,
    };
    ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &mem);
}

void readfile(char *filename, uint8_t *buffer) {
    int fd = open(filename, O_RDONLY);
    if( fd < 0 ) {
        printf("[Error] File not found\r\n");
    }
    while( read(fd, buffer, 1) > 0 ) {
        buffer++;
    }
    close(fd);
}



void arch_init() {
	// register sbi support
	struct kvm_one_reg reg;
	unsigned long data = KVM_REG_RISCV_SBI_MULTI_MASK(0);
	reg.id   = RISCV_SBI_EXT_REG(KVM_REG_RISCV_SBI_SINGLE, 0);
	reg.addr = (unsigned long)&data;

	// dump reg.id reg.addr
	printf("reg.id: %x\r\n", reg.id);
	printf("reg.addr: %x\r\n", data);

	if (ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg) < 0) {
		die("KVM_GET_ONE_REG");
	}

}

int emulate_sbi (unsigned long eid, unsigned long fid, unsigned long arg) {
	int ret = -1;
	// printf("eid: %d, fid: %d, arg: %x %d\r\n", eid, fid, arg, eid == 1);
	if(eid == 1) {
		printf("%c",(char)arg);
		ret = 0;
	} else if (eid == 0x8) {
		is_runing = 0;
	}
	return ret;
}

void add_pc() {
	struct kvm_one_reg reg;
	unsigned long data = 0;
	reg.id   = RISCV_CORE_REG(regs.pc);;
	reg.addr = (unsigned long)&data;
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0) {
		die("KVM_GET_ONE_REG");
	}
	reg.addr = (unsigned long)&data;
	if (ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg) < 0) {
		die("KVM_GET_ONE_REG");
	}
}

static int num = 0;
static int len = 19;
char *message = "Read From CharDev!\n";

// _len always 1
void do_chrdev(int is_write, unsigned char *data, int _len) {
	if(is_write) {
		printf("%c",data[0]);
	} else {
		if(num < len) {
			data[0] = message[num];
			num++;
		} else {
			data[0] = 0;
		}
	}
}

int main(void) {

    uint8_t* ram = mmap(NULL, 1024 * 1024 * 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    sys_fd = open("/dev/kvm", O_RDWR);
    vm_fd  = ioctl(sys_fd, KVM_CREATE_VM, 0);
    vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);

    int mmap_size = ioctl(sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);

    runs = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu_fd, 0);

    printf("KVM (%d), VMDF (%d), VCPU (%d)\r\n", sys_fd, vm_fd, vcpu_fd);

    initram(ram);

    readfile("app.bin", ram);

    arch_init();

	kvm_cpu__show_csrs();

	kvm_cpu__show_registers();

	printf("\r\nKVM RUN:\r\n\r\n");

    while(is_runing) {
        ioctl(vcpu_fd, KVM_RUN, 0);
		switch (runs->exit_reason) {
			case KVM_EXIT_MMIO:
				do_chrdev(runs->mmio.is_write, runs->mmio.data, runs->mmio.len);
				break;
			case KVM_EXIT_RISCV_SBI:
				emulate_sbi(runs->riscv_sbi.extension_id, runs->riscv_sbi.function_id, runs->riscv_sbi.args[0]);
				add_pc();
				break;
			case KVM_EXIT_SYSTEM_EVENT:
				is_runing = 0;
				break;
			default:
				printf("==========KVM_EXIT_UNKNOWN==========\r\n");
				printf("Exit Reason: %d\r\n", runs->exit_reason);
				kvm_cpu__show_csrs();
				kvm_cpu__show_registers();
				is_runing = 0;
				break;
		}
    }
	printf("\r\nKVM END\r\n");
	return 0;
}