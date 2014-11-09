
# include <stdio.h>

# include "List.h"
# include "Zalloc.h"

struct List {
	int size, count;
	void **elements;
};

List *createList() {
	List *list = NEW(List);

	list->size = 16;
	list->count = 0;
	list->elements = ALLOC(void *, 16);

	return list;
}

void deleteList(List *list, void (* destroy)(void *)) {
	if(list == NULL) return;
	if(list->elements != NULL) {
		listClear(list, destroy);
		DELETE(list->elements);
	}
	DELETE(list);
}

static
void listExpand(List *list) {
	int size = list->size * 2, idx;
	void **data = ALLOC(void *, size);
	for(idx = 0; idx < size; idx++)
		data[idx] = list->elements[idx];
	DELETE(list->elements);
	list->elements = data;
	list->size = size;
}

static
void listShrink(List *list) {
	int size = list->size / 2, idx;
	void **data = ALLOC(void *, size);
	for(idx = 0; idx < size; idx++)
		data[idx] = list->elements[idx];
	free(list->elements);
	list->elements = data;
	list->size = size;
}

List *listAdd(List *list, void *item) {
	if(list == NULL) return NULL;
	if(list->count >= list->size) {
		listExpand(list);
	}

	list->elements[list->count++] = item;
	return list;
}

List *listClear(List *list, void (* destroy)(void *)) {
	int idx;
	if(list == NULL) return NULL;
	if(destroy != NULL) {
		for(idx = 0; idx < list->count; idx++) {
			if(list->elements[idx] != NULL) {
				destroy(list->elements[idx]);
			}
		}
	}

	list->count = 0;
	return list;
}

void *listGet(List *list, int idx) {
	if(list == NULL) return NULL;
	if(idx >= list->count) return NULL;
	return list->elements[idx];
}

void *listSet(List *list, int idx, void *item) {
	void *old;
	if(list == NULL) return NULL;
	if(idx >= list->count) return NULL;

	old = list->elements[idx];
	list->elements[idx] = item;
	return old;
}

int listSize(List *list) {
	if(list == NULL) return -1;
	return list->count;
}

void *listRemove(List *list, int idx) {
	void *old;
	if(list == NULL) return NULL;
	if(idx >= list->count) return NULL;

	old = list->elements[idx];
	if(idx == list->count - 1) {
		list->count--;
	} else {
		for(; idx < list->count; idx++)
			list->elements[idx] = list->elements[idx + 1];
		list->count--;
	}

	if(list->count < list->size / 2)
		listShrink(list);
	return old;
}
