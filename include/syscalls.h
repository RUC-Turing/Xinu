#pragma once

#include "valen.h"

#define do_argless_syscall(rettype, id) \
		((rettype)do_syscall(id, 0))
#define do_generic_syscall(rettype, id, ...) \
		((rettype)do_syscall(id, va_args_len(__VA_ARGS__), __VA_ARGS__))

#define SYSCALL_CREATE		1
#define SYSCALL_RESUME		2
#define SYSCALL_RECVCLR		3
#define SYSCALL_RECEIVE		4
#define SYSCALL_SLEEPMS		5
#define SYSCALL_SLEEP		6
#define SYSCALL_FPRINTF		7
#define SYSCALL_PRINTF		8
#define SYSCALL_FSCANF		9
#define SYSCALL_READ		10
#define SYSCALL_OPEN		11
#define SYSCALL_CONTROL		12
#define SYSCALL_KILL		13
#define SYSCALL_GETPID		14
