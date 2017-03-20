#ifndef __LIST_H__
#define __List_H__

typedef struct List List;
typedef struct Iterator Iterator;

List *CreateMyList();

int push(List *list, int data);

int getHead(List *list, int *value);

int pop(List *list, int *value);

void deleteList(List *list);

int getSize(List *list);

int isEmpty(List *list);

int printList(List *list);
 
int insertUniqueItem(List* list, int data);

/**--------------- ITERATOR --------------------**/
 
Iterator *CreateIterator(List *list);

int GetNext(Iterator *iter, int *value);

int GetCurrent(Iterator *iter, int *value);

int IsThereNext(Iterator *iter);

int InsertItem(Iterator *iter, int data);

int PrintListFurther(Iterator *iter);

int RestartIter(Iterator *iter);

void DeleteIterator(Iterator *iter);


#endif