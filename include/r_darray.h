#ifndef RD_ARRAY
#define RD_ARRAY

#include <stdlib.h>
#include <string.h>

#define RDARRAY_RESIZE_FACTOR 	2

typedef struct rdarray {
	int capacity;
	int length;
	int element_size;
	void* elements;
} rdarray;

rdarray 	rdarray_create(int size_of_element);
void 		rdarray_push(rdarray* array, void* element);
void 		rdarray_pop(rdarray* array, void* dest);
void 		rdarray_push_at(rdarray* array, void* element, int index);
void 		rdarray_pop_at(rdarray* array, void* dest, int index);
void* 		rdarray_at(rdarray* array, int index);
rdarray* 	rdarray_resize(rdarray* array);
void 		rdarray_delete(rdarray* array);

#endif // RD_ARRAY