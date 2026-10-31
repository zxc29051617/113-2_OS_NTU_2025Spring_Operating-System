# Lab 3 - Virtual Memory

- `EE5173 lab03_ Virtual Memory.pdf`：題目說明
- `G13_3.zip`：繳交的程式
- `G13_3/`：Lab 1–3 整合後的 Pintos

## 實作內容

- supplemental page table
- frame table：配置、pinning、eviction
- swap in / swap out
- lazy loading
- stack growth
- `mmap` / `munmap`
- process 結束時清除 mapped files、frames 與 swap slots

主要檔案：

- `G13_3/pintos/vm/page.c`
- `G13_3/pintos/vm/frame.c`
- `G13_3/pintos/vm/swap.c`
- `G13_3/pintos/userprog/exception.c`
- `G13_3/pintos/userprog/syscall.c`
