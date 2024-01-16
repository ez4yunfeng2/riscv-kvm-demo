#include "kvm.h"
#include <stdio.h>
#define pause getchar();

static int sys_fd;
static int vm_fd;
static int vcpu_fd;
struct kvm_run *runs;
void console(int func, int arg) {
    if( func == 0 ) {
        printf("%c",arg);
    } else {
        printf("[Error] Bad Function ID\r\n");
    }
}

void initram(void *ram) {
    struct kvm_userspace_memory_region mem = {
        .slot = 0,
        .flags = 0,
        .guest_phys_addr = 0,
        .memory_size = 1024 * 1024 * 100,
        .userspace_addr = (uint64_t)ram,
    };
    int ret = ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &mem);
    printf("KVM_SET_USER_MEMORY_REGION (%d)\r\n", ret);
}

// func readfile Open file and compute file size and Write it to buffer
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

}

void die(char *demo) {

}

void kvm_cpu__show_registers()
{
	struct kvm_one_reg reg;
	unsigned long data;
	struct kvm_riscv_core core;

	reg.addr = (unsigned long)&data;

	reg.id		= RISCV_CORE_REG(mode);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (mode)");
	core.mode = data;

	reg.id		= RISCV_CORE_REG(regs.pc);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (pc)");
	core.regs.pc = data;

	reg.id		= RISCV_CORE_REG(regs.ra);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (ra)");
	core.regs.ra = data;

	reg.id		= RISCV_CORE_REG(regs.sp);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (sp)");
	core.regs.sp = data;

	reg.id		= RISCV_CORE_REG(regs.gp);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (gp)");
	core.regs.gp = data;

	reg.id		= RISCV_CORE_REG(regs.tp);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (tp)");
	core.regs.tp = data;

	reg.id		= RISCV_CORE_REG(regs.t0);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t0)");
	core.regs.t0 = data;

	reg.id		= RISCV_CORE_REG(regs.t1);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t1)");
	core.regs.t1 = data;

	reg.id		= RISCV_CORE_REG(regs.t2);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t2)");
	core.regs.t2 = data;

	reg.id		= RISCV_CORE_REG(regs.s0);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s0)");
	core.regs.s0 = data;

	reg.id		= RISCV_CORE_REG(regs.s1);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s1)");
	core.regs.s1 = data;

	reg.id		= RISCV_CORE_REG(regs.a0);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a0)");
	core.regs.a0 = data;

	reg.id		= RISCV_CORE_REG(regs.a1);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a1)");
	core.regs.a1 = data;

	reg.id		= RISCV_CORE_REG(regs.a2);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a2)");
	core.regs.a2 = data;

	reg.id		= RISCV_CORE_REG(regs.a3);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a3)");
	core.regs.a3 = data;

	reg.id		= RISCV_CORE_REG(regs.a4);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a4)");
	core.regs.a4 = data;

	reg.id		= RISCV_CORE_REG(regs.a5);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a5)");
	core.regs.a5 = data;

	reg.id		= RISCV_CORE_REG(regs.a6);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a6)");
	core.regs.a6 = data;

	reg.id		= RISCV_CORE_REG(regs.a7);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (a7)");
	core.regs.a7 = data;

	reg.id		= RISCV_CORE_REG(regs.s2);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s2)");
	core.regs.s2 = data;

	reg.id		= RISCV_CORE_REG(regs.s3);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s3)");
	core.regs.s3 = data;

	reg.id		= RISCV_CORE_REG(regs.s4);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s4)");
	core.regs.s4 = data;

	reg.id		= RISCV_CORE_REG(regs.s5);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s5)");
	core.regs.s5 = data;

	reg.id		= RISCV_CORE_REG(regs.s6);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s6)");
	core.regs.s6 = data;

	reg.id		= RISCV_CORE_REG(regs.s7);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s7)");
	core.regs.s7 = data;

	reg.id		= RISCV_CORE_REG(regs.s8);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s8)");
	core.regs.s8 = data;

	reg.id		= RISCV_CORE_REG(regs.s9);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s9)");
	core.regs.s9 = data;

	reg.id		= RISCV_CORE_REG(regs.s10);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s10)");
	core.regs.s10 = data;

	reg.id		= RISCV_CORE_REG(regs.s11);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (s11)");
	core.regs.s11 = data;

	reg.id		= RISCV_CORE_REG(regs.t3);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t3)");
	core.regs.t3 = data;

	reg.id		= RISCV_CORE_REG(regs.t4);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t4)");
	core.regs.t4 = data;

	reg.id		= RISCV_CORE_REG(regs.t5);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t5)");
	core.regs.t5 = data;

	reg.id		= RISCV_CORE_REG(regs.t6);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (t6)");
	core.regs.t6 = data;

	printf("\n General Purpose Registers:\n");
	printf(  " -------------------------\n");
	printf(" MODE:  0x%lx\n", data);
	printf(" PC: 0x%016lx   RA: 0x%016lx SP: 0x%016lx GP: 0x%016lx\n",
		core.regs.pc, core.regs.ra, core.regs.sp, core.regs.gp);
	printf(" TP: 0x%016lx   T0: 0x%016lx T1: 0x%016lx T2: 0x%016lx\n",
		core.regs.tp, core.regs.t0, core.regs.t1, core.regs.t2);
	printf(" S0: 0x%016lx   S1: 0x%016lx A0: 0x%016lx A1: 0x%016lx\n",
		core.regs.s0, core.regs.s1, core.regs.a0, core.regs.a1);
	printf(" A2: 0x%016lx   A3: 0x%016lx A4: 0x%016lx A5: 0x%016lx\n",
		core.regs.a2, core.regs.a3, core.regs.a4, core.regs.a5);
	printf(" A6: 0x%016lx   A7: 0x%016lx S2: 0x%016lx S3: 0x%016lx\n",
		core.regs.a6, core.regs.a7, core.regs.s2, core.regs.s3);
	printf(" S4: 0x%016lx   S5: 0x%016lx S6: 0x%016lx S7: 0x%016lx\n",
		core.regs.s4, core.regs.s5, core.regs.s6, core.regs.s7);
	printf(" S8: 0x%016lx   S9: 0x%016lx S10: 0x%016lx S11: 0x%016lx\n",
		core.regs.s8, core.regs.s9, core.regs.s10, core.regs.s11);
	printf(" T3: 0x%016lx   T4: 0x%016lx T5: 0x%016lx T6: 0x%016lx\n",
		core.regs.t3, core.regs.t4, core.regs.t5, core.regs.t6);
}

void add_pc() {
	struct kvm_one_reg reg;
	unsigned long epc;
	reg.id		= RISCV_CORE_REG(regs.pc);
	reg.addr	= &epc;
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (pc)");
	
	printf("epc: 0x%lx\r\n", epc);
	epc += 2;
	reg.id		= RISCV_CORE_REG(regs.pc);
	reg.addr	= &epc;
	int ret = ioctl(vcpu_fd, KVM_SET_ONE_REG, &reg);
	printf("KVM_SET_ONE_REG (%d)\r\n", ret);

	ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg);
	printf("Set Result epc: 0x%lx\r\n", epc);
}

int main(void) {

    uint8_t* ram = mmap(NULL, 1024 * 1024 * 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    sys_fd = open("/dev/kvm", O_RDWR);
    vm_fd  = ioctl(sys_fd, KVM_CREATE_VM, 0);
    vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);

    int mmap_size = ioctl(sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);

    runs = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu_fd, 0);

    printf("%d\r\n", mmap_size);

    printf("KVM (%d), VMDF (%d), VCPU (%d)\r\n", sys_fd, vm_fd, vcpu_fd);

    initram(ram);

    readfile("demo.bin", ram);
    arch_init();

	printf("Start Reason: %d\r\n", runs->exit_reason);
    while(1) {
        ioctl(vcpu_fd, KVM_RUN, 0);
		switch (runs->exit_reason) {
			case KVM_EXIT_MMIO:
				//printf("==========KVM_EXIT_MMIO==========\r\n");
				// dump runs.mmio
				//printf("PhyAddr: 0x%x\r\n", runs->mmio.phys_addr);
				int i;
				if (runs->mmio.is_write)
					printf("%c",runs->mmio.data[0]);

				//kvm_cpu__show_registers();
				// add_pc();
				//printf("================================\r\n");
				break;
			default:
				kvm_cpu__show_registers();
				break;
		}
        // printf("Exit Reason: %d\r\n", runs->exit_reason);
        // getchar();
    }

    return 0;
}