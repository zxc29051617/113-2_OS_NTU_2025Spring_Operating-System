# Lab 3 - Virtual Memory

這個資料夾保存 Lab 3 的作業 PDF、原始提交壓縮檔，以及目前最終整合版 Pintos。

## 內容

- `EE5173 lab03_ Virtual Memory.pdf`：Lab 3 任務說明。
- `G13_3.zip`：過去的提交壓縮檔。
- `G13_3/`：目前整理後的整合版 Pintos 程式與 README。

## 做了哪些事

Lab 3 的最終實作在 `G13_3/pintos`，主要包含：

- supplemental page table
- frame table allocation、pinning、eviction
- swap in / swap out
- lazy loading
- stack growth
- `mmap` / `munmap`
- process exit 時清理 mapped files、frames、swap slots

Lab 1 與 Lab 2 的最終修正也整合在這份 `G13_3/pintos` 中，方便直接跑完整 Pintos 測試。

## 從哪裡看

- `G13_3/README.md`
- `G13_3/pintos/vm/page.c`
- `G13_3/pintos/vm/frame.c`
- `G13_3/pintos/vm/swap.c`
- `G13_3/pintos/userprog/exception.c`
- `G13_3/pintos/userprog/syscall.c`
