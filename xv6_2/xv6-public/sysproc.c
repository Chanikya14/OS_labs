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

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
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
char* sys_mmap(void) {
  int n;
  argint(0,&n);
  return mmap(n);
}
int sys_getvasize(void) {
  int pid;
  argint(0,&pid);
  return getvasize(pid);
}
int sys_getpasize(void) {
  int pid;
  argint(0,&pid);
  return getpasize(pid);
}
uint sys_va_to_pa(void){
  int va;
  argint(0,&va);
  pde_t *pgdir = myproc()->pgdir;
  pte_t *pde = &pgdir[PDX(va)]; // got the entry in pgdir
  pte_t *pgtab;
  pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  return ((pgtab[PTX(va)]>>(12))<<(12))+(va&(0xFFF));
}
int sys_get_pgtb_size(void) {
  pde_t *pgdir = myproc()->pgdir;
  pde_t *pde;
  //pte_t *p;
  int cnt=0;
  for(int i=0;i<=1023;i++) {
    pde = &pgdir[i];
    if(*pde&PTE_P) {
      cnt++;
    } 
  }
  return cnt;
}
int sys_get_usr_pgtb_size(void){
  pde_t *pgdir = myproc()->pgdir;
  pde_t *pde;
  pte_t *p;
  int cnt=0;
  for(int i=0;i<=1023;i++) {
    pde = &pgdir[i];
    p = (pte_t*)P2V(PTE_ADDR(*pde));
    if(*pde&PTE_P) {
      if(*p&PTE_U) cnt++;
    }
    
  }
  return cnt;
}
int sys_get_kernel_pgtb_size(void){
    pde_t *pgdir = myproc()->pgdir;
  pde_t *pde;
  pte_t *p;
  int cnt=0;
  for(int i=0;i<=1023;i++) {
    pde = &pgdir[i];
    p = (pte_t*)P2V(PTE_ADDR(*pde));
    if(*pde&PTE_P) {
      if (!(*p&PTE_U)) cnt++;
    }
  }
  return cnt;
}
int sys_sleep(void)
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
