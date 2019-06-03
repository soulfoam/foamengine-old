#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

/*===================================*/
/* --------------------------        */
/* | count | cap | user_data <---- | */
/* --------------------------        */
/*===================================*/

/*==============*/
/* Private impl */
/*==============*/

#define VECTOR_META_DATA_SIZE__		  2

#define _vector_start_ptr__(v)		  &((uint32_t*)(v))[-2]
#define _vector_set_count__(v, count) ((uint32_t*)(v))[-2] = (count)
#define _vector_set_cap__(v, cap)     ((uint32_t*)(v))[-1] = (cap)

/*===========================*/
/* Public to be used by user */
/*===========================*/

#define VECTOR_EMPTY				NULL
#define vector(type)				type*

#define vector_count(v)				((v) ? (((uint32_t*)(v))[-2]) : 0)
#define vector_cap(v)   			((v) ? (((uint32_t*)(v))[-1]) : 0)
#define vector_size(v)				(vector_count(v) * sizeof(*v))
#define vector_clear(v) 			_vector_set_count__((v), 0)
#define vector_reserve(v, size)		vector_set_capacity(v, vector_cap((v)) + size)
#define vector_pop(v)				_vector_set_count__((v), vector_count((v)) - 1)
#define vector_shrink_to_fit(v)		vector_set_capacity(v, vector_count(v))
#define vector_begin(v)				(v)
#define vector_end(v)				((v) ? &((v)[vector_count(v)]) : NULL)
#define vector_end_index(v)			vector_count(v)-1

#define vector_init_val(v, size, val) \
do \
{ \
	vector_set_capacity(v, size); \
	for (uint32_t _i__ = 0; _i__ < (size); _i__++) \
	{ \
		vector_push(v, val); \
	} \
} while(0) 

#define vector_free(v) \
do \
{ \
	if (v) \
	{ \
		free(_vector_start_ptr__(v)); \
		v = VECTOR_EMPTY; \
	} \
 \
} while(0) 

#define vector_free_all(v, func) \
do \
{ \
	uint32_t _count__ = vector_count(v);  \
	for (uint32_t _i__ = 0; _i__ < _count__; _i__++)  \
	{ \
		(func)((v)[_i__]); \
	} \
	vector_free((v)); \
 \
} while(0) 

#define vector_free_all_self(v, func) \
do \
{ \
	uint32_t _count__ = vector_count(v);  \
	for (uint32_t _i__ = 0; _i__ < _count__; _i__++)  \
	{ \
		(func)(&(v)[_i__]); \
	} \
	vector_free((v)); \
 \
} while(0) 


#define vector_set_capacity(v, size) \
do \
{ \
	if (!(v)) \
	{ \
		uint32_t *_vec_data__ = malloc( \
				(sizeof(*(v)) * (size)) +  \
				(sizeof(uint32_t) * VECTOR_META_DATA_SIZE__)); \
		assert(_vec_data__); \
		(v) = (void*)(&_vec_data__[2]); \
		_vector_set_count__(v, 0); \
		_vector_set_cap__(v, size); \
	} \
	else \
	{ \
		uint32_t *_vec_start__ = _vector_start_ptr__((v));	 \
		uint32_t *_vec_new__ = realloc( \
				_vec_start__,  \
				(sizeof(*(v)) * (size)) +  \
				(sizeof(uint32_t) * VECTOR_META_DATA_SIZE__)); \
		assert(_vec_new__); \
		(v) = (void*)(&_vec_new__[2]); \
		_vector_set_cap__(v, size); \
	} \
} while(0)

#define vector_copy(dst, src) \
do \
{ \
	uint32_t _count__ = vector_count(src); \
	for (uint32_t _i__ = 0; _i__ < _count__; _i__++) \
	{ \
		vector_push(dst, src[_i__]); \
	} \
} while(0) 

#define vector_contains(v, boolptr, data) \
do \
{ \
	*boolptr = false; \
	uint32_t _count__ = vector_count(v); \
	for (uint32_t _i__ = 0; _i__ < _count__; _i__++) \
	{ \
		if (data == v[_i__]) *boolptr = true; \
	} \
} while(0) 

#define vector_push(v, data) \
do \
{ \
	uint32_t _cap__ = vector_cap(v); \
	if (_cap__ <= vector_count(v)) \
	{ \
		if (_cap__ == 0) _cap__ = 1; \
		vector_set_capacity((v), _cap__ * 2); \
	} \
	uint32_t _count__ = vector_count(v); \
	v[_count__] = (data); \
	_vector_set_count__((v), _count__ + 1); \
} while(0) 

#define vector_remove(v, index) \
do \
{ \
	if (v) \
	{ \
		uint32_t _count__ = vector_count(v); \
		if ((index) < _count__) \
		{ \
			for (uint32_t _i___ = index; _i___ < (_count__-1); ++_i___) \
			{ \
				(v)[_i___] = (v)[_i___+1]; \
			} \
			_vector_set_count__((v), _count__-1); \
		} \
	} \
} while(0) 

#define vector_remove_eq(v, val) \
do \
{ \
	if (v) \
	{	 \
		uint32_t _count__ = vector_count(v); \
		for (uint32_t _i__ = 0; _i__ < _count__; _i__++) \
		{ \
			if ((v)[_i__] == val) vector_remove((v), _i__); \
		} \
	} \
} while(0) 

#define vector_remove_eq_str(v, val) \
do \
{ \
	if (v) \
	{	 \
		uint32_t _count__ = vector_count(v); \
		for (uint32_t _i__ = 0; _i__ < _count__; _i__++) \
		{ \
			if (!strcmp((v)[_i__], val)) vector_remove((v), _i__); \
		} \
	} \
} while(0) 


#define vector_remove_ptr(v, ptr) \
do \
{ \
	if (v) \
	{	 \
		uint32_t _count__ = vector_count(v); \
		for (uint32_t _i__ = 0; _i__ < _count__; _i__++) \
		{ \
			if (&(v)[_i__] == ptr) vector_remove((v), _i__); \
		} \
	} \
} while(0)

