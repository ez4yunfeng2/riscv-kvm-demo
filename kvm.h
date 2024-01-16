
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <unistd.h>


#define KVM_REG_RISCV		0x8000000000000000ULL
static inline __u64 __kvm_reg_id(__u64 type, __u64 subtype,
				__u64 idx, __u64  size)
{
	return KVM_REG_RISCV | type | subtype | idx | size;
}
#define KVM_REG_SIZE_ULONG	KVM_REG_SIZE_U64
#define KVM_REG_RISCV_CORE		(0x02 << KVM_REG_RISCV_TYPE_SHIFT)
#define KVM_REG_RISCV_CORE_REG(name)	\
		(offsetof(struct kvm_riscv_core, name) / sizeof(unsigned long))
#define RISCV_CORE_REG(name)	__kvm_reg_id(KVM_REG_RISCV_CORE, 0, \
					    KVM_REG_RISCV_CORE_REG(name), \
					    KVM_REG_SIZE_ULONG)