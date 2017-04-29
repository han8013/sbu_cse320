#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "arraylist.h"
#include "foreach.h"
#include <unistd.h>
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

/******************************************
 *              HELPER FUNCS              *
 ******************************************/



/*****************************************
 *                  MY TESTS                *
 *****************************************/
/***********************************TEST1***********************************/
Test(al_suite, single_thread_single_al_creation, .timeout=2){
    /*CREATE A LIST*/
    arraylist_t* list1 = new_al(sizeof(student_t));
    /*CREATE A STRUCT*/
    student_t* s1 = calloc(1, sizeof(student_t));
    s1 -> name = "Wangshu";
    s1 -> id = 123;
    s1 -> gpa = 4.0;
    student_t* s2 = calloc(1, sizeof(student_t));
    s1 -> name = "Luo";
    s1 -> id = 456;
    s1 -> gpa = 4.0;

    int index = insert_al(list1, s1);
    int i1 = insert_al(list1, s2);
    cr_assert(index == 0, "Inserting is wrong, index is %d\n", index);
    cr_assert(i1 == 1, "Inserting is wrong, index is %d\n", i1);
    cr_assert(list1-> length == 2, "length wrong, index is %lu\n", list1-> length);
    cr_assert(list1-> capacity == INIT_SZ, "Capacity wrong, index is %lu\n", list1-> capacity);
    free(s1);
    free(s2);
}
/**********************************TEST2************************************/
arraylist_t* list1;
student_t* student[NUM_THREADS];
void* test_insert(void* index){
    student_t* student = (student_t*)(index);
    insert_al(list1, student);
    return NULL;
}
Test(al_suite, single_thread_multi_al_creation, .timeout=2){
    list1 = new_al(sizeof(student_t));
    int i;
    int ret_pthread_create;
    pthread_t threads[NUM_THREADS];
    for(i = 0; i < NUM_THREADS; i++){
        student[i] = malloc(sizeof(student_t));
        student[i]-> id = i;
        student[i]-> gpa = 4.0;
        ret_pthread_create = pthread_create(&threads[i], NULL, test_insert,student[i]);
        if(ret_pthread_create){
            printf("error: return from pthread_create\n");
            exit(-1);
        }
    }
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    cr_assert(list1-> length == NUM_THREADS, "list length is wrong, length is %lu\n", list1-> length);
    for(int i = 0; i < NUM_THREADS; i++){
        cr_assert(((student_t*)((char*)list1->base+i*list1->item_size)) -> gpa == 4.0,
            "insert is wrong, gpa is %f\n", student[i]->gpa);
        cr_assert(((student_t*)((char*)list1->base+i*list1->item_size))-> id < 5001,
            "insert wrong: id is greater than 500");
        cr_assert(((student_t*)((char*)list1->base+i*list1->item_size))-> id >= 0,
            "insert wrong: id is smaller than 0");
        free(student[i]);
    }
    delete_al(list1, NULL);
    free(list1);
 }
// /*******************************TEST3***************************************/
arraylist_t* list2;
test_item_t* test_item_list[NUM_THREADS];
int insert_index[NUM_THREADS];
void* test_insert_get(void* index){
    int y = *((int*) index);
    free(index);
    test_item_list[y]->i = y;
    insert_al(list2, test_item_list[y]);
    //printf("index is %d\n", y);
    insert_index[y] = get_data_al(list2,test_item_list[y]);
    //printf("get index is %d\n", insert_index[y]);
    return NULL;
}

Test(al_suite, multithread_test_4, .timeout=10){
    list2 = new_al(sizeof(test_item_t));
    pthread_t threads[NUM_THREADS];
    bool test_bool[NUM_THREADS];
    int i;
    int ret_pthread_create;
    int* y;

    for(i = 0; i < NUM_THREADS; i++){
        test_item_list[i] = malloc(sizeof(test_item_t));
        y = malloc(sizeof(int));
        *y = i;
        ret_pthread_create = pthread_create(&threads[i], NULL, test_insert_get, (void*)y);

        if(ret_pthread_create){
            printf("Error: return from pthread_create\n");
            exit(-1);
        }
    }
    //printf("it goes here\n");
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NUM_THREADS; i++){
        test_bool[insert_index[i]] = true;
    }
    for(int i = 0; i < NUM_THREADS; i++){
        cr_assert(test_bool[i] == true, "wrong\n");
        //printf("index free\n");
        free(test_item_list[i]);
    }
    delete_al(list2, NULL);
    free(list2);
}
// /********************************TEST4**************************************/
Test(al_suite, single_insert_get_index, .timeout = 10){
    arraylist_t *locallist = new_al(sizeof(test_item_t));
    test_item_t* t1 = (test_item_t*)malloc(sizeof(test_item_t));
    t1->i = 1;
    t1->f = 2.0f;
    t1->ld = 3.14;
    t1->c1 = t1->c2 = 1;
    t1->s = 2;
    t1->some_data = locallist->base;
    insert_al(locallist, t1);
    test_item_t* t2 = (test_item_t*)malloc(sizeof(test_item_t));
    t2->i = 3;
    t2->f = 4.2f;
    t2->ld = 2.71828;
    t2->c1 = t2->c2 = 2;
    t2->s = 1;
    t2->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t2);
    test_item_t* t3 = (test_item_t*)malloc(sizeof(test_item_t));
    t3->i = 5;
    t3->f = 7.8f;
    t3->ld = 1.4142;
    t3->c1 = t3->c2 = 3;
    t3->s = 4;
    t3->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t3);
    test_item_t* t4 = (test_item_t*)malloc(sizeof(test_item_t));
    t4->i = 9;
    t4->f = 3.04f;
    t4->ld = 1.732;
    t4->c1 = t4->c2 = 4;
    t4->s = 3;
    t4->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t4);
    printf("slkdflak\n");
    fflush(stdout);
    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 32;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t5);
    printf("5slkdflak\n");
    fflush(stdout);
    test_item_t* s1 = (test_item_t*)get_index_al(locallist, 0);
    cr_assert(t1->i == s1->i, "i value did not set properly, 0");
    cr_assert(t1->f == s1->f, "f value did not set properly, 0");
    cr_assert(t1->ld == s1->ld, "ld value did not set properly, 0");
    cr_assert(t1->c1 == s1->c1, "c1 value did not set properly, 0");
    cr_assert(t1->c2 == s1->c2, "c2 value did not set properly, 0");
    cr_assert(t1->s == s1->s, "s value did not set properly, 0");
    cr_assert(t1->some_data == s1->some_data, "some_data value did not set properly, 0");
    test_item_t* s2 = (test_item_t*)get_index_al(locallist, 1);
    cr_assert(t2->i == s2->i, "i value did not set properly, 1");
    cr_assert(t2->f == s2->f, "f value did not set properly, 1");
    cr_assert(t2->ld == s2->ld, "ld value did not set properly, 1");
    cr_assert(t2->c1 == s2->c1, "c1 value did not set properly, 1");
    cr_assert(t2->c2 == s2->c2, "c2 value did not set properly, 1");
    cr_assert(t2->s == s2->s, "s value did not set properly, 1");
    cr_assert(t2->some_data == s2->some_data, "some_data value did not set properly, 1");
    test_item_t* s3 = (test_item_t*)get_index_al(locallist, 2);
    cr_assert(t3->i == s3->i, "i value did not set properly, 2");
    cr_assert(t3->f == s3->f, "f value did not set properly, 2");
    cr_assert(t3->ld == s3->ld, "ld value did not set properly, 2");
    cr_assert(t3->c1 == s3->c1, "c1 value did not set properly, 2");
    cr_assert(t3->c2 == s3->c2, "c2 value did not set properly, 2");
    cr_assert(t3->s == s3->s, "s value did not set properly, 2");
    cr_assert(t3->some_data == s3->some_data, "some_data value did not set properly, 2");
    test_item_t* s4 = (test_item_t*)get_index_al(locallist, 3);
    cr_assert(t4->i == s4->i, "i value did not set properly, 3");
    cr_assert(t4->f == s4->f, "f value did not set properly, 3");
    cr_assert(t4->ld == s4->ld, "ld value did not set properly, 3");
    cr_assert(t4->c1 == s4->c1, "c1 value did not set properly, 3");
    cr_assert(t4->c2 == s4->c2, "c2 value did not set properly, 3");
    cr_assert(t4->s == s4->s, "s value did not set properly, 3");
    cr_assert(t4->some_data == s4->some_data, "some_data value did not set properly, 3");
    printf("7slkdflak\n");
    fflush(stdout);
    test_item_t* s5 = (test_item_t*)get_index_al(locallist, 5);
    cr_assert(t5->i == s5->i, "i value did not set properly, 4");
    cr_assert(t5->f == s5->f, "f value did not set properly, 4");
    cr_assert(t5->ld == s5->ld, "ld value did not set properly, 4");
    cr_assert(t5->c1 == s5->c1, "c1 value did not set properly, 4");
    cr_assert(t5->c2 == s5->c2, "c2 value did not set properly, 4");
    cr_assert(t5->s == s5->s, "s value did not set properly, 4");
    cr_assert(t5->some_data == s5->some_data, "some_data value did not set properly, 4");
    cr_assert(locallist->capacity == 8, "capacity did not double");
    cr_assert(locallist->length == 5, "length is not correct");
    printf("8slkdflak\n");
    fflush(stdout);
    test_item_t* s6 = (test_item_t*)get_index_al(locallist, 6);
    cr_assert(t5->i == s6->i, "i value did not set properly, 5");
    cr_assert(t5->f == s6->f, "f value did not set properly, 5");
    cr_assert(t5->ld == s6->ld, "ld value did not set properly, 5");
    cr_assert(t5->c1 == s6->c1, "c1 value did not set properly, 5");
    cr_assert(t5->c2 == s6->c2, "c2 value did not set properly, 5");
    cr_assert(t5->s == s6->s, "s value did not set properly, 5");
    cr_assert(t5->some_data == s6->some_data, "some_data value did not set properly, 5");
}


// /**********************************TEST6************************************/
Test(al_suite, single_remove_index_item, .timeout = 10){
    arraylist_t *locallist = new_al(sizeof(test_item_t));
    test_item_t* t1 = (test_item_t*)malloc(sizeof(test_item_t));
    t1->i = 1;
    t1->f = 2.0f;
    t1->ld = 3.14;
    t1->c1 = t1->c2 = 1;
    t1->s = 2;
    t1->some_data = locallist->base;
    insert_al(locallist, t1);
    test_item_t* t2 = (test_item_t*)malloc(sizeof(test_item_t));
    t2->i = 2;
    t2->f = 4.2f;
    t2->ld = 2.71828;
    t2->c1 = t2->c2 = 2;
    t2->s = 1;
    t2->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t2);
    test_item_t* t3 = (test_item_t*)malloc(sizeof(test_item_t));
    t3->i = 3;
    t3->f = 7.8f;
    t3->ld = 1.4142;
    t3->c1 = t3->c2 = 3;
    t3->s = 4;
    t3->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t3);
    test_item_t* t4 = (test_item_t*)malloc(sizeof(test_item_t));
    t4->i = 4;
    t4->f = 3.04f;
    t4->ld = 1.732;
    t4->c1 = t4->c2 = 4;
    t4->s = 3;
    t4->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t4);

    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 5;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t5);


    int x1 = get_data_al(locallist, t1);
    test_item_t* s1 = remove_index_al(locallist, x1);
    //int x3 = get_data_al(locallist, t3);
    test_item_t* s2 = remove_index_al(locallist, x1);
    int x5 = get_data_al(locallist, t5);
    test_item_t* s5 = remove_index_al(locallist, x5);

    cr_assert(t1->i == s1->i, "i value did not set properly, 0");
    cr_assert(t2->i == s2->i, "i value did not set properly, 0");
    cr_assert(t5->i == s5->i, "i value did not set properly, 0");
}


// /***********************TEST7 -- remove index al**********************/
student_t* s_list[NUM_THREADS];
arraylist_t* list3;
bool bool_test[NUM_THREADS];


void* test_multi_remove_index_item(void* index){
    student_t* student = (student_t*)index;
    student_t* s = get_index_al(list3, student->id);
    // bool bl = remove_data_al(list3, s);
    // printf("return is %d\n", bl);

    // int ix = remove_data_al(list3, student);
    bool_test[s->id] = true;

    // student_t* temp = remove_index_al(list3, student->id);
    // bool_test[temp-> id] = true;
    return NULL;
}


Test(al_suite, multi_remove_index_item, .timeout = 20){
    /*CREATE STUDENT LIST*/
    int i;
    int ret_pthread_create;
    pthread_t threads[NUM_THREADS];


    list3 = new_al(sizeof(student_t));
    for(i = 0; i < NUM_THREADS; i++){
        s_list[i] = malloc(sizeof(student_t));
        s_list[i] -> id = i;
        bool_test[i] = false;
        insert_al(list3, s_list[i]);
    }
    //sleep(15);
    printf("After inserting, the length is %lu\n", list3-> length);
    /*CREATE THREADS*/
    for(int i = 0; i < NUM_THREADS; i++){
        ret_pthread_create = pthread_create(&threads[i], NULL,
            test_multi_remove_index_item,s_list[i]);
        if(ret_pthread_create){
            printf("error: return from pthread_create\n");
            exit(-1);
        }
    }
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("after all join, the length is %lu\n", list3-> length);
    //cr_assert(list3->length == 0, "the length is %lu\n", list3->length);
    for(int i = 0; i< NUM_THREADS; i++){
        cr_assert(bool_test[i] == true, "index: %d bool value is wrong\n",i);
    }
}


/***********************************************************************
                                PART 2
***********************************************************************/
/*************************TEST10 foreach****************************/
// Test(al_suite, foreach, .timeout = 20){
//     arraylist_t* list5 = new_al(sizeof(student_t));
//     student_t* s_list2[NUM_THREADS];
//     student_t* temp;


//     for(int i = 0; i < NUM_THREADS; i++){
//         s_list2[i] = malloc(sizeof(char*));
//         s_list2[i]-> id = i;
//         insert_al(list5, s_list2[i]);
//     }

//     foreach(void, value, list5){
//         ((student_t*)value)-> id = ((student_t*)value)-> id+ 1;
//     }

//     for(int i = 0; i < NUM_THREADS; i++){
//         temp = get_index_al(list5, i);
//         cr_assert(temp-> id == i+1, "foreach is wrong %d\n", i);
//     }

//     for(int i = 0; i < NUM_THREADS; i++){
//         free(s_list2[i]);
//     }
// }





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
// #ifndef TEMP1
// #define TEMP1 500
// #endif

// #ifndef TEMP2
// #define TEMP2 50
// #endif

// typedef struct{
// int* i;
// arraylist_t* al;
// }mt_fe_t1_struct;

// void* mt_fe_t1_func(void* index){
//     mt_fe_t1_struct* new_struct = ((mt_fe_t1_struct*)index);
//     int i = *(new_struct->i);
//     // printf("HI, i = %d\n", i);
//     // fflush(stdout);
//     arraylist_t* al = new_struct->al;
//     // free(new_struct->i);
//     // free(index);
//     // printf("See, i = %d\n", i);
//     bool b = false;
//     foreach(student_t, value, al){
//         if(b)
//             value->id = i + 1;
//         else
//             value = NULL;
//         b = !b;
//     }
//     cr_assert(true);
//     // printf("Bye, i = %d\n", i);
//     return NULL;
// }



// Test(al_suite, mt_fe_t2, .timeout = 10){
//     arraylist_t* g1 = new_al(sizeof(student_t));
//     student_t *student = calloc(1,sizeof(student_t));
//     for(int i = 0; i < TEMP2; i++){
//         student->id = i + NUM_THREADS + TEMP1;
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
//         mt_fe_t1_struct* new_struct = malloc(sizeof(mt_fe_t1_struct));
//         new_struct->i = malloc(sizeof(int));
//         *(new_struct->i) = i;
//         new_struct->al = g1;
//         pthread_create(&threads[i], NULL, mt_fe_t1_func, new_struct);
//     }

//     for(int i = 0; i < NUM_THREADS; i++){
//         pthread_join(threads[i], NULL);
//     }
//     bool b = false;
//     int i = 0;
//     foreach(student_t, value, g1){
//         if(b)
//             cr_assert(value->id <= NUM_THREADS);
//         else
//             cr_assert(value->id == NUM_THREADS + TEMP1 + i);
//         b = !b;
//         i++;
//         // student_t* newVal = value + 1;
//         // printf("%d and newVal = NULL? %d\n", value->id, newVal == NULL);
//     }
//     cr_assert(i == TEMP2, "i = %d", i);
//     // fflush(stdout);
//     // printf("\ng1->length == %lu\n", g1->length);
//     // student_t* st = g1->base;
//     // st++;
//     // printf("st[1] == NULL? %d\n", st == NULL);
//     // cr_assert(g1->length == 0);
// }

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

Test(al_suite, multithread_test_1, .timeout = 10)
{
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
        cr_assert(((test_item_t*)((char*)list1->base + (i*list1->item_size))) -> i %2 == 0, "error");
    }

}
