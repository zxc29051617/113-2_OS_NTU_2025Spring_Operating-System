# G13_3 - Integrated Pintos Implementation

這個資料夾是目前最終整合版 Pintos。Lab 1、Lab 2、Lab 3 的程式修正都放在
`pintos/` 這份 codebase 裡。

## 做了哪些事

### Lab 1 - User Programs

- argument passing
- `exec` / `wait` / `exit` synchronization
- user pointer validation
- file descriptor table
- basic system calls

主要檔案：

- `pintos/userprog/process.c`
- `pintos/userprog/syscall.c`
- `pintos/threads/thread.c`

### Lab 2 - Threads

- non-busy-wait alarm clock
- priority scheduling
- priority donation
- priority-aware semaphore / condition variable
- MLFQS and fixed-point arithmetic

主要檔案：

- `pintos/devices/timer.c`
- `pintos/threads/thread.c`
- `pintos/threads/thread.h`
- `pintos/threads/synch.c`
- `pintos/threads/synch.h`
- `pintos/threads/fixed-point.h`

### Lab 3 - Virtual Memory

- supplemental page table
- frame allocation, pinning, eviction
- swap in / swap out
- lazy loading
- stack growth
- `mmap` / `munmap`

主要檔案：

- `pintos/vm/page.c`
- `pintos/vm/frame.c`
- `pintos/vm/swap.c`
- `pintos/userprog/exception.c`
- `pintos/userprog/syscall.c`
- `pintos/userprog/process.c`

## 測試方式

Lab 2 threads tests：

```sh
cd lab3/G13_3/pintos/threads
export PATH="$PWD/../utils:$PATH"
make check
```

Lab 1 + Lab 3 integrated tests：

```sh
cd lab3/G13_3/pintos/vm
export PATH="$PWD/../utils:$PATH"
make check
```

## 驗證狀態

- `pintos/threads/build make check`：全部 listed threads tests PASS。
- `pintos/vm make check`：全部 listed userprog、VM、mmap、filesys base tests PASS。
