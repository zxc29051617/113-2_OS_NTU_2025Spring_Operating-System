# Pintos Labs 1-3 Integrated Submission

This directory is the integrated Pintos submission under `lab3/G13_3/pintos`.
It includes the work required by the Lab 1, Lab 2, and Lab 3 handouts, so this
is the tree to build and test instead of the older per-lab copies.

## Scope

### Lab 1 - User Programs

- Argument passing on the initial user stack
- Process exit messages and parent/child synchronization for `exec` and `wait`
- System call dispatch and user pointer validation
- File descriptor table management
- `exit`, `exec`, `wait`, `create`, `remove`, `open`, `filesize`, `read`,
  `write`, `seek`, `tell`, and `close`

### Lab 2 - Threads

- Non-busy-wait alarm clock using an ordered sleep list
- Priority scheduling with ordered ready and waiter lists
- Priority donation through locks, including nested donation
- Priority-aware semaphores and condition variables
- Multi-level feedback queue scheduler (MLFQS) with fixed-point arithmetic,
  `nice`, `recent_cpu`, and `load_avg`

### Lab 3 - Virtual Memory

- Supplemental page table management
- Frame table allocation, pinning, eviction, and write-back handling
- Swap-in and swap-out support
- Lazy loading for file-backed pages
- Stack growth handling
- `mmap` and `munmap` system calls
- Process cleanup for mapped files, frames, and swap slots

## Important Paths

- `pintos/threads/thread.c`, `pintos/threads/thread.h`: scheduler state,
  priority scheduling, MLFQS, and per-thread VM/mmap bookkeeping
- `pintos/threads/synch.c`, `pintos/threads/synch.h`: priority-aware
  semaphores, locks, donation, and condition variables
- `pintos/threads/fixed-point.h`: fixed-point helpers for MLFQS
- `pintos/devices/timer.c`: alarm clock sleep/wakeup implementation
- `pintos/userprog/process.c`: argument passing, process load, wait/exit
  cleanup, and per-process resource cleanup
- `pintos/userprog/syscall.c`, `pintos/userprog/syscall.h`: system calls,
  VM-aware pointer checks, `mmap`, and `munmap`
- `pintos/vm/page.c`, `pintos/vm/page.h`: supplemental page table and page claim logic
- `pintos/vm/frame.c`, `pintos/vm/frame.h`: frame allocation, eviction, pinning, and unpinning
- `pintos/vm/swap.c`, `pintos/vm/swap.h`: swap slot management
- `pintos/userprog/exception.c`: page fault and stack growth handling

## Setup

Run the tests from Ubuntu/WSL. QEMU is required:

```sh
sudo apt update
sudo apt install qemu-system-x86
```

Add Pintos utilities to `PATH` before building or testing:

```sh
cd lab3/G13_3/pintos
export PATH="$PWD/utils:$PATH"
```

## Build

Threads build:

```sh
cd lab3/G13_3/pintos/threads
make
```

Integrated userprog/VM build:

```sh
cd lab3/G13_3/pintos/vm
make
```

## Test

Lab 2 threads tests:

```sh
cd lab3/G13_3/pintos/threads
export PATH="$PWD/../utils:$PATH"
make check
```

Lab 1 and Lab 3 integrated regression tests:

```sh
cd lab3/G13_3/pintos/vm
export PATH="$PWD/../utils:$PATH"
make check
```

The current implementation was verified with:

- `make check` from `lab3/G13_3/pintos/threads`: all listed alarm, priority
  donation, and MLFQS tests printed PASS.
- `make check` from `lab3/G13_3/pintos/vm`: all listed userprog, VM, mmap, and
  filesys base tests printed PASS.

## Notes

- Lab 1 and Lab 2 fixes are intentionally integrated into this Lab 3 tree,
  because the Lab 3 test target builds userprog, threads, filesys, and VM
  together.
- Non-`mmap` writable file-backed pages are swapped out on eviction so private
  process data is preserved.
- Dirty `mmap` pages are written back to the mapped file on eviction/unmap.
- Stack growth is limited to faults near the current user stack pointer and the
  configured maximum stack size.
- `exec` load synchronization uses a per-child load semaphore, separate from
  `wait` exit synchronization.
