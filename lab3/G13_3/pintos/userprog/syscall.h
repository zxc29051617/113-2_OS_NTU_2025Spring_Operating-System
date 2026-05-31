#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

#ifdef VM
void mmap_close_all (void);
#endif

#endif /* userprog/syscall.h */
