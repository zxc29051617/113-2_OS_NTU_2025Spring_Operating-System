# Lab 1 - System Call

- `EE5173 lab01_ System Call.pdf`：題目說明
- `G13_1.zip`：繳交的程式

## 實作內容

- argument passing
- `exit` 訊息與 process 清理
- `exec` / `wait` 的 parent-child 同步
- user pointer 檢查
- file descriptor table
- `exit`、`exec`、`wait`、`create`、`remove`、`open`、`filesize`、`read`、`write`、`seek`、`tell`、`close`

最終程式整合在 `lab3/G13_3/pintos`，主要檔案：

- `userprog/process.c`
- `userprog/syscall.c`
- `threads/thread.c`
