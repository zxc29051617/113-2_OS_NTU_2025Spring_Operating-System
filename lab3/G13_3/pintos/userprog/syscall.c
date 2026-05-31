#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include <devices/shutdown.h>

#include <string.h>
#include <filesys/file.h>
#include <devices/input.h>
#include <threads/malloc.h>
#include <threads/palloc.h>
#include "process.h"
#include "pagedir.h"
#include <threads/vaddr.h>
#include <filesys/filesys.h>

#define MAX_SYSCALL 20

// lab01 Hint - Here are the system calls you need to implement.

/* System call for process. */

void sys_halt(struct intr_frame *f);
void sys_exit(int status);
void sys_exec(struct intr_frame* f);
void sys_wait(struct intr_frame* f);

/* System call for file. */
void sys_create(struct intr_frame* f);
void sys_remove(struct intr_frame* f);
void sys_open(struct intr_frame* f);
void sys_filesize(struct intr_frame* f);
void sys_read(struct intr_frame* f);
void sys_write(struct intr_frame* f);
void sys_seek(struct intr_frame* f);
void sys_tell(struct intr_frame* f);
void sys_close(struct intr_frame* f);

#ifdef VM
void sys_mmap(struct intr_frame* f);
void sys_munmap(struct intr_frame* f);
static void check_writable_buffer (void *buffer, unsigned size);

/* 預加載並pin住[addr, addr+size)跨越的所有page */
void
preload_and_pin_pages(const void *addr, size_t size)
{
  struct thread *t = thread_current();
  struct supplemental_page_table *supt = t->spt;
  uint8_t *upage;

  for(upage = pg_round_down(addr); upage < (uint8_t*)addr + size; upage += PGSIZE) {
    // 如果用戶空間page不在記憶體中，則加載它
    if(pagedir_get_page(t->pagedir, upage) == NULL)
      vm_load_page(supt, t->pagedir, upage);
    // pin住page，防止在I/O期間被驅逐
    vm_pin_page(supt, upage);
  }
}

/* 解除pin定[addr, addr+size)跨越的所有page */
void
unpin_preloaded_pages(const void *addr, size_t size)
{
  struct thread *t = thread_current();
  struct supplemental_page_table *supt = t->spt;
  uint8_t *upage;

  for(upage = pg_round_down(addr); upage < (uint8_t*)addr + size; upage += PGSIZE) {
    vm_unpin_page(supt, upage);
  }
}
#endif

static void (*syscalls[MAX_SYSCALL])(struct intr_frame *) = {
  [SYS_HALT] = sys_halt,
  [SYS_EXEC] = sys_exec,
  [SYS_WAIT] = sys_wait,
  [SYS_CREATE] = sys_create,
  [SYS_REMOVE] = sys_remove,
  [SYS_OPEN] = sys_open,
  [SYS_FILESIZE] = sys_filesize,
  [SYS_READ] = sys_read,
  [SYS_WRITE] = sys_write,
  [SYS_SEEK] = sys_seek,
  [SYS_TELL] = sys_tell,
  [SYS_CLOSE] = sys_close,
#ifdef VM
  [SYS_MMAP] = sys_mmap,
  [SYS_MUNMAP] = sys_munmap
#endif
};

static void syscall_handler (struct intr_frame *);
static void *check_ptr(const void *vaddr);
static int get_user(const uint8_t *uaddr);
static struct open_file *find_file(int fd);
void invalid_access (void);

void syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static int get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
     : "=&a" (result) : "m" (*uaddr));
  return result;
}

static void *check_ptr(const void *vaddr)
{
    if (vaddr == NULL || !is_user_vaddr(vaddr))
        invalid_access();
    // 最少要檢查 *vaddr 指向的這一頁已存在
    if (!pagedir_get_page(thread_current()->pagedir, vaddr)) {
#ifdef VM
        struct thread *t = thread_current();
        if (t->spt == NULL || !vm_load_page(t->spt, t->pagedir,
                                            pg_round_down(vaddr)))
            invalid_access();
#else
        invalid_access();
#endif
    }
    return (void *)vaddr;
}

#ifdef VM
static void
check_writable_buffer (void *buffer, unsigned size)
{
  if (size == 0)
    return;

  uint8_t *start = buffer;
  uint8_t *end = start + size - 1;
  if (end < start || !is_user_vaddr (end))
    invalid_access ();

  check_ptr (start);
  check_ptr (end);

  for (uint8_t *upage = pg_round_down (start);
       upage <= (uint8_t *) pg_round_down (end);
       upage += PGSIZE)
    {
      check_ptr (upage);
      struct suppPage *page = spt_find_page (thread_current ()->spt, upage);
      if ((page != NULL && !page->writable) ||
          (page == NULL &&
           !pagedir_is_writable (thread_current ()->pagedir, upage)))
        invalid_access ();
    }
}
#endif

static struct open_file *find_file(int fd)
{
  struct list *files = &thread_current()->files;
  for (struct list_elem *e = list_begin(files); e != list_end(files); e = list_next(e)) {
      struct open_file *f = list_entry(e, struct open_file, elem);
      if (f->fd == fd) {
          return f;
      }
  }
  return NULL;
}

void sys_exit(int status) {
    thread_current()->st_exit = status;
    thread_exit();
}

void sys_exec(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    const char *cmd_line = (const char *)args[1];
    check_ptr(cmd_line);
    f->eax = process_execute(cmd_line);
}

void sys_wait(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    int pid = args[1];
    f->eax = process_wait(pid);
}

void sys_write(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    check_ptr(args + 2);
    check_ptr(args + 3);
    
    int fd = args[1];
    const char *buffer = (const char *)args[2];
    unsigned size = (unsigned)args[3];

    check_ptr(buffer);
    if (size > 0)
        check_ptr(buffer + size - 1);  // 檢查整個 buffer 是否 user space

#ifdef VM
    preload_and_pin_pages(buffer, size);
#endif

    if (fd == 1) {  // STDOUT
        putbuf(buffer, size);
        f->eax = size;
    } else {
        struct open_file *tmp = find_file(fd);
        if (tmp) {
            acquire_file_lock();
            f->eax = file_write(tmp->file, buffer, size);
            release_file_lock();
        } else {
            f->eax = 0;
        }
    }

#ifdef VM
    unpin_preloaded_pages(buffer, size);
#endif
}

void sys_create(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    check_ptr(args + 2);
    
    const char *file = (const char *)args[1];
    unsigned initial_size = (unsigned)args[2];
    
    check_ptr(file);

    acquire_file_lock();
    f->eax = filesys_create(file, initial_size);
    release_file_lock();
}

void sys_remove(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    
    const char *file = (const char *)args[1];
    check_ptr(file);

    acquire_file_lock();
    f->eax = filesys_remove(file);
    release_file_lock();
}

void sys_open(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    
    const char *file = (const char *)args[1];
    check_ptr(file);

    acquire_file_lock();
    struct file *opened = filesys_open(file);
    release_file_lock();

    if (opened) {
        struct thread *t = thread_current();
        struct open_file *tmp = malloc(sizeof(struct open_file));
        tmp->fd = t->file_fd++;
        tmp->file = opened;
        list_push_back(&t->files, &tmp->elem);
        f->eax = tmp->fd;
    } else {
        f->eax = -1;
    }
}

void sys_filesize(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    
    int fd = args[1];

    struct open_file *tmp = find_file(fd);
    if (tmp) {
        acquire_file_lock();
        f->eax = file_length(tmp->file);
        release_file_lock();
    } else {
        f->eax = -1;
    }
}

void sys_read(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    check_ptr(args + 2);
    check_ptr(args + 3);
    
    int fd = args[1];
    char *buffer = (char *)args[2];
    unsigned size = (unsigned)args[3];

    check_ptr(buffer);
    if (size > 0)
        check_ptr(buffer + size - 1);

#ifdef VM
    check_writable_buffer(buffer, size);
    preload_and_pin_pages(buffer, size);
#endif

    if (fd == 0) {  // STDIN
        unsigned i;
        for (i = 0; i < size; i++) {
            buffer[i] = input_getc();
        }
        f->eax = size;
    } else {
        struct open_file *tmp = find_file(fd);
        if (tmp) {
            acquire_file_lock();
            f->eax = file_read(tmp->file, buffer, size);
            release_file_lock();
        } else {
            f->eax = -1;
        }
    }

#ifdef VM
    unpin_preloaded_pages(buffer, size);
#endif
}

void sys_seek(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    check_ptr(args + 2);
    
    int fd = args[1];
    unsigned position = (unsigned)args[2];

    struct open_file *tmp = find_file(fd);
    if (tmp) {
        acquire_file_lock();
        file_seek(tmp->file, position);
        release_file_lock();
    }
}

void sys_tell(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    
    int fd = args[1];
    struct open_file *tmp = find_file(fd);
    if (tmp) {
        acquire_file_lock();
        f->eax = file_tell(tmp->file);
        release_file_lock();
    } else {
        f->eax = -1;
    }
}

void sys_close(struct intr_frame *f) {
    uint32_t *args = f->esp;
    check_ptr(args + 1);
    
    int fd = args[1];
    struct open_file *tmp = find_file(fd);
    if (tmp) {
        acquire_file_lock();
        file_close(tmp->file);
        release_file_lock();

        list_remove(&tmp->elem);
        free(tmp);
    }
}

#ifdef VM
static struct mmap_desc *
find_mmap (int mapid)
{
  struct list *maps = &thread_current ()->mmap_list;
  for (struct list_elem *e = list_begin (maps); e != list_end (maps);
       e = list_next (e))
    {
      struct mmap_desc *m = list_entry (e, struct mmap_desc, elem);
      if (m->mapid == mapid)
        return m;
    }
  return NULL;
}

static void
unmap_mapping (struct mmap_desc *m)
{
  struct thread *t = thread_current ();
  size_t offset;

  for (offset = 0; offset < m->length; offset += PGSIZE)
    {
      size_t page_bytes = m->length - offset < PGSIZE
                            ? m->length - offset
                            : PGSIZE;
      vm_supt_mm_unmap (t->spt, t->pagedir, (uint8_t *) m->addr + offset,
                        m->file, offset, page_bytes);
    }

  list_remove (&m->elem);
  acquire_file_lock ();
  file_close (m->file);
  release_file_lock ();
  free (m);
}

void
mmap_close_all (void)
{
  struct list *maps = &thread_current ()->mmap_list;
  while (!list_empty (maps))
    {
      struct mmap_desc *m = list_entry (list_front (maps),
                                        struct mmap_desc, elem);
      unmap_mapping (m);
    }
}

void
sys_mmap (struct intr_frame *f)
{
  uint32_t *args = f->esp;
  check_ptr (args + 1);
  check_ptr (args + 2);

  int fd = args[1];
  void *addr = (void *) args[2];
  struct thread *t = thread_current ();

  if (fd <= 1 || addr == NULL || pg_ofs (addr) != 0 || !is_user_vaddr (addr))
    {
      f->eax = -1;
      return;
    }

  struct open_file *of = find_file (fd);
  if (of == NULL)
    {
      f->eax = -1;
      return;
    }

  acquire_file_lock ();
  off_t length = file_length (of->file);
  struct file *map_file = length > 0 ? file_reopen (of->file) : NULL;
  release_file_lock ();

  if (length <= 0 || map_file == NULL)
    {
      f->eax = -1;
      return;
    }

  for (off_t offset = 0; offset < length; offset += PGSIZE)
    {
      void *upage = (uint8_t *) addr + offset;
      if (!is_user_vaddr (upage) || spt_find_page (t->spt, upage) != NULL ||
          pagedir_get_page (t->pagedir, upage) != NULL)
        {
          acquire_file_lock ();
          file_close (map_file);
          release_file_lock ();
          f->eax = -1;
          return;
        }
    }

  struct mmap_desc *m = malloc (sizeof *m);
  if (m == NULL)
    {
      acquire_file_lock ();
      file_close (map_file);
      release_file_lock ();
      f->eax = -1;
      return;
    }

  size_t installed = 0;
  for (off_t offset = 0; offset < length; offset += PGSIZE)
    {
      void *upage = (uint8_t *) addr + offset;
      size_t page_read_bytes = length - offset < PGSIZE
                                 ? length - offset
                                 : PGSIZE;
      size_t page_zero_bytes = PGSIZE - page_read_bytes;

      if (!vm_supt_install_filesys (t->spt, upage, map_file, offset,
                                    page_read_bytes, page_zero_bytes, true))
        {
          for (size_t rollback = 0; rollback < installed; rollback += PGSIZE)
            {
              size_t bytes = installed - rollback < PGSIZE
                               ? installed - rollback
                               : PGSIZE;
              vm_supt_mm_unmap (t->spt, t->pagedir,
                                (uint8_t *) addr + rollback,
                                map_file, rollback, bytes);
            }
          acquire_file_lock ();
          file_close (map_file);
          release_file_lock ();
          free (m);
          f->eax = -1;
          return;
        }

      struct suppPage *page = spt_find_page (t->spt, upage);
      page->mmap = true;
      installed += page_read_bytes;
    }

  m->mapid = t->next_mapid++;
  m->file = map_file;
  m->addr = addr;
  m->length = length;
  list_push_back (&t->mmap_list, &m->elem);
  f->eax = m->mapid;
}

void
sys_munmap (struct intr_frame *f)
{
  uint32_t *args = f->esp;
  check_ptr (args + 1);

  struct mmap_desc *m = find_mmap ((int) args[1]);
  if (m != NULL)
    unmap_mapping (m);
}
#endif

/* System Call: void halt (void)
    Terminates Pintos by calling shutdown_power_off() (declared in devices/shutdown.h). 
*/
void sys_halt(struct intr_frame *f UNUSED)
{
  shutdown_power_off();
}

void invalid_access (void)
{
  thread_current()->st_exit = -1;
  thread_exit();
}

static void syscall_handler (struct intr_frame *f) 
{
  int syscall_num;
  
  check_ptr(f->esp);
  syscall_num = *(int *)f->esp;
  
  if (syscall_num < 0 || syscall_num >= MAX_SYSCALL) {
    thread_current()->st_exit = -1;
    thread_exit();
    return;
  }

#ifdef VM
  thread_current()->current_esp = f->esp;
#endif


  if (syscall_num == SYS_EXIT) {
    check_ptr(f->esp + 4);
    int status = *(int *)(f->esp + 4);
    sys_exit(status);
    return;
  }

  if (syscalls[syscall_num] != NULL) {
    syscalls[syscall_num](f);
  } else {
    thread_current()->st_exit = -1;
    thread_exit();
  }
}
