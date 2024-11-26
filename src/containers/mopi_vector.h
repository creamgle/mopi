#ifndef MOPI_DARRAY_H
#define MOPI_DARRAY_H

/*
	CREDIT: i kinda sorta stoll this from Travis Vroman's 'Kohi' darray.h
	mopi_Vector is mopi's implementation of a dynamic array,
	it should not be used by the end user / implementor.
	ONLY TO BE USED FOR MOPI INTERNAL!
*/
	
typedef struct mopi_Vector {
	unsigned long long capacity;
	unsigned long long length;
	unsigned long long stride;
} mopi_Vector;

void* _mopi_VectorCreate(unsigned long long length, unsigned long long stride);
void mopi_VectorDestroy(void* vector);

void* _mopi_VectorResize(void* vector);

void* _mopi_VectorPush(void* vector, const void* value_ptr);
void mopi_VectorPop(void* vector, void* dest);

void* mopi_VectorRemoveAt(void* vector, unsigned long long index, void* dest);

unsigned long long mopi_VectorCapacity(void* vector);
unsigned long long mopi_VectorLength(void* vector);
unsigned long long mopi_VectorStride(void* vector);

#define MOPI_VECTOR_DEFAULT_CAPACITY 1
#define MOPI_VECTOR_RESIZE_FACTOR 2

#define mopi_VectorCreate(type) \
	_mopi_VectorCreate(MOPI_VECTOR_DEFAULT_CAPACITY, sizeof(type))

#define mopi_VectorPush(vector, value)            \
    {											  \
        __typeof__(value) temp = value;           \
        vector = _mopi_VectorPush(vector, &temp); \
    }

#endif