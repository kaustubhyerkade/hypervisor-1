//program to demonstrates the VMXON instructions || 
//caution - directly working with CPU virtualization features at low level, to be tested on VMs only. ring 0
//precheck - CPU(Intel) should support VMX & VMX to be enabled.  
//


#include <iostream>
#include <cstdint>
#include <cstring>  // for memset

// CPUID function to check if VMX is supported
bool isVMXSupported() {
    uint32_t eax, ebx, ecx, edx;

    __asm__ __volatile__ (
        "cpuid" 
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) //VMX support is indicated by bit 5 in the ECX register when the cpuid instruction is called with EAX set to 1.
        : "a"(1)
    );

    return (ecx & (1 << 5)) != 0;  // Check the VMX bit (bit 5) in ECX
}

// Function to enable VMX in CR4
void enableVMX() {
    uint64_t cr4;
    
    // Read CR4
    __asm__ __volatile__ (
        "mov %%cr4, %0"
        : "=r"(cr4) 
    );
    
    // Set VMX enable bit (bit 13)
    cr4 |= (1 << 13); 
    //sets the VMX enable bit in the CR4 control register. This is necessary before executing VMX instructions like VMXON.
    // Write back to CR4
    __asm__ __volatile__ (
        "mov %0, %%cr4"
        : : "r"(cr4)
    );
}

// Function to execute VMXON
bool executeVMXON(uint64_t* vmxonRegion) {
    uint8_t status;
    
    __asm__ __volatile__ (
        "vmxon %1; setna %0"
        : "=r"(status)
        : "m"(*vmxonRegion)  //executes the VMXON instruction using inline assembly. It takes the address of the VMXON region as input. The VMXON instruction enables VMX operation.
        : "cc"  // condition codes are modified
    );

    return status == 0;
}

int main() {
    // Check if VMX is supported , & enable CR4 , allocate 4 KB memory block of vmxon region. 
    if (!isVMXSupported()) {
        std::cerr << "VMX is not supported on this CPU." << std::endl;
        return -1;
    }

    // Enable VMX in CR4
    enableVMX();

    // Allocate aligned memory for the VMXON region (4KB aligned)
    uint64_t* vmxonRegion = reinterpret_cast<uint64_t*>(aligned_alloc(4096, 4096));
    if (!vmxonRegion) {
        std::cerr << "Failed to allocate memory for VMXON region." << std::endl;
        return -1;
    }

    // Clear the VMXON region and set the VMCS revision identifier (should be obtained from IA32_VMX_BASIC MSR)
    memset(vmxonRegion, 0, 4096);

    // Assuming revision ID is 0x1 (for demonstration purposes, it should be obtained from MSR)
    uint32_t revisionId = 0x1;
    *vmxonRegion = revisionId;

    // Execute VMXON
    if (executeVMXON(vmxonRegion)) {
        std::cout << "VMXON executed successfully." << std::endl;
    } else {
        std::cerr << "Failed to execute VMXON." << std::endl;
    }

    // Clean up
    free(vmxonRegion);

    return 0;
}
