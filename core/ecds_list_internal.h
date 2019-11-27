#ifndef _ECDS_LIST_INTERNAL_H
#define _ECDS_LIST_INTERNAL_H

#include <core/ecds_object.h>

#define ECDS_TYPE_LIST				0x0F000000
#define ECDS_TYPE_QUEUE				0x0E000000

struct _ecds_list_item_t
{
	ecds_object_t * data;			//!<	Pointer to actual data

	ecds_list_t * list;				//!<	Pointer to list object owning this item (NULL if orphaned)

	int index;						//!<	The index (sequence number) of the object in the list.
	ecds_list_item_t * previous;	//!<	Pointer to previous item in list (NULL if first item)
	ecds_list_item_t * next;		//!<	Pointer to next item in list (NULL if last item)
};

struct _ecds_list_t
{
	ecds_object_t obj;

	uint32_t count;

	ecds_list_item_t * first;
	ecds_list_item_t * last;

	ecds_object_t ** list_array;
};

#endif