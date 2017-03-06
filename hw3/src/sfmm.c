/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>


/* Basic constants and macros */
#define WSIZE sizeof(void *) /* Word and header/footer size (bytes) */
#define DSIZE (2*WSIZE) /* Double word size (bytes) */
#define PAGE (4096) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & 0xfffffff0)
#define GET_ALLOC(p) (p & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLOCK(bp) ((char *)(bp) + GET_SIZE((char *)bp)
#define PREV_BLOCK(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - WSIZE)))

size_t get_alignsize(size_t size);
size_t get_padding_size(size_t size);


void coalesce(void *bp);
void *best_fit(size_t block_size);
void place(void *bp, size_t block_size, size_t padding_size);
void insert_in_freelist(void *bp);
void remove_from_freelist(void *bp);

void set_freeheader(void* location, size_t block_size);
void set_freefooter(void* location, size_t block_size);
void set_header(void *bp, size_t block_size, size_t padding, size_t splinter_size);
void set_footer(void *bp, size_t block_size,size_t splinter_size);

void *get_next(void *bp);
void *get_prev(void *bp);
size_t get_alloc(void* bp);
size_t get_size(void* bp);



/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */

sf_free_header* freelist_head = NULL;
void *start;
void *end;
int is_start = 0;


void *sf_malloc(size_t size) {
	void *bp = NULL;
	if(size <= 0){
		return NULL;
	}
	if(size > ((4096*4)-16)){
		return NULL;
	}
	if (is_start == 0){
		/* request heap by sbrk*/
		end = sf_sbrk(1);
		start = end-4096;
		/* set free list head */
		freelist_head = (sf_free_header*)start;
		freelist_head->header.alloc = 0;
		freelist_head->header.splinter = 0;
		freelist_head->header.block_size = (4096>>4);
		freelist_head->header.splinter_size = 0;
		freelist_head->header.padding_size = 0;

		freelist_head->next = NULL;
		freelist_head->prev = NULL;
		/* set free list footer*/
		void *end_heap = end - 8;
		sf_footer *firstfree_footer = (sf_footer*) end_heap;
		firstfree_footer->alloc = 0;
		firstfree_footer->splinter = 0;
		firstfree_footer->block_size = (4096>>4);

		is_start = 1; /* just for once*/
	}
		// padding size.
		// 16 is size of sum of footer and header
		size_t alignsize = get_alignsize(size);
		size_t padding_size = get_padding_size(size);

		// block size by adding padding,header and footer
		size_t block_size = alignsize+16;

		/* Search the free list for a fit. */

		while(bp==NULL){
			bp = best_fit(block_size);
			if (bp != NULL) {
				place(bp, block_size,padding_size);
				return (bp+8);
			}else{
				/* No fit found.Get more memory and place the block. */
				end = sf_sbrk(1);
				if (end == NULL){
					return NULL;
				}
				set_freeheader(end-4096,4096);
				set_freefooter(end-8,4096);
				coalesce(end-4096);
			}

		}
	return NULL;
}

void coalesce(void *bp){

	size_t s = get_size(bp);
	void *test = (bp+s);

	int next_is_alloc = get_alloc(get_next(bp)) || test == end;
	int prev_is_alloc = get_alloc(get_prev(bp)) || bp == start;
	// size_t free_size = get_size(bp);
	sf_free_header *free_block = (sf_free_header*) bp;

	/* case 1 only next is free block */
	if (prev_is_alloc && !next_is_alloc){
		sf_free_header *next_free = free_block->next;
		size_t size = get_size(bp) + next_free->header.block_size;
		set_freeheader(bp,size);
		set_freefooter(bp,size);
		remove_from_freelist(next_free);

	}
	/* case 2 only prev is free block */
	else if (!prev_is_alloc && next_is_alloc){
		sf_footer *prev_free = (sf_footer*)((void*)free_block - 8);
		size_t size = get_size(bp) + prev_free->block_size;
		remove_from_freelist((void*)prev_free + 8 - get_size(prev_free));
		bp = prev_free;
		set_freeheader(bp,size);
		set_freefooter(bp,size);

	}
	/* case 3 both prev and next are free block */
	else if (!prev_is_alloc && !next_is_alloc){
		sf_free_header *prev_free = free_block->prev;
		sf_free_header *next_free = free_block->next;
		size_t size = get_size(bp) + prev_free->header.block_size + next_free->header.block_size;
		remove_from_freelist(prev_free);
		remove_from_freelist(next_free);
		bp = prev_free;
		set_freeheader(bp,size);
		set_freefooter(bp,size);
	}
	insert_in_freelist(bp);

}


void *get_next(void *bp){
	return bp+get_size(bp);
}

void *get_prev(void *bp){
	return bp-get_size(bp-8);
}

size_t get_alloc(void* bp){
	return (*(unsigned*)(bp)) & 0x1;
}

size_t get_size(void* bp){
	return (*(unsigned*)bp) & 0xfffffff0;
}

void *best_fit(size_t block_size){
	sf_free_header *top_free;
	sf_free_header *found_free = NULL;
	size_t current_size;
	size_t min_size = -1;
	for (top_free = freelist_head; top_free!=NULL; top_free = top_free->next)
	{
		current_size = top_free->header.block_size<<4;
		if (block_size <= current_size)
		{
			if (min_size == -1)
			{
				found_free = top_free;
				min_size = current_size;
			}
			else if (current_size < min_size)
			{
				found_free = top_free;
				min_size = current_size;
			}
		}
	}
	return found_free;
}

void place(void *bp, size_t block_size, size_t padding_size){
	sf_free_header *found = (sf_free_header*) bp;
	size_t total_free = found->header.block_size<<4;
	/* check if split block*/
	if (total_free-block_size>=32){
		size_t new_free = total_free-block_size;
		set_header(bp,block_size,padding_size,0);
		set_footer(bp,block_size,0);
		remove_from_freelist(bp);
		bp = bp + block_size;
		set_freeheader(bp,new_free);
		set_freefooter(bp,new_free);

		if (freelist_head == NULL)
		{
			insert_in_freelist(bp);
		}
		// coalesce(bp);
	}
	else if (total_free-block_size<32){
		size_t splinter_size = total_free-block_size;
		set_header(bp,total_free,padding_size,splinter_size);
		set_footer(bp,total_free,splinter_size);
		remove_from_freelist(bp);
	}

}


/*Inserts the free block pointer int the free_list*/
void insert_in_freelist(void *bp){
	sf_free_header *insert = (sf_free_header*) bp;
	if(freelist_head == NULL){
		freelist_head = insert;
		freelist_head->prev = NULL;
		freelist_head->next = NULL;
	}
	else{
	insert->next = freelist_head;
	freelist_head->prev = insert;
	insert->prev = NULL;
	freelist_head = insert;
	}
}
/*Removes the free block pointer int the free_list*/
void remove_from_freelist(void *bp){
	sf_free_header *prev_free = ((sf_free_header*)bp)->prev;
	sf_free_header *next_free = ((sf_free_header*)bp)->next;
	if (prev_free!=NULL){
		prev_free->next = next_free;
	}
	else{
		freelist_head = next_free;
	}
	if (next_free!=NULL)
	{
		next_free->prev = prev_free;
	}
}

void set_freeheader(void *location, size_t block_size){
	sf_free_header* new_header = (sf_free_header*) location;
	new_header->header.alloc = 0;
	new_header->header.splinter = 0;
	new_header->header.block_size = block_size>>4;
	new_header->header.padding_size = 0;
	new_header->header.splinter_size = 0;

}

void set_freefooter(void *location, size_t block_size){
	sf_footer* new_footer = (sf_footer*) location;
	new_footer->alloc = 0;
	new_footer->splinter = 0;
	new_footer->block_size = block_size>>4;
}

void set_header(void *bp, size_t block_size, size_t padding, size_t splinter_size){
	sf_header* new_header = (sf_header*) bp;
	new_header->alloc = 1;
	new_header->block_size = block_size>>4;
	new_header->requested_size = block_size - padding - splinter_size - 16 ;
	new_header->padding_size = padding;
	if (splinter_size!=0){
		new_header->splinter = 1;
		new_header->splinter_size = splinter_size;
	}
	else{
		new_header->splinter = 0;
		new_header->splinter_size = 0;
	}
}

void set_footer(void *bp, size_t block_size,size_t splinter_size){
	sf_footer* new_footer = (sf_footer*) (bp+block_size-8);
	new_footer->alloc = 1;
	new_footer->block_size = block_size>>4;
	if (splinter_size!=0){
		new_footer->splinter = 1;
	}
	else{
		new_footer->splinter = 0;
	}
}

size_t get_alignsize(size_t size){
	if(size%16 == 0){
		return size;
	}
	else{
		size_t remainder = size % 16;
		return (size + 16 - remainder);
	}
}

size_t get_padding_size(size_t size){

	if(size%16 == 0){
		return 0;
	}
	else
		return (16-size%16);
}




void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void* ptr) {

}

int sf_info(info* ptr) {
	return -1;
}
