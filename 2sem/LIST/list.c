#include /<stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "bad_malloc.h"
#include <errno.h>


typedef struct Node {
	struct Node *next;
	int data;
} Node;

struct List {
	Node *head;
	int length;
};

struct Iterator {
	Node *current;
	Node *head;
};

List *CreateMyList() {
	List *list = (List*) bad_malloc(sizeof(List));
	if (list == NULL) {
		perror("Memory allocating error");
		return NULL;
	}
	list->head = NULL;
	list->length = 0;
	return list;
}

int getHead(List *list, int *value) {
	if(list == NULL) {
		perror("Bad list pointer");
		return -1;
	}
	if (list->head == NULL) {
		perror("There's no head");
		return -1;
	}
	*value = list->head->data;
	return 1;
}


int push(List *list, int data) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;

	}
	Node *new_Node = bad_malloc(sizeof(Node));
	if(new_Node) {
		new_Node->data = data;
		new_Node->next = list->head;
		list->head = new_Node;
		list->length++;
	}
	else {
		perror("Error in creationg new Node");
		return 0;
	}
	return 1;
}


int pop(List *list, int *value) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;
	}
	if (list->head == NULL) {
		perror("There're no elements left to extract");
		perror("Just code in code");
		return -1; 
	}
	*value = list->head->data;
	Node *tmp = list->head->next;
	free(list->head);
	list->head = tmp;
	list->length--;

	return 1;
}


void deleteList(List *list) {
	if(list == NULL) {
		perror("Bad List pointer");
		return;
	}
	Node *tmp;
	while (list->head != NULL) {
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}
	free(list);
}

int getSize(List *list) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;
	}
	Iterator *iter = CreateIterator(list);
	if(!iter) {
		return -3;
	}
	int count = 0;
	int trash;
	while (IsThereNext(iter)) {
		count++;

		GetNext(iter, &trash);
	}
	DeleteIterator(iter);
	return count;
}

int isEmpty(List *list) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;

	}
	return (list->head == NULL) ? 1 : 0;
}

int printList(List *list) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;
	}
	Iterator *iter = CreateIterator(list);
	if(!iter) {
		return -3;
	}
	int i = 1;
	for(; iter->current; i++){
		printf("(%d,%d) -> ", i, iter->current->data);
		iter->current = iter->current->next;
	}
	DeleteIterator(iter);
	return 1;
}

int insertUniqueItem(List* list, int data) {
	if(list == NULL) {
		perror("Bad List pointer");
		return -1;
	}
	Iterator *iter = CreateIterator(list);
	if(!iter) {
		return -3;
	}
	int found = 0;
	while(iter->current) {
		if(iter->current->data == data)
			found = 1;
		iter->current = iter->current->next;
	}
	DeleteIterator(iter);
	if(found) 
		return 0;
	if(!push(list, data)){
		return -2;
	}

	return 1;
}


/**-----------ITERATOR------------**/



Iterator *CreateIterator(List *list) { 
	if(list == NULL) {
		perror("Bad List pointer");
		errno = EFAULT;
		return NULL;
	}
	if(list->head == NULL) {
		perror("Empty List was given");
		errno = EFAULT;
		return NULL;
	}
	Iterator *iter = bad_malloc(sizeof(Iterator));
	if(iter) {
		iter->head = list->head;
		iter->current = list->head;
		return iter;
	}
	else {
		perror("Failed to alocate memory for Iter");
		errno = ENOMEM;
		return NULL;
	}
}

int IsThereNext(Iterator *iter) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;

	}
	if(iter->current->next == NULL)
		return 0;
	return 1;
}

int InsertItem(Iterator *iter, int data) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;
	}
	Node *new_Node = bad_malloc(sizeof(Node));
	if(new_Node == NULL) {
		perror("Error in creationg new Node");
		return 0;
	}
	new_Node->data = data;
	new_Node->next = iter->current->next;
	iter->current->next = new_Node;
	
	return 1;
}

int GetCurrent(Iterator *iter, int *value) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;
	}
	*value = iter->current->data;
	return 1;
}


int GetNext(Iterator *iter, int *value) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;
	}
	if(IsThereNext(iter)) {
 		iter->current = iter->current->next;
		*value = iter->current->data;
		return 1;
	}
	perror("No more items left");
	return 0;
}

int PrintListFurther(Iterator *iter) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;
	}
	int i = 1;
	for (; iter->current; iter->current = iter->current->next, i++) {
		printf("(%d,%d) -> ", i, iter->current->data);   
	}
	printf( "NULL\n" );
	return 1;
}


void DeleteIterator(Iterator *iter) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return;
	}
	free(iter);
}

int RestartIter(Iterator *iter) {
	if(iter == NULL) {
		perror("Bad iter pointer");
		return -1;
	}
	iter->current = iter->head;
	return 0;
}