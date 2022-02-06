#!/bin/bash

if ! command -v qemu-system-i386 &>/dev/null; then
	echo Command qemu-system-i386 not found!
	exit 1
fi

cd "$(dirname "$0")"

qemu-system-i386 -s -S -nographic -monitor mon:stdio -serial telnet:127.0.0.1:1235,server -kernel compile/xinu.elf

GDB_PID="$(cat .gdb.pid)"
pkill -P "$GDB_PID"
