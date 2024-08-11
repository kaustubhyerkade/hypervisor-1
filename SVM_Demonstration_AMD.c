//C program to demonstrate SVM instruction set 
// low level CPU instructions , to be tested in VM only 



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/msr.h>
#include <asm/processor.h>
#include <asm/svm.h>

// Enable SVM in EFER MSR
static void enable_svm(void) {
    uint64_t efer;
    rdmsrl(MSR_EFER, efer);
    efer |= EFER_SVME;  // Set the SVME (Secure Virtual Machine Enable) bit
    wrmsrl(MSR_EFER, efer);
}

// Disable SVM in EFER MSR
static void disable_svm(void) {
    uint64_t efer;
    rdmsrl(MSR_EFER, efer);
    efer &= ~EFER_SVME;  // Clear the SVME bit
    wrmsrl(MSR_EFER, efer);
}

// Example: Execute VMRUN instruction
static void execute_vmrun(uint64_t *vmcb) {
    __asm__ __volatile__("vmrun %0" : : "a"(vmcb) : "memory");  //vmrun sarts the execution of a virtual machine (guest) by loading its state from the VMCB (Virtual Machine Control Block).
}

// Example: Execute VMLOAD instruction
static void execute_vmload(uint64_t *vmcb) {
    __asm__ __volatile__("vmload %0" : : "a"(vmcb) : "memory"); //vmload loads the hypervisor's (host) state from the VMCB
}

// Example: Execute VMSAVE instruction
static void execute_vmsave(uint64_t *vmcb) {
    __asm__ __volatile__("vmsave %0" : : "a"(vmcb) : "memory"); //vmsave saves the current hypervisor state into the VMCB before transferring control to the guest or after a VMEXIT.
}

static int __init svm_example_init(void) {
    printk(KERN_INFO "Loading SVM example module...\n");

    // Enable SVM
    enable_svm();

    // Allocate memory for a VMCB (Virtual Machine Control Block)
    uint64_t *vmcb = (uint64_t *)kmalloc(4096, GFP_KERNEL);  //he VMCB is allocated using kmalloc and must be properly initialized. This block is used to store the state of the virtual machine or hypervisor.
    if (!vmcb) {
        printk(KERN_ERR "Failed to allocate memory for VMCB\n");
        return -ENOMEM;
    }

    // Initialize the VMCB with zeros
    memset(vmcb, 0, 4096);

    // Execute VMLOAD to load the host state
    execute_vmload(vmcb);

    // Example: Execute VMRUN to start the guest VM (dummy example)
    execute_vmrun(vmcb);

    // Execute VMSAVE to save the host state
    execute_vmsave(vmcb);

    // Free the VMCB memory
    kfree(vmcb);

    // Disable SVM after usage
    disable_svm();

    printk(KERN_INFO "SVM example module loaded successfully.\n");
    return 0;
}

static void __exit svm_example_exit(void) {
    printk(KERN_INFO "Unloading SVM example module...\n");
}
