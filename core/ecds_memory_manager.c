/*****************************************************************************/
/*	@file ecds_memory_manager.h												 */
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
#include <stdio.h>

#include <ecds.h>
#include <common/ecds_list.h>

#include <core/ecds_memory_manager.h>
#include <core/ecds_object.h>

#define ECDS_LOG_DOMAIN "ecds-memory-manager"

ecds_memory_manager_t * default_memory_manager = NULL;

ecds_memory_manager_t * ecds_memory_manager_construct()
{
	return (ecds_memory_manager_t *)ecds_object_new("memory-manager", sizeof(ecds_memory_manager_t), ECDS_TYPE_MEMORY_MANAGER);
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
	
	mmgr->memory_entry_list = ecds_list_new();
	ecds_list_initialize(mmgr->memory_entry_list);
}

void _memory_manager_dispose(ecds_object_t * obj)
{
	ecds_memory_manager_t * mmgr = (ecds_memory_manager_t *)obj;
	ecds_list_item_t * list_entry = NULL;
	
	/* Dispose every single object in the manager before destroying self */
	for(list_entry = ecds_list_first_item(mmgr->memory_entry_list); list_entry; list_entry = ecds_list_next_item(list_entry))
	{
		struct _ecds_memory_entry_t * ent = (struct _ecds_memory_entry_t *)ecds_list_get_item(mmgr->memory_entry_list, list_entry);
		
		_dispose_object(ent->entry);
		free(ent);
		
		ecds_list_drop_item(list_entry);
		free(list_entry);
	}
	
	free(obj->name);
}

ecds_memory_manager_t * _memory_manager_create_default()
{
	ecds_memory_manager_t * ret = NULL;
	
	if(default_memory_manager)
		/* We already have a default */
		return default_memory_manager;
	
	ret = ecds_memory_manager_construct();
	if(!ret)
	{	
		/* Something is seriously wrong */
		ecds_log_fatal("Cannot create default memory manager");
	}
	
	ret->process.obj.name = _strdup("memory-manager-default");
	default_memory_manager = ret;

	_memory_manager_init((ecds_object_t *)ret);

	ecds_log_info("Creating default memory manager");
	return ret;
}

void ecds_object_ref(ecds_object_t * obj)
{
	ecds_list_item_t * iter = NULL;
	struct _ecds_memory_entry_t * entry = NULL;
	
	if(!obj)
		/* User is trying to pull our leg. */
		return;
	
	if(obj->type_uid > ECDS_TYPE_UNMANAGED)
		/* Unmanaged object (e.g. memory entry or core object). Ignore. */
		return;
	
	if(!obj->manager)
	{
		/* Object does not have a manager yet, set it to default */
		if (!default_memory_manager)
			default_memory_manager = _memory_manager_create_default();

		obj->manager = default_memory_manager;
	}
	
	for(iter = ecds_list_first_item(obj->manager->memory_entry_list); iter; iter=ecds_list_next_item(iter))
	{
		struct _ecds_memory_entry_t * entry = (struct _ecds_memory_entry_t *)ecds_list_get_item(obj->manager->memory_entry_list, iter);
		if(entry->entry == obj)
		{
			/* Already managed, increase reference count */
			entry->refcnt++;
			ecds_log_debug("Reference count for object %s increased to %d", obj->name, entry->refcnt);
			return;
		}
	}
	
	entry = (ecds_memory_entry_t *)ecds_object_new(NULL, sizeof(ecds_memory_entry_t), ECDS_TYPE_MEMORY_MANAGER_ENTRY);
	entry->entry = obj;
	entry->uid = (uint32_t)rand();
	entry->refcnt++;
	
	if(!obj->name)
	{
		obj->name = malloc(16);
		sprintf_s(obj->name, 16, "object-%8X", entry->uid);
	}
	
	ecds_log_debug("Object %s added to memory manager", obj->name, entry->refcnt);

	ecds_list_add_item(obj->manager->memory_entry_list, (ecds_object_t *)(entry) );
}

void ecds_object_unref(ecds_object_t * obj)
{
	if(!obj)
		return;
	
	if(obj->type_uid > ECDS_TYPE_UNMANAGED)
		return;
	
	if(!obj->manager)
	{
		/* Object does not have a manager yet, set it to default */
		if(!default_memory_manager)
			return;
		obj->manager = default_memory_manager;
	}
	
	for(ecds_list_item_t * iter = ecds_list_first_item(obj->manager->memory_entry_list); iter; iter = ecds_list_next_item(iter))
	{
		ecds_memory_entry_t * entry = (ecds_memory_entry_t *)ecds_list_get_item(obj->manager->memory_entry_list, iter);
		if(entry->entry == obj)
		{
			entry->refcnt--;
			ecds_log_debug("Reference count for object %s decreased to %d", obj->name, entry->refcnt);

			if(entry->refcnt <= 0)
			{
				ecds_log_debug("Disposing object %s (reference count: %d)", obj->name, entry->refcnt);
				/* Refcount exhausted, dispose object */
				if(obj->dispose)
					(* obj->dispose)(obj);
				free(obj->name);
				free(obj);
				
				ecds_list_dispose_item(iter);
				free(entry);
				break;
			}
		}
	}
}

//!< Construct a new object of a specified size and take a reference on it.
ecds_object_t * ecds_object_new(const char * name, size_t size, uint32_t type)
{
	ecds_object_t * ret = NULL;

	if (size < sizeof(ecds_object_t))
		/* Invalid argument */
		return NULL;

	ret = (ecds_object_t *)calloc(1, size);

	if (ret == NULL)
	{
		/* Out of memory */
		ecds_log_error("Out of memory when allocating object %s", name);
		return NULL;
	}

	if (name != 0)
		ret->name = _strdup(name);

	if (type != 0)
		ret->type_uid = type;

	if (type < ECDS_TYPE_UNMANAGED)
		ecds_object_ref(ret);

	return ret;
}

//!< Find the UID of an object in the memory manager's memory.
uint32_t ecds_memory_manager_find_object(ecds_memory_manager_t * mgr, const char * object_name);

//!< Find an object by UID, and take a reference on it.
ecds_object_t * ecds_memory_manager_fetch_object(ecds_memory_manager_t * mgr, uint32_t object_id);