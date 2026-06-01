# 王凡_作業系統_IDSP_NTU_2025Spring_Operating System

這個 repository 是 113-2 作業系統課程作業整理。主要程式實作集中在
`lab3/G13_3/pintos`，因為 Lab 3 的 Pintos 測試會一起編譯 threads、
userprog、filesys、VM，所以 Lab 1 和 Lab 2 的修正也整合到這份 Lab 3
程式樹裡。

## 專案是做什麼的

這份專案是 Pintos 作業實作，內容包含：

- Lab 0：作業環境與基礎說明文件。
- Lab 1：User Program / System Call，包含 argument passing、process
  wait/exec/exit、file descriptor 與基本 system calls。
- Lab 2：Threads，包含 alarm clock、priority scheduling、priority
  donation、priority-aware synchronization，以及 MLFQS。
- Lab 3：Virtual Memory，包含 supplemental page table、frame table、
  swap、lazy loading、stack growth、`mmap` / `munmap`。

目前可執行、可測試的最終整合版本在：

```text
lab3/G13_3/pintos
```

## 主要資料夾結構

```text
.
├── lab0/
│   └── lab0.pdf
├── lab1/
│   ├── EE5173 lab01_ System Call.pdf
│   ├── G13_1.zip
│   └── G21_1.zip
├── lab2/
│   ├── threads.pptx.pdf
│   ├── G13_2/
│   └── G13_2.zip
├── lab3/
│   ├── EE5173 lab03_ Virtual Memory.pdf
│   ├── G13_3.zip
│   └── G13_3/
│       ├── README.md
│       └── pintos/
└── Midterm report/
```

## 從哪裡開始看

1. 先看這份最外層 `README.md`，確認每個 lab 的狀態。
2. 再看 `lab3/G13_3/README.md`，了解整合版 Pintos 做了哪些功能。
3. 看 Lab 1 實作時，從這些檔案開始：
   - `lab3/G13_3/pintos/userprog/process.c`
   - `lab3/G13_3/pintos/userprog/syscall.c`
4. 看 Lab 2 實作時，從這些檔案開始：
   - `lab3/G13_3/pintos/devices/timer.c`
   - `lab3/G13_3/pintos/threads/thread.c`
   - `lab3/G13_3/pintos/threads/synch.c`
   - `lab3/G13_3/pintos/threads/fixed-point.h`
5. 看 Lab 3 實作時，從這些檔案開始：
   - `lab3/G13_3/pintos/vm/page.c`
   - `lab3/G13_3/pintos/vm/frame.c`
   - `lab3/G13_3/pintos/vm/swap.c`
   - `lab3/G13_3/pintos/userprog/exception.c`
   - `lab3/G13_3/pintos/userprog/syscall.c`

## Build / Test

測試請在 Ubuntu / WSL 裡跑，並先安裝 QEMU：

```sh
sudo apt update
sudo apt install qemu-system-x86
```

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

## 目前驗證狀態

已在 `lab3/G13_3/pintos` 整合版中驗證：

- `pintos/threads/build make check`：alarm、priority scheduling、
  priority donation、MLFQS 測試皆 PASS。
- `pintos/vm make check`：userprog、VM、mmap、filesys base 測試皆 PASS。

## README 分工

- 這份最外層 README：整理整個 repository 的狀態、資料夾結構、閱讀順序與測試方式。
- 各 lab 內的 README：只簡短說明該 lab 資料夾內容，以及實作是否整合到
  `lab3/G13_3/pintos`。
