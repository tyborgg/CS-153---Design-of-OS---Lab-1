#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    struct shm_page {
        uint id;
        char *frame;
        int refcnt;
    } shm_pages[64];
} shm_table;

void shminit() {
    int i;
    initlock(&(shm_table.lock), "SHM lock");
    acquire(&(shm_table.lock));
    for (i = 0; i< 64; i++) {
        shm_table.shm_pages[i].id =0;
        shm_table.shm_pages[i].frame =0;
        shm_table.shm_pages[i].refcnt =0;
    }
    release(&(shm_table.lock));
}
//test
int shm_open(int id, char **pointer) {
    struct proc *curproc = myproc();
    acquire(&(shm_table.lock));
    int i;

    for (i = 0; i< 64; i++) {
        if(id == shm_table.shm_pages[i].id){
            mappages(curproc->pgdir, (void *)curproc->sz, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U);
            shm_table.shm_pages[i].refcnt++;
        }
        else if(shm_table.shm_pages[i].id == 0){
            shm_table.shm_pages[i].id = id;
            shm_table.shm_pages[i].frame = kalloc();
            shm_table.shm_pages[i].refcnt++;
            memset(shm_table.shm_pages[i].frame, 0, PGSIZE);
            mappages(curproc->pgdir, (void *)curproc->sz, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U);
        }

        *pointer = (char *)curproc->sz;
        curproc->sz += PGSIZE;
    }

    release(&(shm_table.lock));

    return (int)*pointer; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
    acquire(&(shm_table.lock));
    int i;

    for (i = 0; i< 64; i++) {
        if(id == shm_table.shm_pages[i].id){
            shm_table.shm_pages[i].refcnt--;
        }
        if(shm_table.shm_pages[i].refcnt == 0){
            shm_table.shm_pages[i].id = 0;
            shm_table.shm_pages[i].frame = 0;
            shm_table.shm_pages[i].refcnt = 0;
        }
    }

    release(&(shm_table.lock));

    return 0; //added to remove compiler warning -- you should decide what to return
}