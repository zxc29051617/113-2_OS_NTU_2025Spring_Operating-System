# Lab 3 - Pintos Virtual Memory

This submission implements and fixes the Pintos virtual memory work under
`lab3/G13_3/pintos`.

## Scope

- Supplemental page table management
- Frame table allocation, pinning, eviction, and write-back handling
- Swap-in and swap-out support
- Lazy loading for file-backed pages
- Stack growth handling
- `mmap` and `munmap` system calls
- Process cleanup for mapped files, frames, and swap slots

## Important Paths

- `pintos/vm/page.c`, `pintos/vm/page.h`: supplemental page table and page claim logic
- `pintos/vm/frame.c`, `pintos/vm/frame.h`: frame allocation, eviction, pinning, and unpinning
- `pintos/vm/swap.c`, `pintos/vm/swap.h`: swap slot management
- `pintos/userprog/exception.c`: page fault and stack growth handling
- `pintos/userprog/syscall.c`, `pintos/userprog/syscall.h`: VM-aware pointer checks, `mmap`, `munmap`
- `pintos/userprog/process.c`: process load/exit cleanup
- `pintos/threads/thread.c`, `pintos/threads/thread.h`: per-thread VM and mmap bookkeeping

## Setup

Run the tests from Ubuntu/WSL. QEMU is required:

```sh
sudo apt update
sudo apt install qemu-system-x86
```

Add Pintos utilities to `PATH` before building or testing:

```sh
cd lab3/G13_3/pintos/vm
export PATH="$PWD/../utils:$PATH"
```

## Build

```sh
make
```

## Test

```sh
make check
```

The current implementation was verified with `make check` from
`lab3/G13_3/pintos/vm`. The listed userprog, VM, mmap, and filesys base tests
passed.

## Notes

- Non-`mmap` writable file-backed pages are swapped out on eviction so private
  process data is preserved.
- Dirty `mmap` pages are written back to the mapped file on eviction/unmap.
- Stack growth is limited to faults near the current user stack pointer and the
  configured maximum stack size.
- `exec` load synchronization uses a per-child load semaphore, separate from
  `wait` exit synchronization.
