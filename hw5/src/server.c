#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include "csapp.h"

arraylist_t *global_list_t;

typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student;

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

test_item *test_list[10];
sem_t mutex1;

int main(int argc, char *argv[]){
	global_list_t = new_al(sizeof(test_item));
	pthread_t tid_list[20];
	sem_init(&mutex1,0,0);
	for (int i = 0; i < 10; ++i){
		test_item* t1 = (test_item*)malloc(sizeof(test_item));
	    t1->i = i;
	    t1->f = 1.0*i;
	    t1->ld = 2.0*i;
	    t1->c1 = t1->c2 = i;
	    test_list[i] = t1;
	    // printf("assign value: %d\n", test_list[i]->i);
	}


	for (int i = 0; i < 10; ++i){
		pthread_t tid1,tid2;
		Pthread_create(&tid1,NULL,thread_insert,test_list[i]);
		Pthread_create(&tid2,NULL,thread_remove,NULL);
		tid_list[i] = tid1;
		tid_list[i+10] = tid2;
	}
	for (int i = 0; i < 10; ++i){
		Pthread_join(tid_list[i],NULL);
		Pthread_join(tid_list[i+10],NULL);
	}

	for (int i = 0; i < 10; ++i){
		free(test_list[i]);
	}

	/* check result */
	size_t r = global_list_t->length;
	printf("Final result: %d\n", (int)r);
	free(global_list_t->base);
	free(global_list_t);
	exit(0);
}

void *thread_insert(void *data){
	// printf("basic test\n");
	// fflush(stdout);
	size_t index = insert_al(global_list_t, data);
	printf("I: begin\n");
	printf("insert thread return value: %d\n", (int)index);
	printf("insert thread value: %d\n", (int)(((test_item*)data)->i));
	int i = 0;
	sem_getvalue(&mutex1, &i);
	printf("I: The mutex number is %d\n", i);
	fflush(stdout);
	V(&mutex1);
	return NULL;
}

void *thread_remove(){
	P(&mutex1);
	printf("R: begin\n");
	test_item *item = remove_index_al(global_list_t,0);
	// printf("remove thread return value: %d\n", (int)item->i);
	free(item);

	int index = 0;
	printf("SGFT\n");
	fflush(stdout);
	sem_getvalue(&mutex1, &index);
	printf("R: The mutex number is %d\n", index);
	fflush(stdout);
	return NULL;
}

