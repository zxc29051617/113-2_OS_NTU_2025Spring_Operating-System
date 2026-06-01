# Lab 1 - System Call

這個資料夾保存 Lab 1 的作業 PDF 與原始提交壓縮檔。

## 內容

- `EE5173 lab01_ System Call.pdf`：Lab 1 任務說明。
- `G13_1.zip`、`G21_1.zip`：過去的提交或參考壓縮檔。

## 做了哪些事

Lab 1 的最終實作已整合到 `lab3/G13_3/pintos`，主要包含：

- argument passing
- `exit` 訊息與 process cleanup
- `exec` / `wait` parent-child synchronization
- user pointer validation
- file descriptor table
- `exit`, `exec`, `wait`, `create`, `remove`, `open`, `filesize`, `read`,
  `write`, `seek`, `tell`, `close`

## 從哪裡看

- `lab3/G13_3/pintos/userprog/process.c`
- `lab3/G13_3/pintos/userprog/syscall.c`
- `lab3/G13_3/pintos/threads/thread.c`
