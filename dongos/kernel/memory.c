#include "memory.h"
#include "type_def.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */
//#define MEMORY_MODULE_DBG
#define MODULE_NAME_PFX    "[memory.c]"
#ifdef MEMORY_MODULE_DBG
	#define MM_DBG(fmt, args...)        printf(MODULE_NAME_PFX " [%s %d] " fmt, __func__, __LINE__, ##args)
#else
	#define MM_DBG(fmt, args...)        
#endif
#define MM_ERROR(fmt, args...)        printf("[ERROR] " MODULE_NAME_PFX " [%s %d] " fmt, __func__, __LINE__, ##args)

#if (MAXMEMHEAP >= 32768)             //内存管理默认使用15位数据标识，最大能管理32768字节
  #error MAXMEMHEAP is too big to manage!
#endif

//内存分配返回的是一个指向分配区域的指针，指针的长度是内存控制头和内存对齐方式中较大的一个。
//并且这个长度也是最小分配单元的长度。在些 中它的长度是16bit。
//此处需要根据实际编译环境修改，确保memhdr_t长度为16bit或以上
#define MEM_HDRSZ              sizeof(memhdr_t) //4 byte(32bit)

// Round a value up to the ceiling of MEM_HDRSZ for critical dependencies on even multiples.
//将申请的内存空间大小向上调整(ceil)，如申请15字节的空间，则调整成16字节
#define MEM_ROUND(X)       ((((X) + MEM_HDRSZ - 1) / MEM_HDRSZ) * MEM_HDRSZ) //like DOS_ALIGN()

//查找到合适的内存块之后，就要决定是否对此内存块进行分割。
//如果内存块过大的话必然会造成内存的浪费。
//如果内存块的大小减去要申请的内存块的值大于MEM_MIN_BLKSZ(4byte)， 
//则分割此内存块。并初使化分割出来的内存分配控制块头。
//最小块大小 
#if !defined(MEM_MIN_BLKSZ)
  #define MEM_MIN_BLKSZ         (MEM_ROUND((MEM_HDRSZ * 2)))
#endif

//默认长存块大小
#if !defined MEM_LL_BLKSZ
//#if defined ZCL_KEY_ESTABLISH     // Attempt to capture worst-case for SE sample apps.
//#define MEM_LL_BLKSZ          (MEM_ROUND(526) + (32 * MEM_HDRSZ))
#define MEM_LL_BLKSZ          (MEM_ROUND(417) + (19 * MEM_HDRSZ))
#endif

/*********************************************************************
 * CONSTANTS
 */
#define MEM_IN_USE  0x8000//0000        //内存控制头（32bit）最高位标识该内存块是否被使用

/* 根据程序的概要分析，最大的同时分配的大小是16x18, 如果不进行程序概要分析，
   在定时分析系统的开销时，最坏情况里面的最好情况的内存分配大小是232(byte)的长度。
   所以把固定分配区域的长度定义为232（byte）。
 */
#define SMALLBLKHEAP    232

#if !defined MEM_SMALL_BLKSZ
#define MEM_SMALL_BLKSZ       (MEM_ROUND(16)) //16 //固定内存分配区域的固定长度，16字节//小块大小
#endif
#if !defined MEM_SMALL_BLKCNT
#define MEM_SMALL_BLKCNT       16 //8 //默认小块数量
#endif

/*
 * These numbers setup the size of the small-block bucket which is reserved at the front of the
 * heap for allocations of MEM_SMALL_BLKSZ or smaller.
 */
// Size of the heap bucket reserved for small block-sized allocations.
// Adjust accordingly to attempt to accomodate the vast majority of very high frequency operations.
//小块总空间
#define MEM_SMALLBLK_BUCKET   ((MEM_SMALL_BLKSZ * MEM_SMALL_BLKCNT) + MEM_LL_BLKSZ) //((MAXMEMHEAP / MEM_HDRSZ) / 8) //
// Index of the first available memhdr_t after the small-block heap which will be set in-use in
// order to prevent the small-block bucket from being coalesced with the wilderness.
//区分块位置
#define MEM_SMALLBLK_HDRCNT   (MEM_SMALLBLK_BUCKET / MEM_HDRSZ)
// Index of the first available memhdr_t after the small-block heap which will be set in-use in
#define MEM_BIGBLK_IDX        (MEM_SMALLBLK_HDRCNT + 1)
// The size of the wilderness after losing the small-block heap, the wasted header to block the
// small-block heap from being coalesced, and the wasted header to mark the end of the heap.
#define MEM_BIGBLK_SZ         (MAXMEMHEAP - MEM_SMALLBLK_BUCKET - MEM_HDRSZ*2)
// Index of the last available memhdr_t at the end of the heap which will be set to zero for
// fast comparisons with zero to determine the end of the heap.
#define MEM_LASTBLK_IDX      ((MAXMEMHEAP / MEM_HDRSZ) - 1)

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */


/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */


//内存分配返回的是一个指向分配区域的指针，指针的长度是内存控制头和内存对齐方式中较大的一个。
//并且这个长度也是最小分配单元的长度。在STM32L4中它的长度是32bit。
//此处需要根据实际编译环境修改，确保memhdr_t长度为32bit或以上
//typedef hal_data_align_t  memhdr_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Local Variables
 * ------------------------------------------------------------------------------------------------
 */

memhdr_t theHeap[MAXMEMHEAP / MEM_HDRSZ];
static memhdr_t *ff1;  // First free block in the small-block bucket.


static uint8 memStat;            // Discrete status flags: 0x01 = kicked.

#if MEM_METRICS
static uint16 blkMax;  // Max cnt of all blocks ever seen at once.
static uint16 blkCnt;  // Current cnt of all blocks.
static uint16 blkFree; // Current cnt of free blocks.
static uint16 memAlo;  // Current total memory allocated.
static uint16 memMax;  // Max total memory ever allocated at once.
#endif
/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**************************************************************************************************
 * @fn          mem_init
 *
 * @brief       This function is the heap memory management initialization callback.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void mem_init(void)
{
  // Setup a NULL block at the end of the heap for fast comparisons with zero.
  //整个内存池最后的两个字节清零，避免分配内存产生溢出
  theHeap[MEM_LASTBLK_IDX].val = 0;
  MM_DBG("MEM_LASTBLK_IDX:%d \r\n", MEM_LASTBLK_IDX);

  // Setup the small-block bucket.
  //设置固定长度内存区长度，232字节，内存区第一个字保存该区长度
  ff1 = theHeap;
  ff1->val = MEM_SMALLBLK_BUCKET;                   // Set 'len' & clear 'inUse' field.
  MM_DBG("ff1->val:0x%08x MEM_SMALLBLK_BUCKET:%d \r\n", ff1->val, MEM_SMALLBLK_BUCKET);

  // Set 'len' & 'inUse' fields - this is a 'zero data bytes' lifetime allocation to block the
  // small-block bucket from ever being coalesced with the wilderness.
  theHeap[MEM_SMALLBLK_HDRCNT].val = (MEM_HDRSZ | MEM_IN_USE) | (HEAP_MAGIC << 16);
  MM_DBG("theHeap[MEM_SMALLBLK_HDRCNT].val:0x%08x MEM_SMALLBLK_HDRCNT:%d \r\n", theHeap[MEM_SMALLBLK_HDRCNT].val, MEM_SMALLBLK_HDRCNT);

  // Setup the wilderness.
  theHeap[MEM_BIGBLK_IDX].val = MEM_BIGBLK_SZ;  // Set 'len' & clear 'inUse' field.

  MM_DBG("MEM_BIGBLK_IDX:%d MEM_BIGBLK_SZ:0x%08x \r\n", MEM_BIGBLK_IDX, MEM_BIGBLK_SZ);
}

/**************************************************************************************************
 * @fn          mem_kick
 *
 * @brief       This function is the task initialization callback.
 * @brief       Kick the ff1 pointer out past the long-lived Task blocks.
 *              Invoke this once after all long-lived blocks have been allocated -
 *              presently at the end of init_system().
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void mem_kick(void)
{
  dos_int_state_t  int_state;
  memhdr_t *tmp = mem_alloc(1);

  ENTER_CRITICAL_SECTION(int_state);  // Hold off interrupts.

  /* All long-lived allocations have filled the LL block reserved in the small-block bucket.
   * Set 'memStat' so searching for memory in this bucket from here onward will only be done
   * for sizes meeting the MEM_SMALL_BLKSZ criteria.
   */
  ff1 = tmp - 1;       // Set 'ff1' to point to the first available memory after the LL block.
  mem_free(tmp);
  memStat = 0x01;  // Set 'memStat' after the free because it enables memory profiling.

  EXIT_CRITICAL_SECTION(int_state);  // Re-enable interrupts.
}

/**************************************************************************************************
 * @fn          mem_alloc
 *
 * @brief       This function implements the dynamic memory allocation functionality.
 *
 * input parameters
 *
 * @param size - the number of bytes to allocate from the HEAP.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void *mem_alloc(uint16 size)
{
  memhdr_t  *prev = NULL;
  memhdr_t  *hdr;
  dos_int_state_t  int_state;
  byte coal = 0;

  MM_DBG("mem_alloc(%u)\r\n", size);
  size += MEM_HDRSZ;

  // Calculate required bytes to add to 'size' to align to hal_data_align_t.
  //根据实际的芯片的字长hal_data_align_t进行字节对齐
  if (sizeof(hal_data_align_t) == 4)//2 )
  {
    //size += (size & 0x01);
	  MM_DBG("mem_alloc(%u) sizeof(memhdr_t):%d\r\n", DOS_ALIGN(size, 4), sizeof(memhdr_t));
	size = DOS_ALIGN(size, 4);
  }
  else if (sizeof(hal_data_align_t) != 1)
  {
    const byte mod = size % sizeof( hal_data_align_t );

    if ( mod != 0 )
    {
      size += (sizeof(hal_data_align_t) - mod);
    }
  }

  MM_DBG("mem_alloc(%u)\r\n", size);
  ENTER_CRITICAL_SECTION(int_state); // Hold off interrupts.

  // Smaller allocations are first attempted in the small-block bucket, and all long-lived
  // allocations are channeled into the LL block reserved within this bucket.
  if (size <= MEM_SMALL_BLKSZ)
  {
    hdr = ff1;
  }
  else
  {
    hdr = (theHeap + MEM_BIGBLK_IDX);
  }

  do
  {
    if ( hdr->hdr.inUse )
    {//该片已被使用，得出长度
      coal = 0;                 //找到空内存标志清零，因为遇到了一片已使用的内存
    }
    else                        //该片未被使用
    {
      if ( coal != 0 )          //上轮查找有找到空内存，但空间不够
      {
#if (MEM_METRICS)
        blkCnt--;               //内存合并，总内存块计数减一
        blkFree--;              //内存合并，空闲内存块计数减一
#endif
        prev->hdr.len += hdr->hdr.len;          //加上本次找到的空内存大小

        if ( prev->hdr.len >= size )    //加上后内存大小符合申请需要
        {
          hdr = prev;           //返回该内存块
          //得出长度
          break;
        }
      }
      else                      //上轮查找未找到空内存
      {
        if ( hdr->hdr.len >= size )      //该片内存大小符合需求，跳出查找循环
        {
          break;
        }

        coal = 1;               //该片内存大小不符合需求，标记找到一块空内存
        prev = hdr;             //记录该内存
      }
    }

    hdr = (memhdr_t *)((byte *)hdr + hdr->hdr.len);  //偏移至下一片内存区域

    //读取该区域长度
    if ( hdr->val == 0 )
    {
      hdr = ((void *)NULL);
      break;
    }
  }while ( 1 );

  if ( hdr != ((void *)NULL))
  {
    uint16 tmp = hdr->hdr.len - size;                //本次申请后剩余长度

    // Determine whether the threshold for splitting is met.
    if ( tmp >= MEM_MIN_BLKSZ )   //剩余空间大于最小需求空间，分割内存供下次申请
    {
      // Split the block before allocating it.
      memhdr_t *next = (memhdr_t *)((byte *)hdr + size);  //偏移
      next->val = tmp;                    //记录未使用区域剩余长度
      hdr->val = (size | MEM_IN_USE); //标志本次申请区域已被使用，并记录本次使用长度
      hdr->hdr.magic = HEAP_MAGIC; 

#if (MEM_METRICS)
      blkCnt++;                       //内存分割，总内存块计数加一
      if ( blkMax < blkCnt )
      {
        blkMax = blkCnt;              //调整内存块数量最大值
      }
      memAlo += size;                 //调整已用内存大小
#endif
    }
    else
    {
#if (MEM_METRICS)
      memAlo += hdr->hdr.len;
      blkFree--;                      //内存不分割，空闲内存块计数减一
#endif

      hdr->hdr.inUse = TRUE;
      hdr->hdr.magic = HEAP_MAGIC; 
    }

#if (MEM_METRICS)
    if ( memMax < memAlo )
    {
      memMax = memAlo;
    }
#endif


    if ((memStat != 0) && (ff1 == hdr))
    {
      ff1 = (memhdr_t *)((uint8 *)hdr + hdr->hdr.len);
    }

    MM_DBG("magic:0x%x inUse:%d, len:%d \r\n", hdr->hdr.magic, hdr->hdr.inUse, hdr->hdr.len);
    hdr++;    //偏移，返回实际申请的内存地址
  }

  EXIT_CRITICAL_SECTION(int_state);  // Re-enable interrupts.


  MM_DBG("mem_alloc(%u)->%lx \r\n", size, (unsigned)hdr);

  return (void *)hdr;
}

/**************************************************************************************************
 * @fn          mem_free
 *
 * @brief       This function implements the dynamic memory de-allocation functionality.
 *
 * input parameters
 *
 * @param ptr - A valid pointer (i.e. a pointer returned by mem_alloc()) to the memory to free.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void mem_free(void *ptr)
{
  memhdr_t *hdr = (memhdr_t *)ptr - 1;
  dos_int_state_t  int_state;

  if ((hdr->hdr.inUse == FALSE) || (hdr->hdr.magic != HEAP_MAGIC))
  {
	  MM_ERROR("to free a bad data block: \r\n");
	  MM_ERROR("magic:0x%x inUse:%d, len:%d \r\n", hdr->hdr.magic, hdr->hdr.inUse, hdr->hdr.len);
	  return ;
  }

  ENTER_CRITICAL_SECTION(int_state);  // Hold off interrupts.
  hdr->hdr.inUse = FALSE;
  hdr->hdr.magic = HEAP_MAGIC; 

  if (ff1 > hdr)
  {
    ff1 = hdr;
  }

#if MEM_METRICS
  memAlo -= hdr->hdr.len;
  blkFree++;
#endif

  EXIT_CRITICAL_SECTION(int_state);  // Re-enable interrupts.
}

#if MEM_METRICS
/*********************************************************************
 * @fn      heap_block_max
 *
 * @brief   Return the maximum number of blocks ever allocated at once.
 *
 * @param   none
 *
 * @return  Maximum number of blocks ever allocated at once.
 */
uint16 heap_block_max(void)
{
  return blkMax;
}

/*********************************************************************
 * @fn      heap_block_cnt
 *
 * @brief   Return the current number of blocks now allocated.
 *
 * @param   none
 *
 * @return  Current number of blocks now allocated.
 */
uint16 heap_block_cnt(void)
{
  return blkCnt;
}

/*********************************************************************
 * @fn      heap_block_free
 *
 * @brief   Return the current number of free blocks.
 *
 * @param   none
 *
 * @return  Current number of free blocks.
 */
uint16 heap_block_free(void)
{
  return blkFree;
}

/*********************************************************************
 * @fn      heap_mem_used
 *
 * @brief   Return the current number of bytes allocated.
 *
 * @param   none
 *
 * @return  Current number of bytes allocated.
 */
uint16 heap_mem_used(void)
{
  return memAlo;
}

/*********************************************************************
 * @fn      heap_high_water
 *
 * @brief   Return the highest byte ever allocated in the heap.
 *
 * @param   none
 *
 * @return  Highest number of bytes ever used by the stack.
 */
uint16 heap_high_water(void)
{
#if ( MEM_METRICS )
  return memMax;
#else
  return MAXMEMHEAP;
#endif
}

//返回内存使用率
uint16 heap_mem_usage_rate(void)
{
  return (uint16)(memAlo/(MAXMEMHEAP/100));
}

#endif

