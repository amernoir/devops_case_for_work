#!/bin/bash
echo "=== АВТОМАТИЗИРОВАННОЕ СОЗДАНИЕ DEBIAN VM В QEMU ==="

# Параметры
VM_NAME="debian-qemu-vm"
DISK_SIZE="20G"
MEMORY="4G"
CPUS="2"

# 1. Скачиваем Debian ISO если нет
ISO_FILE="debian-12.5.0-amd64-netinst.iso"
if [ ! -f "$ISO_FILE" ]; then
    echo "Скачиваем Debian..."
    wget -q --show-progress \
        https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.5.0-amd64-netinst.iso
fi

echo "Создаем виртуальный диск..."
qemu-img create -f qcow2 "${VM_NAME}.qcow2" "$DISK_SIZE"

cat > "${VM_NAME}.conf" << CONFIG
# Конфигурация VM: $VM_NAME
# Создана: $(date)

память = $MEMORY
cpu = $CPUS ядра
диск = $DISK_SIZE
образ = $ISO_FILE

графический = qemu-system-x86_64 -name "$VM_NAME" -m $MEMORY -smp $CPUS -drive file="${VM_NAME}.qcow2",format=qcow2 -cdrom "$ISO_FILE" -boot d -vga std -display gtk
headless = qemu-system-x86_64 -name "$VM_NAME" -m $MEMORY -smp $CPUS -drive file="${VM_NAME}.qcow2",format=qcow2 -cdrom "$ISO_FILE" -boot d -vnc :0 -display none -daemonize
CONFIG

cat > "run_${VM_NAME}.sh" << 'SCRIPT'
#!/bin/bash
echo "Запуск Debian VM..."
qemu-system-x86_64 \
    -name "debian-qemu-vm" \
    -m 4G \
    -smp 2 \
    -drive file="debian-qemu-vm.qcow2",format=qcow2 \
    -cdrom "debian-12.5.0-amd64-netinst.iso" \
    -boot d \
    -netdev user,id=net0 \
    -device e1000,netdev=net0 \
    -vga std \
    -display gtk
SCRIPT

chmod +x "run_${VM_NAME}.sh"

echo ""
echo "VM создана!"
echo "Запустите: ./run_${VM_NAME}.sh"
