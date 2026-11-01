# NTU Operating System 2025 Spring

這個 repository 收錄臺大 113-2 作業系統（王凡老師）課程的 Pintos 實作與期中報告。

Pintos 的 Lab 1 到 Lab 3 是第 13 組（G13）的分組作業，在同一份程式上逐步完成，三個 lab 的最終程式整合在
`lab3/G13_3/pintos`。

## 專案結構

```text
.
├── README.md
├── lab0/                  # Pintos 環境說明
├── lab1/                  # User Program 與 System Call
├── lab2/                  # Threads
├── lab3/                  # Virtual Memory
│   └── G13_3/pintos/      # Lab 1–3 整合後的 Pintos
└── Midterm report/        # 期中報告：作業系統與 AI（AIOS）
```

## Lab 0：環境

題目說明：[`lab0/lab0.pdf`](lab0/lab0.pdf)

Pintos 開發環境的建置與基本操作。

## Lab 1：User Program 與 System Call

題目說明：[`lab1/EE5173 lab01_ System Call.pdf`](<lab1/EE5173 lab01_ System Call.pdf>)

- argument passing
- `exec` / `wait` / `exit` 的同步與 process 清理
- user pointer 檢查
- file descriptor table
- system calls：`create`、`remove`、`open`、`filesize`、`read`、`write`、`seek`、`tell`、`close` 等

主要檔案：`userprog/process.c`、`userprog/syscall.c`、`threads/thread.c`

## Lab 2：Threads

題目說明：[`lab2/threads.pptx.pdf`](lab2/threads.pptx.pdf)  
報告：[`lab2/G13_2/`](lab2/G13_2)

- alarm clock：用 sleep list 取代 busy waiting
- priority scheduling 與 priority donation（含 nested donation）
- semaphore、condition variable 依 priority 喚醒
- MLFQS：`nice`、`recent_cpu`、`load_avg` 與 fixed-point 運算

主要檔案：`devices/timer.c`、`threads/thread.c`、`threads/synch.c`、`threads/fixed-point.h`

## Lab 3：Virtual Memory

題目說明：[`lab3/EE5173 lab03_ Virtual Memory.pdf`](<lab3/EE5173 lab03_ Virtual Memory.pdf>)

- supplemental page table
- frame table：配置、pinning、eviction
- swap in / swap out
- lazy loading 與 stack growth
- `mmap` / `munmap`

主要檔案：`vm/page.c`、`vm/frame.c`、`vm/swap.c`、`userprog/exception.c`

## 期中報告

[`Midterm report/`](<Midterm report>)：作業系統與 AI 的期中報告投影片、錄影，以及參考的 AIOS 論文。

## 編譯與測試

在 Ubuntu 或 WSL 上安裝 QEMU：

```sh
sudo apt install qemu-system-x86
```

Lab 2 的 threads 測試：

```sh
cd lab3/G13_3/pintos/threads
export PATH="$PWD/../utils:$PATH"
make check
```

Lab 1 與 Lab 3 的測試：

```sh
cd lab3/G13_3/pintos/vm
export PATH="$PWD/../utils:$PATH"
make check
```
