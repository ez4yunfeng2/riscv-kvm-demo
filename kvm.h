
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <unistd.h>

#define pause getchar();
#define KVM_REG_RISCV		0x8000000000000000ULL
static inline __u64 __kvm_reg_id(__u64 type, __u64 subtype,
				__u64 idx, __u64  size)
{
	return KVM_REG_RISCV | type | subtype | idx | size;
}

#define KVM_REG_RISCV_SUBTYPE_SHIFT	16
#define KVM_REG_SIZE_ULONG	KVM_REG_SIZE_U64
#define KVM_REG_RISCV_CORE		(0x02 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_SBI_SINGLE	(0x0 << KVM_REG_RISCV_SUBTYPE_SHIFT)
#define KVM_REG_RISCV_SBI_EXT		(0x08 << KVM_REG_RISCV_TYPE_SHIFT)

#define KVM_REG_RISCV_CORE_REG(name)	\
		(offsetof(struct kvm_riscv_core, name) / sizeof(unsigned long))

#define RISCV_CORE_REG(name)	__kvm_reg_id(KVM_REG_RISCV_CORE, 0, \
					    KVM_REG_RISCV_CORE_REG(name), \
					    KVM_REG_SIZE_ULONG)

#define RISCV_CSR_REG(name)	__kvm_reg_id(KVM_REG_RISCV_CSR, 0, \
					     KVM_REG_RISCV_CSR_REG(name), \
					     KVM_REG_SIZE_ULONG)

#define RISCV_SBI_EXT_REG(subtype, id)	\
				__kvm_reg_id(KVM_REG_RISCV_SBI_EXT, subtype, \
					     id, KVM_REG_SIZE_ULONG)

#define KVM_REG_RISCV_SBI_MULTI_MASK(__ext_id)	\
		(1UL << ((__ext_id) % __BITS_PER_LONG))

static int sys_fd;
static int vm_fd;
static int vcpu_fd;
struct kvm_run *runs;
static int is_runing = 1;

void die(char *demo) {
	printf("[Error] %s\r\n", demo);
	_exit(-1);
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

void kvm_cpu__show_csrs()
{
	struct kvm_one_reg reg;
	struct kvm_riscv_csr csr;
	unsigned long data;


	reg.addr = (unsigned long)&data;
	printf("\n Control Status Registers:\n");
	printf(  " ------------------------\n");

	reg.id		= RISCV_CSR_REG(sstatus);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (sstatus)");
	csr.sstatus = data;

	reg.id		= RISCV_CSR_REG(sie);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (sie)");
	csr.sie = data;

	reg.id		= RISCV_CSR_REG(stvec);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (stvec)");
	csr.stvec = data;

	reg.id		= RISCV_CSR_REG(sip);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (sip)");
	csr.sip = data;

	reg.id		= RISCV_CSR_REG(satp);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (satp)");
	csr.satp = data;

	reg.id		= RISCV_CSR_REG(stval);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (stval)");
	csr.stval = data;

	reg.id		= RISCV_CSR_REG(scause);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (SCAUSE)");
	csr.scause = data;

	reg.id		= RISCV_CSR_REG(sscratch);
	if (ioctl(vcpu_fd, KVM_GET_ONE_REG, &reg) < 0)
		die("KVM_GET_ONE_REG failed (sscartch)");
	csr.sscratch = data;
	printf(" SSTATUS:  0x%016lx\n", csr.sstatus);
	printf(" SIE:      0x%016lx\n", csr.sie);
	printf(" STVEC:    0x%016lx\n", csr.stvec);
	printf(" SIP:      0x%016lx\n", csr.sip);
	printf(" SATP:     0x%016lx\n", csr.satp);
	printf(" STVAL:    0x%016lx\n", csr.stval);
	printf(" SCAUSE:   0x%016lx\n", csr.scause);
	printf(" SSCRATCH: 0x%016lx\n", csr.sscratch);
}