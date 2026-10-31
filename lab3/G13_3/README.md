# G13_3 - Pintos（Lab 1–3）

Lab 1、Lab 2、Lab 3 的程式都在 `pintos/` 這份程式碼裡。

## Lab 1 - User Programs

- argument passing
- `exec` / `wait` / `exit` 同步
- user pointer 檢查
- file descriptor table
- 基本 system calls

主要檔案：`pintos/userprog/process.c`、`pintos/userprog/syscall.c`、`pintos/threads/thread.c`

## Lab 2 - Threads

- 不 busy wait 的 alarm clock
- priority scheduling 與 priority donation
- 依 priority 喚醒的 semaphore 與 condition variable
- MLFQS 與 fixed-point 運算

主要檔案：`pintos/devices/timer.c`、`pintos/threads/thread.c`、`pintos/threads/synch.c`、`pintos/threads/fixed-point.h`

## Lab 3 - Virtual Memory

- supplemental page table
- frame 配置、pinning、eviction
- swap in / swap out
- lazy loading 與 stack growth
- `mmap` / `munmap`

主要檔案：`pintos/vm/page.c`、`pintos/vm/frame.c`、`pintos/vm/swap.c`、`pintos/userprog/exception.c`、`pintos/userprog/process.c`

## 測試

```sh
cd pintos/threads          # Lab 2
export PATH="$PWD/../utils:$PATH"
make check

cd ../vm                   # Lab 1 與 Lab 3
make check
```

threads 與 vm（userprog、VM、mmap、filesys base）的測試全部通過。
