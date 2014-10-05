# ifndef __LIST_H__
# define __LIST_H__

typedef struct List List;

List *createList();

void deleteList(List *list, void (* destroy)());

List *listAdd(List *list, void *item);

List *listClear(List *list, void (* destroy)());

void *listGet(List *list, int idx);

void *listSet(List *list, int idx, void *item);

int listSize(List *list);

void *listRemove(List *list, int idx);

# endif /* List.h */