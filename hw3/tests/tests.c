#include <criterion/criterion.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "sfmm.h"

/**
 *  HERE ARE OUR TEST CASES NOT ALL SHOULD BE GIVEN STUDENTS
 *  REMINDER MAX ALLOCATIONS MAY NOT EXCEED 4 * 4096 or 16384 or 128KB
 */

Test(sf_memsuite, Malloc_an_Integer, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(sizeof(int));
  *x = 4;
  cr_assert(*x == 4, "Failed to properly sf_malloc space for an integer!");
}

Test(sf_memsuite, Free_block_check_header_footer_values, .init = sf_mem_init, .fini = sf_mem_fini) {
  void *pointer = sf_malloc(sizeof(short));
  sf_free(pointer);
  pointer = (char*)pointer - 8;
  sf_header *sfHeader = (sf_header *) pointer;
  cr_assert(sfHeader->alloc == 0, "Alloc bit in header is not 0!\n");
  sf_footer *sfFooter = (sf_footer *) ((char*)pointer + (sfHeader->block_size << 4) - 8);
  cr_assert(sfFooter->alloc == 0, "Alloc bit in the footer is not 0!\n");
}

Test(sf_memsuite, SplinterSize_Check_char, .init = sf_mem_init, .fini = sf_mem_fini){
  void* x = sf_malloc(32);
  void* y = sf_malloc(32);
  (void)y;

  sf_free(x);

  x = sf_malloc(16);

  sf_header *sfHeader = (sf_header *)((char*)x - 8);
  cr_assert(sfHeader->splinter == 1, "Splinter bit in header is not 1!");
  cr_assert(sfHeader->splinter_size == 16, "Splinter size is not 16");

  sf_footer *sfFooter = (sf_footer *)((char*)sfHeader + (sfHeader->block_size << 4) - 8);
  cr_assert(sfFooter->splinter == 1, "Splinter bit in header is not 1!");
}

Test(sf_memsuite, Check_next_prev_pointers_of_free_block_at_head_of_list, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(4);
  memset(x, 0, 0);
  cr_assert(freelist_head->next == NULL);
  cr_assert(freelist_head->prev == NULL);
}

Test(sf_memsuite, Coalesce_no_coalescing, .init = sf_mem_init, .fini = sf_mem_fini) {
  int *x = sf_malloc(4);
  int *y = sf_malloc(4);
  memset(y, 0, 0);
  sf_free(x);

  //just simply checking there are more than two things in list
  //and that they point to each other
  cr_assert(freelist_head->next != NULL);
  cr_assert(freelist_head->next->prev != NULL);
}
//#
//STUDENT UNIT TESTS SHOULD BE WRITTEN BELOW
//DO NOT DELETE THESE COMMENTS
//#
Test(sf_memsuite, Check_ErroNo_for_oversize, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(26000);
    //memset(x,1,16);
    cr_assert(x==NULL);
    cr_assert(errno == ENOMEM);//Error is Set to ENOMEM
}

Test(sf_memsuite, Coalesce_Next_And_Previous, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(16);
    memset(x,1,16);
    void* y = sf_malloc(16);
    memset(y,1,16);
    sf_free(x);
    sf_free(y); // Coalescinf with prev and next free. Whole page is empty
    void *z = sf_malloc(4080); //Malloc whole page
    memset(z,1,4080);
    cr_assert(freelist_head == NULL);//freelist head should be null
}

Test(sf_memsuite, Two_Partial_Page_Allocation, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(4097);
    memset(x,1,4097);
    sf_free(x);
    cr_assert(freelist_head->header.block_size << 4 == (4096*2));
}
/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/

Test(sf_memsuite, Zero_Size_Allocation, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(0);
    cr_assert(x == NULL);
}
//2
Test(sf_memsuite, Struct_Info_for_one_page, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(4080);
    memset(x,1,4080);
    info inf;
    sf_info(&inf);
    cr_assert(inf.splintering == 0);
    cr_assert(inf.allocatedBlocks == 1);
    cr_assert(inf.peakMemoryUtilization == (4080/4096));
}
//3
Test(sf_memsuite, Free_list_Null_Allocation, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(4080);
    memset(x,1,4080);
    cr_assert(freelist_head == NULL);
}
//4
Test(sf_memsuite, Two_Full_Page_Allocation, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(8176);
    memset(x,1,8176);
    cr_assert(freelist_head == NULL);
}
//5

//6
Test(sf_memsuite, Re_Allocation, .init = sf_mem_init, .fini = sf_mem_fini) {
    int *value26 = sf_malloc(16);
    int *value28 = sf_malloc(16);
    int *value30 = sf_malloc(16);
    sf_free(value28);
    void *value32 = sf_realloc(value26,64);
    sf_header *castedvalue = (sf_header*)(value32-8);
    cr_assert(castedvalue->block_size<<4 == 80);
    sf_varprint(value30);
}
//7
Test(sf_memsuite, Check_Splinter_And_free, .init = sf_mem_init, .fini = sf_mem_fini) {
    void *x = sf_malloc(32);
    void* y = sf_malloc(4016);
    memset(x, 0, 32);
    memset(y, 0, 4000);
    cr_assert(((sf_header*)(y-8))->block_size << 4 == 4048);
}
//8
Test(sf_memsuite, Realloc_With_Splinter_coalesced, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(32);
    memset(x,1,32);
    void* y = sf_malloc(16);
    memset(y,1,16);
    void* z = sf_realloc(x,16); //Return same address
    cr_assert(x==z);
}
//9
Test(sf_memsuite, Realloc_giving_null, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(32);
    memset(x,1,32);
    void* y = sf_malloc(16);
    memset(y,1,16);
    void* z = sf_realloc(x,4096*4); //realloc giving null for more memory that 4096*4 bytes
    cr_assert(z==NULL);
    // cr_assert(errno = ENOMEM);
}
//12
Test(sf_memsuite, Check_ErrorNO_For_Invalid_Value, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(0);
    cr_assert(x==NULL);
    cr_assert(errno == EINVAL);//error is set to EINVAL
}
//13
Test(sf_memsuite, Freeing_A_Free_Block, .init = sf_mem_init, .fini = sf_mem_fini) {
    void* x = sf_malloc(0);
    sf_free(x);
    sf_free(x); // Trying to free a free block
    cr_assert(errno == EINVAL);//error is set to EINVAL
}
