# Lab 2 - Threads

這個資料夾保存 Lab 2 的作業 PDF、報告資料與原始提交壓縮檔。

## 內容

- `threads.pptx.pdf`：Lab 2 任務說明。
- `G13_2/`：Lab 2 報告與當時資料。
- `G13_2.zip`：過去的提交壓縮檔。

## 做了哪些事

Lab 2 的最終實作已整合到 `lab3/G13_3/pintos`，主要包含：

- alarm clock：使用 sleep list，避免 busy waiting
- priority scheduling：ready list 依 priority 排序
- priority donation：支援 lock donation 與 nested donation
- synchronization：semaphore / condition variable 依 priority 喚醒
- MLFQS：實作 `nice`、`recent_cpu`、`load_avg` 與 fixed-point arithmetic

## 從哪裡看

- `lab3/G13_3/pintos/devices/timer.c`
- `lab3/G13_3/pintos/threads/thread.c`
- `lab3/G13_3/pintos/threads/thread.h`
- `lab3/G13_3/pintos/threads/synch.c`
- `lab3/G13_3/pintos/threads/synch.h`
- `lab3/G13_3/pintos/threads/fixed-point.h`
