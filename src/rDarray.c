#include "rDarray.h"

rdarray rdarray_create(int size_of_element){
	rdarray array = {0};

	array.capacity = 1;
	array.element_size = size_of_element;
	array.elements = calloc(array.capacity, size_of_element);

	return array;
}

void rdarray_push(rdarray* array, void* element){
	if(array->capacity == array->length) array = rdarray_resize(array);
	memcpy((char*)array->elements + array->length * array->element_size, element, array->element_size);
	array->length++;
}

void rdarray_pop(rdarray* array, void* dest){
	if(array->length > 0){
		char* result = (char*)array->elements + (array->length - 1) * array->element_size;
		if(dest){
			memcpy(dest, result, array->element_size);
		}
		array->length--;
	}
}

void rdarray_push_at(rdarray* array, void* element, int index){
	if(array->length == array->capacity){
		array = rdarray_resize(array);
	}

	if(index == array->length) {
		rdarray_push(array, element);
	}else if (index < array->length){
		memmove((char*)array->elements + ((index + 1) * array->element_size),
				(char*)array->elements + index * array->element_size, 
				(array->length - index) * array->element_size);
		memcpy((char*)array->elements + index * array->element_size, element, array->element_size);
		array->length++;
	}
}

void rdarray_pop_at(rdarray* array, void* dest, int index){
	if(index == array->length - 1){
		rdarray_pop(array, dest);
	}else if(index < array->length - 1){
		if(dest){
			memcpy(dest, (char*)array->elements + (index * array->element_size), array->element_size);
		}
		memmove((char*)array->elements + index * array->element_size,
				(char*)array->elements + ((index + 1) * array->element_size),
				(array->length - index) * array->element_size);
		array->length--;
	}
}

void* rdarray_at(rdarray* array, int index){
	return (index < array->length) ? (char*)array->elements + index * array->element_size : NULL;
}

rdarray* rdarray_resize(rdarray* array){
	array->capacity *= RDARRAY_RESIZE_FACTOR;
	void* aux = realloc(array->elements, array->capacity * array->element_size);
	if (aux){
		array->elements = aux;
		return array;
	}
	return NULL;
}

void rdarray_delete(rdarray* array){
	if(array){
		array->capacity = 0;
		array->element_size = 0;
		array->length = 0;
		free(array->elements);
		array->elements = NULL;
	}
}