#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include "csapp.h"
#include "foreach.h"

arraylist_t *global_list_t;
#ifndef NUM_THREADS
#define NUM_THREADS 50
#endif
typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

typedef struct{
    int i;
    float f;
    long double ld;
    char c1:4;
    char c2:4;
    short s;
    void *some_data;
}test_item;

void *thread_insert(void *data);
void *thread_remove();

test_item *test_list[50];
sem_t mutex1;

typedef struct{
int* i;
arraylist_t* al;
}mt_fe_t1_struct1;

#ifndef TEMP1
#define TEMP1 500
#endif

#ifndef TEMP2
#define TEMP2 50
#endif

typedef struct{
int* i;
arraylist_t* al;
}mt_fe_t1_struct;

void* mt_fe_t1_func(void* index){
    mt_fe_t1_struct* new_struct = ((mt_fe_t1_struct*)index);
    int i = *(new_struct->i);
    printf("HI, i = %d\n", i);
    fflush(stdout);
    arraylist_t* al = new_struct->al;
    free(new_struct->i);
    free(index);
    printf("See, i = %d\n", i);
    bool b = false;
    foreach(student_t, value, al){
        if(b)
            value->id = i + 1;
        else
            value = NULL;
        b = !b;
    }
    // cr_assert(true);
    printf("Bye, i = %d\n", i);
    return NULL;
}

typedef struct{
    int i;
    float f;
    long double ld;
    char c1:4;
    char c2:4;
    short s;
    void *some_data;
}test_item_t;

test_item_t* struct_list[NUM_THREADS];
arraylist_t* list1;

void* test_remove_index(void *index)
{
    int y = *((int*)index);
    free(index);
    struct_list[y]->i = y;
    insert_al(list1,struct_list[y]);

    if(struct_list[y]->i % 2 != 0){
        int x = get_data_al(list1, struct_list[y]);
        remove_index_al(list1,x);
    }

    return NULL;
}

int main(int argc, char* argv[]){
    list1 = new_al(sizeof(test_item_t));

    pthread_t threads[NUM_THREADS];
    //bool test_bool[NUM_THREADS];
    int thread_child;
    int i;

    for(i = 0  ; i < NUM_THREADS; i++)
    {
        struct_list[i] = malloc(sizeof(test_item_t));
        int* y = malloc(sizeof(int));
        *y = i;
        thread_child = pthread_create(&threads[i],NULL,test_remove_index,(void*)y);
        if (thread_child)
        {
            printf("ERROR; return code from pthread_create() is %d\n", thread_child);
            exit(-1);
        }
    }
    for(int i = 0 ; i < NUM_THREADS ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    for(int i = 0 ; i < NUM_THREADS/2 ; i++)
    {
        printf("left %d\n", ((test_item_t*)((char*)list1->base + (i*list1->item_size))) -> i);
        fflush(stdout);
        //cr_assert(((test_item_t*)((char*)list1->base + (i*list1->item_size))) -> i %2 == 0, "error");
    }
}






