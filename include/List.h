# ifndef __LIST_H__
# define __LIST_H__

# include <vector>

# include "Defines.h"

// Temporary hack
typedef std::vector<void *> List;
// typedef struct List List;

// List *createList();
# define createList new std::vector<void *>

// void deleteList(List *list, void (* destroy)(void *));
static inline
void deleteList(List *list, void (* destroy)(void *)) {
	if(!list) return;
	if(!list->empty() && destroy) {
		for(unsigned int idx = 0; idx < list->size(); idx++) {
			destroy((*list)[idx]);
		}
	}
	DELETE(list);
}

// List *listAdd(List *list, void *item);
# define listAdd(list, item) (!list ? (void)0 : list->push_back(item))

// List *listClear(List *list, void (* destroy)(void *));
# define listClear(list) (!list ? (void)0 : list->clear())

// void *listGet(List *list, int idx);
# define listGet(list, idx) ((void *)(!list ? 0 : ((*list)[idx])))

// void *listSet(List *list, int idx, void *item);
# define listSet(list, idx, item) ((void *)(!list ? 0: ((*list)[idx] = item)))

// int listSize(List *list);
# define listSize(list) (!list ? 0 : list->size())

// void *listRemove(List *list, int idx);
# define listRemove(list, idx) (!list ? (void)0 : list->erase(list->begin() + idx))

# endif /* List.h */