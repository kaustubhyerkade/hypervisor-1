//create a VM using libvert library

#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {
    virConnectPtr conn;
    virDomainPtr dom;
    char *xml;

    // Connect to the hypervisor
    conn = virConnectOpen("qemu:///system");
    if (conn == NULL) {
        fprintf(stderr, "Failed to connect to the hypervisor\n");
        return 1;
    }

    // Define the XML for the new virtual machine
    xml = "<domain type='kvm'>"
          "  <name>test-vm</name>"
          "  <memory unit='KiB'>1048576</memory>"   // 1 GB memory
          "  <vcpu placement='static'>1</vcpu>"     // 1 vCPU
          "  <os>"
          "    <type arch='x86_64' machine='pc-i440fx-2.9'>hvm</type>"
          "    <boot dev='hd'/>"
          "  </os>"
          "  <devices>"
          "    <disk type='file' device='disk'>"
          "      <driver name='qemu' type='qcow2'/>"
          "      <source file='/var/lib/libvirt/images/test-vm.qcow2'/>"
          "      <target dev='vda' bus='virtio'/>"
          "      <address type='pci' domain='0x0000' bus='0x00' slot='0x04' function='0x0'/>"
          "    </disk>"
          "    <interface type='network'>"
          "      <mac address='52:54:00:6b:3c:58'/>"
          "      <source network='default'/>"
          "      <model type='virtio'/>"
          "      <address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>"
          "    </interface>"
          "  </devices>"
          "</domain>";

    // Create the virtual machine
    dom = virDomainCreateXML(conn, xml, 0);
    if (dom == NULL) {
        fprintf(stderr, "Failed to create the domain\n");
        virConnectClose(conn);
        return 1;
    }

    printf("Virtual machine created successfully\n");

    // Start the VM
    if (virDomainCreate(dom) < 0) {
        fprintf(stderr, "Failed to start the domain\n");
        virDomainFree(dom);
        virConnectClose(conn);
        return 1;
    }

    printf("Virtual machine started successfully\n");

    // Clean up
    virDomainFree(dom);
    virConnectClose(conn);

    return 0;
}
