#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

//Lab_1
int
sys_exit(void)
{
  int status;
  if(argint(0, &status) < 0){
      return -1;
  }
  exit(status);
  return 0;  // not reached
}

//Lab_1
int
sys_wait(void)
{
    char** status = 0;
    argptr(0, status, sizeof(status));
    int *w_status = (int*)*status;
    return wait(w_status);
}

//Lab_1
int
sys_waitpid(void)
{
    char** status = 0;
    int pid = 0;
    argptr(1, status, sizeof(status));
    argint(0, &pid);
    int *w_status = (int*)*status;
    return waitpid(pid, w_status, 0);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_add(void)
{
  int a = 2;
  int b = 2020;
  return a+b;
}
