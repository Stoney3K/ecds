/*****************************************************************************/
/*	@file ecds_list.c														 */
/*	@brief Defines a double linked list.					 				 */
/*																			 */
/*	This is an implementation of a dynamically-sized, double linked list	 */
/*	which grows or shrinks depending on the items added or removed.			 */
/*	It is an independent implementation, the items themselves do not		 */
/*	register in the memory manager but are managed by the list itself.		 */
/*																			 */
/*****************************************************************************/

#include <common/ecds_list.h>
#include <core/ecds_list_internal.h>

#include <memory.h>

void _ecds_list_reorder(ecds_list_t * list)
{
	/* Stub. This function will iterate through all items in the list and update their sequence number. */
	return;
}

ecds_list_t * ecds_list_new()
{
	return (ecds_list_t *)ecds_object_new("ecds-list", sizeof(ecds_list_t), ECDS_TYPE_LIST);
}

void ecds_list_initialize(ecds_list_t * list)
{
	if(!list)
		return;
	
	memset(list, 0, sizeof(ecds_list_t));
}

void ecds_list_dispose(ecds_list_t * list)
{
	ecds_list_item_t * iter;
	
	if(!list)
		/* Invalid argument */
		return;
	
	for(iter = list->last; iter; iter=iter->previous)
	{
		ecds_list_dispose_item(iter);
	}
}

ecds_list_item_t * ecds_list_first_item(ecds_list_t * list)
{
	if (list)
		return list->first;
	else
		return 0;
}

ecds_list_item_t * ecds_list_last_item(ecds_list_t * list)
{
	if (list)
		return list->last;
	else
		return 0;
}

ecds_list_item_t * ecds_list_next_item(ecds_list_item_t * item)
{
	if (item)
		return item->next;
	else
		return 0;
}

ecds_list_item_t * ecds_list_previous_item(ecds_list_item_t * item)
{
	if (item)
		return item->previous;
	else
		return 0;
}

ecds_list_item_t * ecds_list_add_item(ecds_list_t * list, ecds_object_t * obj)
{
	ecds_list_item_t * new_item;
	
	if(!list || !obj)
		return NULL;
	
	new_item = (ecds_list_item_t *)calloc(1, sizeof(ecds_list_item_t));
	if(!new_item)
		/* Serious problem -- out of memory */
		return NULL;

	ecds_object_ref(obj);
	
	new_item->data = obj;
	return ecds_list_take_item(list, new_item);
}

ecds_list_item_t * ecds_list_take_item(ecds_list_t * list, ecds_list_item_t * item)
{
	if(!list || !item)
		return NULL;
	if(item->list)
		/* Item is already attached, do not relocate it */
		return NULL;
	
	item->list = list;
	item->previous = list->last;
	
	if(list->last)
		list->last->next = item;
	
	if (!list->first)
		list->first = item;
	
	list->last = item;
	
	_ecds_list_reorder(list);
	
	return item;
}

ecds_object_t * ecds_list_get_item(ecds_list_t * list, ecds_list_item_t * item)
{
	if (!list || !item)
		return NULL;
	if (item->list != list)
		/* Item does not belong to our list */
		return NULL;

	return item->data;
}

ecds_list_item_t * ecds_list_drop_item(ecds_list_item_t * item)
{
	ecds_list_t * list;
	if(!item)
		return NULL;
	
	if(!item->list)
		/* Nothing to be done */
		return item;
	
	list = item->list;
		
	if(item->previous)
		item->previous->next = item->next;
	if(item->next)
		item->next->previous = item->previous;
	
	if(list->last == item)
		list->last = item->previous;
	if(list->first == item)
		list->first = item->next;
	
	item->list = NULL;
	item->index = -1;
	
	_ecds_list_reorder(list);
	
	return item;
}

ecds_object_t * ecds_list_dispose_item(ecds_list_item_t * item)
{
	ecds_object_t * obj;
	
	if(!item)
		return NULL;
	
	ecds_list_drop_item(item);
	obj = item->data;
	
	ecds_object_unref(obj);
	
	free(item);
	
	return obj;
}