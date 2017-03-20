#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <errno.h>

int main() {
    /***list TEST***/
    for(int j = 0; j < 100; j++) {
        printf("\n/***list TEST***/\n");
        int value = 0;
    
        List *list = CreateMyList();
        for (int i = 0; i <= 1000; i++)
            push(list, i);
        getHead(list, &value);
        printf("getHead %d\n", value);

        for (int i = 0; i <= 5; i++) {
            pop(list, &value);
            printf("pop %d\n", value);
        }
        getHead(list, &value);
        printf("getHead %d\n", value);
        printf("isEmpty? %d\n", isEmpty(list));

        printf("SIZE %d\n", getSize(list));
        for (int i = 0; i < 5; i++) {
            pop(list, &value);
            printf("pop %d\n", value);
        }
        printf("isEmpty? %d\n\n", isEmpty(list));

        printf("deletelist\n");


        deleteList(list);

        /*** ITERATOR TEST ***/

        List *list1 = CreateMyList();
        for (int i = 0; i <= 2; i++)
            push(list1, i);
        insertUniqueItem(list1, 1);
        insertUniqueItem(list1, 1554);
            
        printf("\n/*** ITERATOR TEST ***/\n");
        printf("\nLets find the max\n");

        Iterator *iter = CreateIterator(list1);
        printf("1\n");
        GetNext(iter, &value);
        printf("2\n");
        int max = value;
        /*while (IsThereNext(iter)) {
            int tmp = GetNext(iter, &value);
            if (tmp > max)
                max = tmp;
        }*/

        Iterator *_iter = CreateIterator(list1);
        PrintListFurther(_iter);
        DeleteIterator(_iter);


        printf("MAX = %d\n", max);
        // printf("GETNEXT: %d\n", GetNext(iter, &value));
        printf("RestartIter\n");
        RestartIter(iter);
        GetNext(iter, &value);
        printf("And after restart GETNEXT1: %d\n", value);
        printf("Insert 8 right here\n");
        InsertItem(iter, 8);    
        GetNext(iter, &value);
        printf("And after restart GETNEXT2: %d\n", value);
        GetNext(iter, &value);
        printf("And after restart GETNEXT3: %d\n", value);
        GetNext(iter, &value);
        printf("And after restart GETNEXT4: %d\n", value);
        GetCurrent(iter, &value);
        printf("SO, GetCurrent: %d\n", value);

        printList(list1);

        deleteList(list1);
       // printf("And after delete list GETNEXT: %d\n", GetNext(iter, &value));

        // RestartIter(iter);
        // printf("And after restart GETNEXT1: %d\n", GetNext(iter, &value));
      


        printf("So.....\n\n");
        printf("DELETE ITER!\n");
        DeleteIterator(iter);

        //To execute the exception with the empty list
        List *list2 = CreateMyList();
        CreateIterator(list2);

        //And what is going with getHead?
        getHead(list2, &value);
        //And with an empty list and pop?
        pop(list2, &value);
        deleteList(list2);

        /**FINAL TEST**/
        push(NULL, 6);
        getHead(NULL, &value);
        pop(NULL, &value);
        getSize(NULL);
        printList(NULL);
        isEmpty(NULL);
        deleteList(NULL);
        insertUniqueItem(NULL, 2);


    /**--------------- ITERATOR --------------------**/
        CreateIterator(NULL);
        GetNext(NULL, &value);
        GetCurrent(NULL, &value);
        IsThereNext(NULL);
        InsertItem(NULL, value);
        PrintListFurther(NULL);
        RestartIter(NULL);
        DeleteIterator(NULL);
    }

    return 0;

}