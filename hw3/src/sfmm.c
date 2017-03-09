/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>

size_t get_alignsize(size_t size);
size_t get_padding_size(size_t size);


void coalesce(void *bp);
void *best_fit(size_t block_size);
void place(void *bp, size_t needed_size, size_t padding_size);
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
int sbrk_time = 1;

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
		start = sf_sbrk(0);

		start = sf_sbrk(1);
		end = sf_sbrk(0);
		void* start_head = start;
		/* set free list head */
		freelist_head = (sf_free_header*)start_head;
		freelist_head->header.alloc = 0;
		freelist_head->header.splinter = 0;
		freelist_head->header.block_size = (4096>>4);
		freelist_head->header.splinter_size = 0;
		freelist_head->header.padding_size = 0;

		freelist_head->next = NULL;
		freelist_head->prev = NULL;
		/* set free list footer*/
		void *end_footer = (char*)end - 8;
		sf_footer *firstfree_footer = (sf_footer*) end_footer;
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
				return ((char*)bp+8);
			}else{
				/* No fit found.Get more memory and place the block. */
				if (sbrk_time<4)
				{
				void *old_end = sf_sbrk(1);
				end = sf_sbrk(0);
				old_end = (char*)end-4096;
				if (end == NULL){
					return NULL;
				}
				set_freeheader(old_end,4096); /* need add new free header to freelist*/
				set_freefooter((char*)end-8,4096);
				insert_in_freelist(old_end);
				// printf("%s\n", "before coalesce---------------------------------------------------------");
				// sf_snapshot(true);
				coalesce(old_end);
				// printf("%s\n", "after coalesce---------------------------------------------------------");
				// sf_snapshot(true);
				}

			}
		}
	return NULL;
}

void coalesce(void *bp){
	/* bp is current location of pointer */
	size_t s = get_size(bp);
	void *end_footer = ((char*)bp+s);

	int next_is_alloc = get_alloc(get_next(bp)) || end_footer == end;
	int prev_is_alloc = get_alloc(get_prev(bp)) || bp == start;
	// size_t free_size = get_size(bp);
	// sf_free_header *free_block = (sf_free_header*) bp;

	/* case 1: only next is free block */
	if (prev_is_alloc && !next_is_alloc){
		sf_free_header *next_header = (sf_free_header*) get_next(bp);
		size_t size = get_size(bp) + (next_header->header.block_size<<4);
		remove_from_freelist(next_header);
		// printf("%s\n", "after remove ******************************************************************");
		// sf_snapshot(true);
		set_freeheader(bp,size);
		void *footer_location = (char*)bp+size-8;
		set_freefooter(footer_location,size);

	}
	/* case 2: only prev is free block */
	else if (!prev_is_alloc && next_is_alloc){
		sf_free_header *prev_header = (sf_free_header*) get_prev(bp);
		size_t size = get_size(bp) + (prev_header->header.block_size <<4);
		// void* prev_header = (void*)prev_footer + 8 - get_size(prev_footer);
		remove_from_freelist(bp);
		bp = prev_header;
		set_freeheader(bp,size);
		set_freefooter((char*)bp+size-8, size);

	}
	/* case 3: both prev and next are free block */
	else if (!prev_is_alloc && !next_is_alloc){
		sf_free_header *prev_free = (sf_free_header*) get_prev(bp);
		sf_free_header *next_free = (sf_free_header*) get_next(bp);
		size_t size = get_size(bp) + (prev_free->header.block_size<<4) + (next_free->header.block_size<<4);
		remove_from_freelist(bp);
		remove_from_freelist(next_free);
		bp = prev_free;
		set_freeheader(bp,size);
		set_freefooter((char*)bp+size-8,size);
	}
	insert_in_freelist(bp);

}


void *get_next(void *bp){
	return (char*)bp+get_size(bp);
}

void *get_prev(void *bp){
	return (char*)bp-get_size((char*)bp-8);
}

size_t get_alloc(void* bp){
	return (*(unsigned long*)(bp)) & 0x1;
}

size_t get_size(void* bp){
	return (*(unsigned long*)bp) & 0xfffffff0;
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
			else if (current_size == min_size)
			{
				if (found_free < top_free)
				{
					found_free = top_free;
				}
			}
		}
	}
	return found_free;
}

void place(void *bp, size_t needed_size, size_t padding_size){
	sf_free_header *found = (sf_free_header*) bp;
	size_t total_free = found->header.block_size<<4;
	/* check if split block*/
	if (total_free-needed_size>=32){
		size_t new_free = total_free-needed_size;
		set_header(bp,needed_size,padding_size,0);
		set_footer((char*)bp+needed_size-8,needed_size,0);
		remove_from_freelist(bp);
		bp = (char*)bp + needed_size;
		set_freeheader(bp,new_free);
		set_freefooter((char*)bp+new_free-8,new_free);
		insert_in_freelist(bp);

	}
	else if (total_free-needed_size<32){
		size_t splinter_size = total_free-needed_size;
		set_header(bp,total_free,padding_size,splinter_size);
		set_footer((char*)bp+total_free-8,total_free,splinter_size);
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
		sf_free_header *start_free;
		sf_free_header *end_header;
		for (start_free = freelist_head; start_free!=NULL; start_free = start_free->next){
			end_header = start_free;
			if (insert<start_free)
			{
				insert->next = start_free;
				start_free->prev = insert;
				insert->prev = NULL;
				freelist_head = insert;
				return;
			}
			else if (insert==start_free)
			{
				return;
			}
			else if (start_free<insert && insert<start_free->next){
				insert->next = start_free->next;
				start_free->next->prev = insert;
				insert->prev = start_free;
				start_free->next = insert;
				return;
			}
		}
		end_header->next = insert;
		insert->prev = end_header;
		insert->next = NULL;
	}
}
/*Removes the free block pointer int the free_list*/
void remove_from_freelist(void *bp){
	// sf_free_header *remove = (sf_free_header*)bp;
	// remove->header.block_size = 0;
	sf_free_header *prev_free = ((sf_free_header*)bp)->prev;
	sf_free_header *next_free = ((sf_free_header*)bp)->next;
	if (prev_free!=NULL){
		prev_free->next = next_free;
		((sf_free_header*)bp)->prev = NULL;
	}
	else{
		freelist_head = next_free;
	}
	if (next_free!=NULL)
	{
		next_free->prev = prev_free;
		((sf_free_header*)bp)->next = NULL;
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
	sf_footer* new_footer = (sf_footer*) bp;
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
	/* chcek if is null*/
	ptr = (char*)ptr-8;
	if (ptr == NULL){
		return;
	}
	size_t free_size = get_size(ptr);
	set_freeheader(ptr,free_size);
	void *footer_location = (char*)ptr+free_size-8;
	set_freefooter(footer_location,free_size);
	insert_in_freelist(ptr);
	// printf("%s\n", "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	// sf_snapshot(true);
	coalesce(ptr);


}

int sf_info(info* ptr) {
	return -1;
}
