#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "type_def.h"

#define MAXMEMHEAP            	1024*10 //6   	//内存池大小，单位字节

#define MEM_METRICS 		1			//定义有效则开启内存统计
#define HEAP_MAGIC    0x1ea0 

typedef struct {
  // The 15 LSB's of 'val' indicate the total item size, including the header, in 8-bit bytes.
  unsigned len : 15;
  // The 1 MSB of 'val' is used as a boolean to indicate in-use or freed.
  unsigned inUse : 1;
  unsigned magic : 16;//high 0x1ea0 ==> heap 
} MemHdrHdr_t;

typedef union {
  /* Dummy variable so compiler forces structure to alignment of largest element while not wasting
   * space on targets when the halDataAlign_t is smaller than a UINT16.
   */
  hal_data_align_t alignDummy;
  uint32 val;
  MemHdrHdr_t hdr;
} memhdr_t;

#define GET_ALLOC_LEN(ptr)  (memhdr_t *)((memhdr_t *)ptr - 1)->hdr.len
#define GET_ALLOC_INUSE(ptr)  (memhdr_t *)((memhdr_t *)ptr - 1)->hdr.inUse
#define GET_ALLOC_MAGIC(ptr)  (memhdr_t *)((memhdr_t *)ptr - 1)->hdr.magic

void mem_init(void);
void mem_kick(void);
void *mem_alloc(uint16 size);
void mem_free(void *ptr);

#if MEM_METRICS
uint16 heap_block_max(void);
uint16 heap_block_cnt(void);
uint16 heap_block_free(void);
uint16 heap_mem_used(void);
uint16 heap_high_water(void);
uint16 heap_mem_usage_rate(void);
#endif



#endif //#ifndef __MEMORY_H__
