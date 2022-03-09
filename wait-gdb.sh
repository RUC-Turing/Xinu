#!/bin/bash

cd "$(dirname "$0")"/compile

GDB="gdb"
if [[ "$(uname)" == "Darwin" ]]; then
	GDB="x86_64-elf-gdb"
fi

if ! command -v "$GDB" &>/dev/null; then
	echo Command "$GDB" not found!
	exit 1
fi

echo "$$" > ../.gdb.pid
while true; do
	echo Waiting for QEMU debug server...
	"$GDB" -q -x ../.gdbrc
	sleep 0.5
done
