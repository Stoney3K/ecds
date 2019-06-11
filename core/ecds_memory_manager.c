/*****************************************************************************/
/*	@file ecsd_memory_manager.h												 */
/*	@brief Implementation for ECDS memory manager							 */
/*																			 */
/*	The memory manager is responsible for managing all objects which		 */
/*	are being constructed and maintained in memory. Each object will have	 */
/*	a reference count and the object is cleaned up if the reference count	 */
/*	is zero. Applications can access all objects through this memory manager */
/*	and creating or passing objects outside of the memory manager will		 */
/*	result in crashes or unexpected behavior.								 */	
/*																			 */
/*****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include <core/ecds_list.h>

ecds_memory_manager_t * default_memory_manager = NULL;

ecds_memory_manager_t * ecds_memory_manager_construct()
{
	/**
	 * The memory manager and scheduler objects are the only objects that will construct
	 * themselves using calloc(). All other objects will allocate using ecds_object_new().
	 */
	return (ecds_memory_manager_t *)calloc(1, sizeof(ecds_memory_manager_t));
}

void _dispose_object(ecds_object_t * obj)
{
	/* Call destructor for object */
	if(obj->dispose)
		(* obj->dispose)(obj);
	
	free(obj->name);
	free(obj);
}

void _memory_manager_init(ecds_object_t * obj)
{
	ecds_memory_manager_t * mmgr = (ecds_memory_manager_t *)obj;
	
	mmgr->memory_entry_list = (ecds_list_t *)calloc(1, sizeof(ecds_list_t));
	ecds_list_initialize(mmgr->memory_entry_list);
}

void _memory_manager_dispose(ecds_object_t * obj)
{
	ecds_memory_manager_t * mmgr = (ecds_memory_manager_t *)obj;
	ecds_list_entry_t * list_entry = NULL;
	
	/* Dispose every single object in the manager before destroying self */
	for(list_entry = mmgr->memory_entry_list->first; list_entry; list_entry = list_entry->next)
	{
		ecds_memory_entry_t * entry = list_entry->data;
		
		_dispose_object(entry->obj);
		free(entry);
		
		ecds_list_drop_item(list_entry);
		free(list_entry);
	}
	
	free(mmgr.object->name);
}

ecds_memory_manager_t * _memory_manager_create_default()
{
	ecds_memory_manager_t * ret = NULL;
	
	if(default_memory_manager)
		/* We already have a default */
		return;
	
	ret = ecds_memory_manager_construct();
	if(!ret)
	{	
		/* Something is seriously wrong */
		ecds_log_fatal("Cannot create default memory manager");
	}
	
	ret->object.name = malloc(1, strlen("memory-manager-default" + 1);
	sprintf(ret->object.name, "memory-manager-default");
}

//!< Construct a new object of a specified size and take a reference on it.
ecds_object_t * ecds_memory_manager_create_object(ecds_memory_manager_t * mgr, size_t size, const char * object_name)
{
	ecds_object_t * ret = NULL;
	
	if(!mgr || size < sizeof(ecds_object_t))
		/* Invalid argument */
		return NULL;
		
	ret = (ecds_object_t *)calloc(1, size);
	if(!ret)
		/* Probably out of memory */
		return NULL;
		
	if(object_name)
	{
		ret->name = malloc(strlen(object_name) + 1);
		strcpy(ret->name, object_name);
	}
	
	ret->manager = mgr;
	
	ecds_object_ref(ret);
	
	return ret;
}

void ecds_object_ref(ecds_object_t * obj)
{
	ecds_list_item_t * iter = NULL;
	ecds_memory_entry_t * entry = NULL;
	
	if(!obj)
		/* User is trying to pull our leg. */
		return;
	
	if(obj->type_uid > ECDS_TYPE_UNMANAGED)
		/* Unmanaged object (e.g. memory entry or core object). Ignore. */
		return;
	
	if(!obj->manager)
	{
		/* Object does not have a manager yet, set it to default */
		if(!default_memory_manager)
			return;
		obj->manager = default_memory_manager;
	}
	
	for(iter = ecds_list_first_item(obj->manager->memory_entry_list); iter; iter=ecds_list_next_item(iter))
	{
		ecds_memory_entry_t * entry = (ecds_memory_entry_t *)iter->data;
		if(entry->obj == obj)
		{
			/* Already managed, increase reference count */
			entry->refcnt++;
			return;
		}
	}
	
	entry = (ecds_memory_entry_t *)calloc(1, sizeof(ecds_memory_entry_t *);
	entry->uid = (uint32_t)rand();
	entry->refcnt++;
	
	if(!obj->name)
	{
		obj->name = malloc(16);
		sprintf(obj->name, "object-%8X", entry->uid);
	}
	
	ecds_list_add_item(obj->manager->memory_entry_list, OBJECT(entry));
}

void ecds_object_unref(ecds_object_t * obj)
{
	if(!obj)
		/* User is trying to pull our leg. */
		return;
	
	if(obj->type_uid > ECDS_TYPE_UNMANAGED)
		/* Unmanaged object (e.g. memory entry or core object). Ignore. */
		return;
	
	if(!obj->manager)
	{
		/* Object does not have a manager yet, set it to default */
		if(!default_memory_manager)
			return;
		obj->manager = default_memory_manager;
	}
	
	for(iter = ecds_list_first_item(obj->manager->memory_entry_list); iter; iter=ecds_list_next_item(iter))
	{
		ecds_memory_entry_t * entry = (ecds_memory_entry_t *)iter->data;
		if(entry->obj == obj)
		{
			entry->refcnt--;
			if(entry->refcnt <= 0)
			{
				/* Refcount exhausted, dispose object */
				if(obj->dispose)
					(* obj->dispose)(obj);
				free(obj->name);
				free(obj);
				
				ecds_list_dispose_item(iter);
				free(entry);
			}
		}
	}
}

//!< Find the UID of an object in the memory manager's memory.
uint32_t ecds_memory_manager_find_object(ecds_memory_manager_t * mgr, const char * object_name);

//!< Find an object by UID, and take a reference on it.
ecds_object_t * ecds_memory_manager_fetch_object(ecds_memory_manager_t * mgr, uint32_t object_id);

#endif /* _ECDS_MEMORY_MANAGER_H */