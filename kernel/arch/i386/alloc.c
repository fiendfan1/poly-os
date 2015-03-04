/*
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <kernel/alloc.h>

typedef struct DList_ DList;
struct DList_ {
    DList *next;
    DList *prev;
};
 
// initialize a one element DList
static inline void dlist_init(DList *dlist) {
    //printf("%s(%p)\n", __FUNCTION__, dlist);
    dlist->next = dlist;
    dlist->prev = dlist;
}
 
// insert d2 after d1
static inline void dlist_insert_after(DList *d1, DList *d2) {
    //printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *n1 = d1->next;
    DList *e2 = d2->prev;
 
    d1->next = d2;
    d2->prev = d1;
    e2->next = n1;
    n1->prev = e2;
}
 
// insert d2 before d1
static inline void dlist_insert_before(DList *d1, DList *d2) {
    //printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *e1 = d1->prev;
    DList *e2 = d2->prev;
 
    e1->next = d2;
    d2->prev = e1;
    e2->next = d1;
    d1->prev = e2;
}
 
// remove d from the list
static inline void dlist_remove(DList *d) {
    //printf("%s(%p)\n", __FUNCTION__, d);
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->next = d;
    d->prev = d;    
}
 
// push d2 to the front of the d1p list
static inline void dlist_push(DList **d1p, DList *d2) {
    //printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p != 0) {
    dlist_insert_before(*d1p, d2);
    }
    *d1p = d2;
}
 
// pop the front of the dp list
static inline DList* dlist_pop(DList **dp) {
    //printf("%s(%p)\n", __FUNCTION__, dp);
    DList *d1 = *dp;
    DList *d2 = d1->next;
    dlist_remove(d1);
    if (d1 == d2) {
    *dp = 0;
    } else {
    *dp = d2;
    }
    return d1;
}
 
// remove d2 from the list, advancing d1p if needed
static inline void dlist_remove_from(DList **d1p, DList *d2) {
    //printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p == d2) {
    dlist_pop(d1p);
    } else {
    dlist_remove(d2);
    }
}
 
#define CONTAINER(C, l, v) ((C*)(((char*)v) - (intptr_t)&(((C*)0)->l)))
#define OFFSETOF(TYPE, MEMBER)  __builtin_offsetof (TYPE, MEMBER)
 
#define DLIST_INIT(v, l) dlist_init(&v->l)
 
#define DLIST_REMOVE_FROM(h, d, l)                    \
    {                                    \
    typeof(**h) **h_ = h, *d_ = d;                    \
    DList *head = &(*h_)->l;                    \
    dlist_remove_from(&head, &d_->l);                    \
    if (head == 0) {                        \
        *h_ = 0;                            \
    } else {                            \
        *h_ = CONTAINER(typeof(**h), l, head);            \
    }                                \
    }
 
#define DLIST_PUSH(h, v, l)                        \
    {                                    \
    typeof(*v) **h_ = h, *v_ = v;                    \
    DList *head = &(*h_)->l;                    \
    if (*h_ == 0) head = 0;                    \
    dlist_push(&head, &v_->l);                    \
    *h_ = CONTAINER(typeof(*v), l, head);                \
    }
 
#define DLIST_POP(h, l)                            \
    ({                                    \
    typeof(**h) **h_ = h;                        \
    DList *head = &(*h_)->l;                    \
    DList *res = dlist_pop(&head);                    \
    if (head == 0) {                        \
        *h_ = 0;                            \
    } else {                            \
        *h_ = CONTAINER(typeof(**h), l, head);            \
    }                                \
    CONTAINER(typeof(**h), l, res);                    \
    })
 
#define DLIST_ITERATOR_BEGIN(h, l, it)                    \
    {                                    \
        typeof(*h) *h_ = h;                        \
    DList *last_##it = h_->l.prev, *iter_##it = &h_->l, *next_##it;    \
    do {                                \
        if (iter_##it == last_##it) {                \
        next_##it = 0;                    \
        } else {                            \
        next_##it = iter_##it->next;                \
        }                                \
        typeof(*h)* it = CONTAINER(typeof(*h), l, iter_##it);
 
#define DLIST_ITERATOR_END(it)                        \
    } while((iter_##it = next_##it));                \
    }
 
#define DLIST_ITERATOR_REMOVE_FROM(h, it, l) DLIST_REMOVE_FROM(h, iter_##it, l)
 
typedef struct Chunk_ Chunk;
struct Chunk_ {
    DList all;
    int used;
    union {
    char data[0];
    DList free;
    };
};
 
enum {
    NUM_SIZES = 32,
    ALIGN = 4,
    MIN_SIZE = sizeof(DList),
    HEADER_SIZE = OFFSETOF(Chunk, data),
};
 
Chunk *free_chunk[NUM_SIZES] = { 0 };
size_t mem_free = 0;
size_t mem_used = 0;
size_t mem_meta = 0;
Chunk *first = 0;
Chunk *last = 0;
 
static void memory_chunk_init(Chunk *chunk) {
    //printf("%s(%p)\n", __FUNCTION__, chunk);
    DLIST_INIT(chunk, all);
    chunk->used = 0;
    DLIST_INIT(chunk, free);
}
 
static size_t memory_chunk_size(const Chunk *chunk) {
    //printf("%s(%p)\n", __FUNCTION__, chunk);
    char *end = (char*)(chunk->all.next);
    char *start = (char*)(&chunk->all);
    return (end - start) - HEADER_SIZE;
}
 
static int memory_chunk_slot(size_t size) {
    int n = -1;
    while(size > 0) {
    ++n;
    size /= 2;
    }
    return n;
}
 
void memory_init(void *mem, size_t size) {
    char *mem_start = (char *)(((intptr_t)mem + ALIGN - 1) & (~(ALIGN - 1)));
    char *mem_end = (char *)(((intptr_t)mem + size) & (~(ALIGN - 1)));
    first = (Chunk*)mem_start;
    Chunk *second = first + 1;
    last = ((Chunk*)mem_end) - 1;
    memory_chunk_init(first);
    memory_chunk_init(second);
    memory_chunk_init(last);
    dlist_insert_after(&first->all, &second->all);
    dlist_insert_after(&second->all, &last->all);
    // make first/last as used so they never get merged
    first->used = 1;
    last->used = 1;

    printf("Memory init: mem_start # %d # mem_end # %d #",
            (uintptr_t)mem_start, (uintptr_t)mem_end);
 
    size_t len = memory_chunk_size(second);
    int n = memory_chunk_slot(len);
    //printf("%s(%p, %#lx) : adding chunk %#lx [%d]\n", __FUNCTION__, mem, size, len, n);
    DLIST_PUSH(&free_chunk[n], second, free);
    mem_free = len - HEADER_SIZE;
    mem_meta = sizeof(Chunk) * 2 + HEADER_SIZE;
}
 
void* kmalloc(size_t size) {
    size = (size + ALIGN - 1) & (~(ALIGN - 1));
 
    if (size < MIN_SIZE) size = MIN_SIZE;
 
    int n = memory_chunk_slot(size - 1) + 1;
 
    if (n >= NUM_SIZES) {
        printf("kmalloc error #1");
        return 0;
    }
 
    while(!free_chunk[n]) {
        ++n;
        if (n >= NUM_SIZES) {
            printf("kmalloc error #2");
            return 0;
        }
    }
 
    Chunk *chunk = DLIST_POP(&free_chunk[n], free);
    size_t size2 = memory_chunk_size(chunk);
    //printf("@ %p [%#lx]\n", chunk, size2);
    size_t len = 0;

//    printf("MEM(%d)", (unsigned int)chunk);
 
    if (size + sizeof(Chunk) <= size2) {
        Chunk *chunk2 = (Chunk*)(((char*)chunk) + HEADER_SIZE + size);
        memory_chunk_init(chunk2);
        dlist_insert_after(&chunk->all, &chunk2->all);
        len = memory_chunk_size(chunk2);
        int n = memory_chunk_slot(len);
        //printf("  adding chunk @ %p %#lx [%d]\n", chunk2, len, n);
        DLIST_PUSH(&free_chunk[n], chunk2, free);
        mem_meta += HEADER_SIZE;
        mem_free += len - HEADER_SIZE;
    }
 
    chunk->used = 1;
    //memset(chunk->data, 0xAA, size);
    //printf("AAAA\n");
    mem_free -= size2;
    mem_used += size2 - len - HEADER_SIZE;
    //printf("  = %p [%p]\n", chunk->data, chunk);
    return chunk->data;
}

static void remove_free(Chunk *chunk) {
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : removing chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_REMOVE_FROM(&free_chunk[n], chunk, free);
    mem_free -= len - HEADER_SIZE;
}
 
static void push_free(Chunk *chunk) {
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : adding chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_PUSH(&free_chunk[n], chunk, free);
    mem_free += len - HEADER_SIZE;
}
 
void kfree(void *mem) {
    Chunk *chunk = (Chunk*)((char*)mem - HEADER_SIZE);
    Chunk *next = CONTAINER(Chunk, all, chunk->all.next);
    Chunk *prev = CONTAINER(Chunk, all, chunk->all.prev);
 
    //printf("%s(%p): @%p %#lx [%d]\n", __FUNCTION__, mem, chunk, memory_chunk_size(chunk), memory_chunk_slot(memory_chunk_size(chunk)));
    mem_used -= memory_chunk_size(chunk);
 
    if (next->used == 0) {
        // merge in next
        remove_free(next);
        dlist_remove(&next->all);
        //memset(next, 0xDD, sizeof(Chunk));
        mem_meta -= HEADER_SIZE;
        mem_free += HEADER_SIZE;
    }
    if (prev->used == 0) {
        // merge to prev
        remove_free(prev);
        dlist_remove(&chunk->all);
        //memset(chunk, 0xDD, sizeof(Chunk));
        push_free(prev);
        mem_meta -= HEADER_SIZE;
        mem_free += HEADER_SIZE;
    } else {
        // make chunk as free
        chunk->used = 0;
        DLIST_INIT(chunk, free);
        push_free(chunk);
    }
}

// XXX: Idk if this is correct...
void* krealloc(void* mem, size_t size) {
    Chunk *chunk = (Chunk*)((char*)mem - HEADER_SIZE);
    size_t mem_len = memory_chunk_size(chunk);

    void* new_mem = kmalloc(mem_len + size);
    memcpy(new_mem, mem, mem_len);
    kfree(mem);

    return new_mem;
}
 
void check(void) {
    int    i;
    Chunk *t = last;
 
    DLIST_ITERATOR_BEGIN(first, all, it) {
//        assert(CONTAINER(Chunk, all, it->all.prev) == t);
        t = it;
    } DLIST_ITERATOR_END(it);
 
    for(i = 0; i < NUM_SIZES; ++i) {
        if (free_chunk[i]) {
            t = CONTAINER(Chunk, free, free_chunk[i]->free.prev);
            DLIST_ITERATOR_BEGIN(free_chunk[i], free, it) {
//            assert(CONTAINER(Chunk, free, it->free.prev) == t);
            t = it;
            } DLIST_ITERATOR_END(it);
        }
    }
}
*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct _KHEAPBLOCKBM {
    struct _KHEAPBLOCKBM *next;
    uint32_t size;
    uint32_t used;
    uint32_t bsize;
    uint32_t lfb;
} KHEAPBLOCKBM;
 
typedef struct _KHEAPBM {
    KHEAPBLOCKBM            *fblock;
} KHEAPBM;
 
void k_heapBMInit(KHEAPBM *heap) {
    heap->fblock = 0;
}
 
int k_heapBMAddBlock(KHEAPBM *heap, uintptr_t addr, uint32_t size, uint32_t bsize) {
    KHEAPBLOCKBM        *b;
    uint32_t                bcnt;
    uint32_t                x;
    uint8_t                *bm;
 
    b = (KHEAPBLOCKBM*)addr;
    b->size = size - sizeof(KHEAPBLOCKBM);
    b->bsize = bsize;
 
    b->next = heap->fblock;
    heap->fblock = b;
 
    bcnt = size / bsize;
    bm = (uint8_t*)&b[1];
 
    // clear bitmap
    for (x = 0; x < bcnt; ++x) {
            bm[x] = 0;
    }
 
    // reserve room for bitmap
    bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
    for (x = 0; x < bcnt; ++x) {
            bm[x] = 5;
    }
 
    b->lfb = bcnt - 1;
 
    b->used = bcnt;
 
    return 1;
}
 
static uint8_t k_heapBMGetNID(uint8_t a, uint8_t b) {
    uint8_t        c;    
    for (c = a + 1; c == b || c == 0; ++c);
    return c;
}
 
void *k_heapBMAlloc(KHEAPBM *heap, uint32_t size) {
    KHEAPBLOCKBM        *b;
    uint8_t                *bm;
    uint32_t                bcnt;
    uint32_t                x, y, z;
    uint32_t                bneed;
    uint8_t                nid;
 
    // iterate blocks
    for (b = heap->fblock; b; b = b->next) {
        // check if block has enough room
        if (b->size - (b->used * b->bsize) >= size) {
 
            bcnt = b->size / b->bsize;        
            bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1 : size / b->bsize;
            bm = (uint8_t*)&b[1];
 
            for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x != b->lfb; ++x) {
                // just wrap around
                if (x >= bcnt) {
                    x = 0;
                }        
 
                if (bm[x] == 0) {    
                    // count free blocks
                    for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y);
 
                    // we have enough, now allocate them
                    if (y == bneed) {
                        // find ID that does not match left or right
                        nid = k_heapBMGetNID(bm[x - 1], bm[x + y]);
 
                        // allocate by setting id
                        for (z = 0; z < y; ++z) {
                            bm[x + z] = nid;
                        }
 
                        // optimization
                        b->lfb = (x + bneed) - 2;
 
                        // count used blocks NOT bytes
                        b->used += y;
 
                        return (void*)(x * b->bsize + (uintptr_t)&b[1]);
                    }
 
                    // x will be incremented by one ONCE more in our FOR loop
                    x += (y - 1);
                    continue;
                }
            }
        }
    }

    printf("k_heapBMAlloc: error");
    return 0;
}
 
void k_heapBMFree(KHEAPBM *heap, void *ptr) {
    KHEAPBLOCKBM        *b;    
    uintptr_t                ptroff;
    uint32_t                bi, x;
    uint8_t                *bm;
    uint8_t                id;
    uint32_t                max;
 
    for (b = heap->fblock; b; b = b->next) {
        if ((uintptr_t)ptr > (uintptr_t)b && (uintptr_t)ptr < (uintptr_t)b + b->size) {
            // found block
            ptroff = (uintptr_t)ptr - (uintptr_t)&b[1];  // get offset to get block
            // block offset in BM
            bi = ptroff / b->bsize;

            bm = (uint8_t*)&b[1];
            // clear allocation
            id = bm[bi];
            // oddly.. GCC did not optimize this
            max = b->size / b->bsize;
            for (x = bi; bm[x] == id && x < max; ++x) {
                bm[x] = 0;
            }
            // update free block count
            b->used -= x - bi;
            return;
        }
    }
 
    // this error needs to be raised or reported somehow
    return;
}


KHEAPBM kheap;

void kfree(void* mem)
{
    k_heapBMFree(&kheap, mem);
}

void* kmalloc(size_t size)
{
    k_heapBMAlloc(&kheap, size);
}

void* krealloc(void* mem, size_t size)
{
    return mem;  // XXX TODO
}

void memory_init(void* mem, size_t size)
{
    k_heapBMInit(&kheap);
    k_heapBMAddBlock(&kheap, (uintptr_t)mem, size, 16);
}
