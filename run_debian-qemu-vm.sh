#!/bin/bash
echo "Запуск Debian VM..."
qemu-system-x86_64 \
    -name "debian-qemu-vm" \
    -m 4G \
    -smp 2 \
    -drive file="debian-qemu-vm.qcow2",format=qcow2 \
    -cdrom "debian-13.3.0-amd64-netinst.iso" \
    -boot d \
    -netdev user,id=net0 \
    -device e1000,netdev=net0 \
    -vga std \
    -display gtk
