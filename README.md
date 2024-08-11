# hypervisor-1

# Virtual Machine Control Structure (VMCS)

The **Virtual Machine Control Structure (VMCS)** is a key component in hardware-assisted virtualization technologies, such as Intel's VT-x and AMD's AMD-V. It plays a crucial role in managing and controlling the execution of virtual machines (VMs) on a physical processor. 

#### What is VMCS?

-   **Definition**: VMCS is a data structure used by the hypervisor (or Virtual Machine Monitor, VMM) to manage the state transitions between the host (hypervisor) and guest (VM) environments.
-   **Purpose**: It helps in storing and managing the state information of VMs, ensuring efficient switching between the host and guest states.

#### Components of VMCS:

The VMCS contains several fields that are divided into different categories:

1.  **Guest-State Area**:
    
    -   **Registers**: Stores the state of the guest CPU registers.
    -   **Control Registers**: Includes CR0, CR3, and CR4, which control the operating mode and memory management.
    -   **Segment Registers**: Contains the segment selectors, base addresses, and limits for the guest.
2.  **Host-State Area**:
    
    -   **Registers**: Stores the state of the host CPU registers that need to be restored when exiting to the host.
    -   **Control Registers**: Includes the host's CR0, CR3, and CR4 values.
    -   **Segment Registers**: Contains the segment selectors, base addresses, and limits for the host.
3.  **VM-Execution Control Fields**:
    
    -   Controls that configure how the VM runs, including which instructions cause VM exits and which hardware features are available to the guest.
4.  **VM-Exit Control Fields**:
    
    -   Controls that determine the behavior of the hypervisor when a VM exit occurs (e.g., handling of certain instructions, interrupts).
5.  **VM-Entry Control Fields**:
    
    -   Controls that determine the behavior when entering a VM (e.g., setting initial state, event injection).
6.  **VM-Exit Information Fields**:
    
    -   Contains information about the VM exit, such as the reason for the exit and relevant data for handling it.

#### How VMCS Works:

1.  **Initialization**:
    
    -   The hypervisor initializes the VMCS for each VM, setting up the guest and host state areas, as well as the control fields.
2.  **VM Entry**:
    
    -   When a VM is launched, the processor uses the VMCS to load the guest state from the VMCS into the physical CPU, effectively switching from the host to the guest environment.
3.  **VM Execution**:
    
    -   The VM runs as if it were on a physical machine, but under the control of the hypervisor.
    -   Certain operations or events (e.g., privileged instructions, interrupts) can trigger a VM exit.
4.  **VM Exit**:
    
    -   On a VM exit, the processor saves the guest state back into the VMCS and loads the host state from the VMCS, switching control back to the hypervisor.
5.  **Handling VM Exits**:
    
    -   The hypervisor examines the exit reason and uses the information in the VM-Exit Information Fields to handle the exit appropriately.

#### Benefits of VMCS:

-   **Performance**: Efficiently manages state transitions between VMs and the hypervisor, minimizing overhead.
-   **Control**: Provides fine-grained control over the execution of VMs and their interactions with hardware.
-   **Isolation**: Ensures strong isolation between VMs by managing their states independently.











AMD -V SVM instruction set - 

VMRUN - 
transitions the CPU from the host mode to the guest mode

VMLOAD -

loads the VMCB from memory into the processor
VMSAVE - saves the current VMCB state from the processor to memory.
CLGI - prevent interrupts from being delivered to the processor.
STGI - sets the Global Interrupt Flag, allowing interrupts to be delivered to the processor.
VMMCALL - used by guest operating systems to invoke services or functions provided by the hypervisor.
INVLPGA - invalidates TLB(Translation lookaside buffer) entries associated with a specific guest physical address and an ASID (Address Space Identifier).
SKINIT - initializes the secure execution environment by setting up necessary states and configurations for secure execution.


