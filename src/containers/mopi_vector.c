#include "mopi_vector.h"
#include "../internal.h"
#include "../message_defines.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>


static void _Vector_SetLength(void* vector, unsigned long long length)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	header->length = length;
}

void* _mopi_VectorCreate(unsigned long long length, unsigned long long stride)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	unsigned long long vector_size = length * stride;
	void* new_vector = 0;

	new_vector = malloc(header_size + vector_size);
	memset(new_vector, 0, header_size + vector_size);

	if (length == 0)
	{
		// Some sorta error
	}

	mopi_Vector* vector = new_vector;
	vector->capacity = length;
	vector->length = 0;
	vector->stride = stride;
	
	return (void*)((unsigned char*)new_vector + header_size);
}

void mopi_VectorDestroy(void* vector)
{
	if (!vector)
		return;

	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	unsigned long long total_size = header_size + header->capacity * header->stride;
	free(header, total_size);
}

void* _mopi_VectorResize(void* vector)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	if (header->capacity == 0)
	{
		return 0;
	}

	void* temp = _mopi_VectorCreate((MOPI_VECTOR_RESIZE_FACTOR * header->capacity), header->stride);
	mopi_Vector* new_header = (mopi_Vector*)((unsigned char*)temp - header_size);
	new_header->length = header->length;

	memcpy(temp, vector, header->length * header->stride);
	mopi_VectorDestroy(vector);
}

void* _mopi_VectorPush(void* vector, const void* value_ptr)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	if (header->length >= header->capacity) {
		vector = _mopi_VectorResize(vector);
		header = (mopi_Vector*)((unsigned char*)vector - header_size);
	}

	unsigned long long addr = (unsigned long long)vector;
	addr += (header->length * header->stride);
	memcpy((void*)addr, value_ptr, header->stride);
	_Vector_SetLength(vector, header->length + 1);
	return vector;
}

void mopi_VectorPop(void* vector, void* dest)
{
	unsigned long long length = mopi_VectorLength(vector);
	unsigned long long stride = mopi_VectorStride(vector);
	if (length < 1) {
		return;
	}
	unsigned long long addr = (unsigned long long)vector;
	addr += ((length - 1) * stride);
	if (dest) {
		memcpy(dest, (void*)addr, stride);
	}
	_Vector_SetLength(vector, length - 1);
}

void* mopi_VectorRemoveAt(void* vector, unsigned long long index, void* dest)
{
	unsigned long long length = mopi_VectorLength(vector);
	unsigned long long stride = mopi_VectorStride(vector);

	if (index >= length) {
		mopiInternal_SendMessage(MOPI_MESSAGE_ERROR, MOPI_MSG_VECTOR_INDEX_OUT_OF_BOUNDS);
		return vector;
	}

	unsigned long long addr = (unsigned long long)vector;
	if (dest) {
		memcpy(dest, (void*)(addr + (index * stride)), stride);
	}

	// If not on the last element, snip out the entry and copy the rest inward.
	if (index != length - 1) {
		memcpy(
			(void*)(addr + (index * stride)),
			(void*)(addr + ((index + 1) * stride)),
			stride * (length - (index - 1)));
	}

	_Vector_SetLength(vector, length - 1);
	return vector;
}

unsigned long long mopi_VectorCapacity(void* vector)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	return header->capacity;
}

unsigned long long mopi_VectorLength(void* vector)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	return header->length;
}

unsigned long long mopi_VectorStride(void* vector)
{
	unsigned long long header_size = sizeof(mopi_Vector);
	mopi_Vector* header = (mopi_Vector*)((unsigned char*)vector - header_size);
	return header->stride;
}