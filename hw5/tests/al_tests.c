#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"

/******************************************
 *                  ITEMS                 *
 ******************************************/
arraylist_t *global_list;

typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

// typedef struct{
//     int i;
//     float f;
//     long double ld;
//     char c1:4;
//     char c2:4;
//     short s;
//     void *some_data;
// }test_item_t;

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
