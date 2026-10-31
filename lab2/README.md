# Lab 2 - Threads

- `threads.pptx.pdf`：題目說明
- `G13_2/G13_2.pdf`：報告
- `G13_2.zip`：繳交的程式

## 實作內容

- alarm clock：用 sleep list 取代 busy waiting
- priority scheduling：ready list 依 priority 排序
- priority donation：支援 lock donation 與 nested donation
- synchronization：semaphore 與 condition variable 依 priority 喚醒
- MLFQS：`nice`、`recent_cpu`、`load_avg` 與 fixed-point 運算

最終程式整合在 `lab3/G13_3/pintos`，主要檔案：

- `devices/timer.c`
- `threads/thread.c`、`threads/thread.h`
- `threads/synch.c`、`threads/synch.h`
- `threads/fixed-point.h`
