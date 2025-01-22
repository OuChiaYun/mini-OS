#!/bin/bash
set -xue

# QEMU 文件路径
QEMU=qemu-system-riscv32

# 启动 QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot


# -machine virt：启动一个 virt 机器。你可以用 -machine '?' 选项查看其他支持的机器。
# -bios default：使用默认固件（在本例中是 OpenSBI）。
# -nographic：启动 QEMU 时不使用 GUI 窗口。
# -serial mon:stdio：将 QEMU 的标准输入/输出连接到虚拟机的串行端口。指定 mon: 允许通过按下 Ctrl+A 然后 C 切换到 QEMU 监视器。
# --no-reboot：如果虚拟机崩溃，停止模拟器而不重启（对调试有用）。
