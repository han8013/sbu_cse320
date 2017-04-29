#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "arraylist.h"
#include "foreach.h"
#include <unistd.h>
#include <error.h>
//#include <string.h>
#ifndef NUM_THREADS
#define NUM_THREADS 5000
#endif
// /******************************************
//  *                  ITEMS                 *
//  ******************************************/
arraylist_t *global_list;
arraylist_t *global_list2;

typedef struct {
    char* name;
    int id;
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
}test_item_t;



/***********************************************************************
                                PART 2
***********************************************************************/
/*************************TEST10 foreach****************************/
Test(al_suite, foreach, .timeout = 20){
    arraylist_t* list5 = new_al(sizeof(student_t));
    student_t* s_list2[NUM_THREADS];
    student_t* temp;


    for(int i = 0; i < NUM_THREADS; i++){
        s_list2[i] = malloc(sizeof(char*));
        s_list2[i]-> id = i;
        insert_al(list5, s_list2[i]);
    }

    foreach(void, value, list5){
        ((student_t*)value)-> id = ((student_t*)value)-> id+ 1;
        printf("still in the loop:\n");
    }
    printf("compare:\n");
    for(int i = 0; i < NUM_THREADS; i++){
        printf("get_index_al:\n");
        temp = get_index_al(list5, i);
        cr_assert(temp-> id == i+1, "foreach is wrong %d\n", i);
    }
    printf("end of compare:\n");
    for(int i = 0; i < NUM_THREADS; i++){
        printf("free:\n");
        free(s_list2[i]);
    }
    printf("test 1 end:\n");
}





// typedef struct{
// int* i;
// arraylist_t* al;
// }mt_fe_t1_struct1;

// void* mt_fe_t1_func1(void* index){
//     mt_fe_t1_struct1* new_struct = ((mt_fe_t1_struct1*)index);
//     int i = *(new_struct->i);
//     // printf("HI, i = %d\n", i);
//     // fflush(stdout);
//     arraylist_t* al = new_struct->al;
//     // free(new_struct->i);
//     // free(index);
//     // printf("See, i = %d\n", i);
//     foreach(student_t, value, al){
//         value->id = i + 1;
//     }
//     // printf("Bye, i = %d\n", i);
//     return NULL;
// }



// Test(al_suite, mt_fe_t1, .timeout = 50){
//     arraylist_t* g1 = new_al(sizeof(student_t));
//     student_t *student = calloc(1,sizeof(student_t));
//     for(int i = 0; i < 100; i++){
//         student->id = i + 6000;
//         insert_al(g1,student);
//     }
//     // printf("%lu\n", g1->length);
//     // for(int i = 0; i < 10; i++){
//     //     student_t* student = g1->base;
//     //     printf("%d\n", student[i].id);
//     //     fflush(stdout);
//     // }
//     // fflush(stdout);

//     pthread_t threads[NUM_THREADS];
//     for(int i = 0 ; i < NUM_THREADS; i++)
//     {
//         mt_fe_t1_struct1* new_struct = malloc(sizeof(mt_fe_t1_struct1));
//         new_struct->i = malloc(sizeof(int));
//         *(new_struct->i) = i;
//         new_struct->al = g1;
//         pthread_create(&threads[i], NULL, mt_fe_t1_func1, new_struct);
//     }

//     for(int i = 0; i < NUM_THREADS; i++){
//         pthread_join(threads[i], NULL);
//     }
//     foreach(student_t, value, g1){
//         //cr_assert(value->id <= 5000);
//         student_t* newVal = value + 1;
//         printf("%d and newVal = NULL? %d\n", value->id, newVal == NULL);
//     }
//     // fflush(stdout);
//     printf("\ng1->length == %lu\n", g1->length);
//     student_t* st = g1->base;
//     st++;
//     printf("st[1] == NULL? %d\n", st == NULL);
//     // cr_assert(g1->length == 0);
// }

/**********************KK*********************/
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
    // printf("HI, i = %d\n", i);
    // fflush(stdout);
    arraylist_t* al = new_struct->al;
    // free(new_struct->i);
    // free(index);
    // printf("See, i = %d\n", i);
    bool b = false;
    foreach(student_t, value, al){
        if(b)
            value->id = i + 1;
        else
            value = NULL;
        b = !b;
    }
    cr_assert(true);
    // printf("Bye, i = %d\n", i);
    return NULL;
}



Test(al_suite, mt_fe_t2, .timeout = 10){
    arraylist_t* g1 = new_al(sizeof(student_t));
    student_t *student = calloc(1,sizeof(student_t));
    for(int i = 0; i < TEMP2; i++){
        student->id = i + NUM_THREADS + TEMP1;
        insert_al(g1,student);
    }
    // printf("%lu\n", g1->length);
    // for(int i = 0; i < 10; i++){
    //     student_t* student = g1->base;
    //     printf("%d\n", student[i].id);
    //     fflush(stdout);
    // }
    // fflush(stdout);

    pthread_t threads[NUM_THREADS];
    for(int i = 0 ; i < NUM_THREADS; i++)
    {
        mt_fe_t1_struct* new_struct = malloc(sizeof(mt_fe_t1_struct));
        new_struct->i = malloc(sizeof(int));
        *(new_struct->i) = i;
        new_struct->al = g1;
        pthread_create(&threads[i], NULL, mt_fe_t1_func, new_struct);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    bool b = false;
    int i = 0;
    foreach(student_t, value, g1){
        if(b)
            cr_assert(value->id <= NUM_THREADS);
        else
            cr_assert(value->id == NUM_THREADS + TEMP1 + i);
        b = !b;
        i++;
        student_t* newVal = value + 1;
        printf("%d and newVal = NULL? %d\n", value->id, newVal == NULL);
    }
    cr_assert(i == TEMP2, "i = %d", i);
    fflush(stdout);
    printf("\ng1->length == %lu\n", g1->length);
    student_t* st = g1->base;
    st++;
    printf("st[1] == NULL? %d\n", st == NULL);
    cr_assert(g1->length == 50);
}

// test_item_t* struct_list[NUM_THREADS];
// arraylist_t* list1;

// void* test_remove_index(void *index)
// {
//     int y = *((int*)index);
//     free(index);
//     struct_list[y]->i = y;
//     insert_al(list1,struct_list[y]);

//     if(struct_list[y]->i % 2 != 0){
//         int x = get_data_al(list1, struct_list[y]);
//         remove_index_al(list1,x);
//     }

//     return NULL;
// }

// Test(al_suite, multithread_test_1, .timeout = 10)
// {
//     list1 = new_al(sizeof(test_item_t));

//     pthread_t threads[NUM_THREADS];
//     //bool test_bool[NUM_THREADS];
//     int thread_child;
//     int i;

//     for(i = 0  ; i < NUM_THREADS; i++)
//     {
//         struct_list[i] = malloc(sizeof(test_item_t));
//         int* y = malloc(sizeof(int));
//         *y = i;
//         thread_child = pthread_create(&threads[i],NULL,test_remove_index,(void*)y);
//         if (thread_child)
//         {
//             printf("ERROR; return code from pthread_create() is %d\n", thread_child);
//             exit(-1);
//         }
//     }
//     for(int i = 0 ; i < NUM_THREADS ; i++)
//     {
//         pthread_join(threads[i],NULL);
//     }

//     for(int i = 0 ; i < NUM_THREADS/2 ; i++)
//     {
//         printf("left %d\n", ((test_item_t*)((char*)list1->base + (i*list1->item_size))) -> i);
//         fflush(stdout);
//         cr_assert(((test_item_t*)((char*)list1->base + (i*list1->item_size))) -> i %2 == 0, "error");
//     }

// }

