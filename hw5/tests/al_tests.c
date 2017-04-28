#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include <semaphore.h>
#include <pthread.h>
/******************************************
 *                  ITEMS                 *
 ******************************************/
arraylist_t *global_list;

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
}test_item_t;

/**/
arraylist_t* list1;
#define NUM_THREADS 5000
test_item_t* test_item_list[NUM_THREADS];
int insert_index[NUM_THREADS];

/******************************************
 *              HELPER FUNCS              *
 ******************************************/
void test_item_t_free_func(void *argptr){
    test_item_t* ptr = (test_item_t*) argptr;
    if(!ptr)
        free(ptr->some_data);
    // else
    //     cr_log_warn("%s\n", "Pointer was NULL");
}

void setup(void) {
    // cr_log_warn("Setting up test");
    global_list = new_al(sizeof(test_item_t));
}

void teardown(void) {
    // cr_log_error("Tearing down");
    delete_al(global_list, test_item_t_free_func);
}

arraylist_t* global_list8;
arraylist_t* global_list2;
arraylist_t* global_list7;
test_item_t* gt2[5000];
test_item_t* gt[500];

test_item_t* w[5000];
size_t k[5000];
int x[500];
typedef struct{
    int index;
    sem_t mutex_add;
} sem_struct;

void* test_insert_get2(void *index)
{
    int y = *((int*)index);
    free(index);
    gt2[y]->i = y;
    k[y] = insert_al(global_list8,gt2[y]);
    w[y] = get_index_al(global_list8,k[y]);
    // printf("y = %d and x[y] = %d\n", y, x[y]);
    // fflush(stdout);
    return NULL;
}

void* delete_one_from_list(void* mutex)
{
    sem_wait(&((sem_struct*)mutex)->mutex_add);
    // printf("remove :%d\n", *(int*)index);
    remove_index_al(global_list2, 0);
    return NULL;
}


void* add_one_to_list(void* mutex)
{
    student_t *test = calloc(1, sizeof(student_t));
    insert_al(global_list2, test);
    free(test);
    mutex = (sem_struct*)mutex;
    sem_post(&((sem_struct*)mutex)->mutex_add);
    int x = 1000;

    sem_getvalue(&((sem_struct*)mutex)->mutex_add, &x);
    // printf("add: %d\n", x);
    return NULL;
}

void* test_insert_get(void *index)
{
    int y = *((int*)index);
    free(index);
    gt[y]->i = y;
    insert_al(global_list7,gt[y]);
    x[y] = get_data_al(global_list7,gt[y]);

    return NULL;
}

void* test_insert_get_lll(void* index){
    int y = *((int*) index);
    free(index);
    test_item_list[y]->i = y;
    insert_al(list1, test_item_list[y]);
    // printf("index is %d\n", y);
    insert_index[y] = get_data_al(list1,test_item_list[y]);
    // printf("get index is %d\n", insert_index[y]);
    return NULL;
}

/******************************************
 *                  TESTS                 *
 ******************************************/
// Test(al_suite, 0_creation, .timeout=2){
//     arraylist_t *locallist = new_al(sizeof(test_item_t));

//     cr_assert_not_null(locallist, "List returned was NULL");
// }

// Test(al_suite, 1_deletion, .timeout=2){
//     arraylist_t *locallist = new_al(sizeof(test_item_t));

//     cr_assert_not_null(locallist, "List returned was NULL");

//     delete_al(locallist, test_item_t_free_func);

//     cr_assert(true, "Delete completed without crashing");
// }

// Test(al_suite, 2_insertion, .timeout=2, .init=setup, .fini=teardown){
//     cr_assert(true, "I win");
// }

// Test(al_suite, 3_removal, .timeout=2, .init=setup, .fini=teardown){
// }


Test(al_suite, 4_insert, .timeout=2, .init=setup, .fini=teardown){
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
    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 32;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t5);

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
    test_item_t* s6 = (test_item_t*)get_index_al(locallist, 6);
    cr_assert(t5->i == s6->i, "i value did not set properly, 5");
    cr_assert(t5->f == s6->f, "f value did not set properly, 5");
    cr_assert(t5->ld == s6->ld, "ld value did not set properly, 5");
    cr_assert(t5->c1 == s6->c1, "c1 value did not set properly, 5");
    cr_assert(t5->c2 == s6->c2, "c2 value did not set properly, 5");
    cr_assert(t5->s == s6->s, "s value did not set properly, 5");
    cr_assert(t5->some_data == s6->some_data, "some_data value did not set properly, 5");
}

Test(al_suite, 8_insert, .timeout=2, .init=setup, .fini=teardown){
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
    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 32;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t5);
    size_t s1 = get_data_al(locallist, t1);
    cr_assert(s1 == 0, "get index not correct. s1");
    size_t s2 = get_data_al(locallist, t2);
    cr_assert(s2 == 1, "get index not correct. s2");
    size_t s3 = get_data_al(locallist, t3);
    cr_assert(s3 == 2, "get index not correct. s3");
    size_t s4 = get_data_al(locallist, t4);
    cr_assert(s4 == 3, "get index not correct. s4");
    size_t s5 = get_data_al(locallist, t5);
    cr_assert(s5 == 4, "get index not correct. s5");
    size_t s6 = get_data_al(locallist, NULL);
    cr_assert(s6 == 0, "get index not correct. s6");
    test_item_t* t6 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 18;
    t5->f = 70.1f;
    t5->ld = 91.02;
    t5->c1 = t5->c2 = 6;
    t5->s = 5;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    size_t s7 = get_data_al(locallist, t6);
    cr_assert(s7 == UINT_MAX, "get index not correct. s7");
}

Test(al_suite, test_remove_index, .timeout=30){

    arraylist_t *locallist1 = new_al(sizeof(student_t));
    student_t* student_ary[1000];

    student_t *student;

    for(int i = 0; i < 1000 ; i++)
    {
        student = calloc(1,sizeof(student_t));
        student->gpa = i;
        student_ary[i] = student;
        cr_assert(i == insert_al(locallist1,student), "%d\n", i);
        cr_assert(locallist1->length == i+1);
    }

    for(int i = 999 ; i >= 0 ; i--)
    {
        student_t *removed_student = remove_index_al(locallist1,i);
        // printf("remove index %d\n", i);
        cr_assert(memcmp(removed_student, student_ary[i], locallist1->item_size) == 0, "%f---%f\n", removed_student->gpa, student_ary[i]->gpa);
    }
    cr_assert(locallist1->capacity == 4);
    cr_assert(locallist1->length == 0);
}

Test(al_suite, test_remove_shifting_index, .timeout=30){

    arraylist_t *locallist1 = new_al(sizeof(student_t));
    student_t* student_ary[1000];

    student_t *student;

    for(int i = 0; i < 1000 ; i++)
    {
        student = calloc(1,sizeof(student_t));
        student->gpa = i;
        student_ary[i] = student;
        cr_assert(i == insert_al(locallist1,student), "%d\n", i);
        cr_assert(locallist1->length == i+1);
    }

    for(int i = 0 ; i < 1000 ; i++)
    {
        student_t *removed_student = remove_index_al(locallist1,0);
        cr_assert(memcmp(removed_student, student_ary[i], locallist1->item_size) == 0, "%f---%f\n", removed_student->gpa, student_ary[i]->gpa);
    }
    cr_assert(locallist1->capacity == 4);
    cr_assert(locallist1->length == 0);
}



Test(al_suite, multithread_test_4, .timeout = 4){
    global_list7 = new_al(sizeof(test_item_t));
    pthread_t threads[500];
    int rc;
    int i;

    for(i = 0  ; i < 500; i++){
        gt[i] = malloc(sizeof(test_item_t));
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,test_insert_get,(void*)y);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for(int i = 0 ; i < 500 ; i++){
        pthread_join(threads[i],NULL);
    }
    bool test_bool[500] = {0};
    for(int i = 0 ; i < 500 ; i++){
        //printf("%d\n", x[i]);
        cr_assert(!test_bool[x[i]], "i = %d, x[i] = %d", i, x[i]);
        test_bool[x[i]] = true;
    }
    for(int i = 0 ; i < 500 ; i++){
        cr_assert(test_bool[gt[i]->i], "i = %d, gt[i]->i = %d", i, gt[i]->i);
        test_bool[gt[i]->i] = false;
    }
}

Test(al_suite, multithread_test_4444, .timeout=10){
    list1 = new_al(sizeof(test_item_t));
    pthread_t threads[NUM_THREADS];
    bool test_bool[NUM_THREADS];
    int i;
    int ret_pthread_create;
    int* y;

    for(i = 0; i < NUM_THREADS; i++){
        test_item_list[i] = malloc(sizeof(test_item_t));
        y = malloc(sizeof(int));
        *y = i;
        ret_pthread_create = pthread_create(&threads[i], NULL, test_insert_get_lll, (void*)y);

        if(ret_pthread_create){
            printf("Error: return from pthread_create\n");
            exit(-1);
        }
    }
    printf("it goes here\n");
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NUM_THREADS; i++){
        test_bool[insert_index[i]] = true;
    }
    for(int i = 0; i < NUM_THREADS; i++){
        cr_assert(test_bool[i] == true, "wrong\n");
        free(test_item_list[i]);
    }
    free(list1->base);
    free(list1);
    delete_al(list1, NULL);
}

Test(al_suite, multithread_test_add_delete, .timeout = 10){
    pthread_t threads[5000];
    int rc;
    long i;

    global_list= new_al(sizeof(test_item_t));
    global_list2 = new_al(sizeof(test_item_t));

    volatile int j = 0;
    for(i = 0  ; i < 2500; i++){
        sem_struct* struct_mutex = malloc(sizeof(sem_struct));
        sem_init(&(struct_mutex->mutex_add), 0, 0);
        struct_mutex->index = i;
        rc = pthread_create(&threads[j],NULL,add_one_to_list, &struct_mutex);
        j++;
        rc = pthread_create(&threads[j],NULL,delete_one_from_list, &struct_mutex);

        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for(int i = 0 ; i < 2500 ; i++){
        pthread_join(threads[i],NULL);
    }
    //printf("1.%lu\n",global_list->length);
    //printf("2.%lu\n",global_list2->length);
    cr_assert(global_list->length == 0, "%-----------zu\n", global_list->length);
}

Test(al_suite, threads_test_2, .timeout = 40)
{
    global_list8 = new_al(sizeof(test_item_t));

    // memset(x, 0, );
    // student_t *t = calloc(1,sizeof(student_t));
    pthread_t threads[5000];
    // bool test_bool[NUM_THREADS];
    int rc;
    int i;

    for(i = 0  ; i < 5000; i++)
    {
        gt2[i] = malloc(sizeof(test_item_t));
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,test_insert_get2,(void*)y);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
       }
    }

    for(int i = 0 ; i < 5000 ; i++){
        pthread_join(threads[i],NULL);
    }
    // printf("%p\n", global_list8->base);
    for(int i = 0 ; i < 5000 ; i++){
        // printf("%d\n", i);
        //cr_assert(memcmp(gt2[i], w[i], sizeof(test_item_t)) == 0);
        test_item_t* p = global_list8->base;
        p += k[i];
        //cr_assert(memcmp(p, gt2[i], sizeof(test_item_t)) == 0, "i is %d, p->i is %d, and gt2[i]->i is %d\n", i, p->i, gt2[i]->i);
        //cr_assert(memcmp(p, w[i], sizeof(test_item_t)) == 0);

        cr_assert(memcmp(gt2[i], w[i], sizeof(test_item_t))==0);
    }
}
