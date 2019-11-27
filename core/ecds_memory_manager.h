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

#ifndef _ECDS_MEMORY_MANAGER_H
#define _ECDS_MEMORY_MANAGER_H

#include <ecds.h>

#include <core/ecds_process.h>
#include <core/ecds_object.h>

typedef struct _ecds_memory_manager_t ecds_memory_manager_t;

//!<	Class UID of MAXINT is reserved for the memory manager.
#define ECDS_TYPE_MEMORY_MANAGER 			0xFFFFFFFF
#define ECDS_TYPE_MEMORY_MANAGER_ENTRY		0xFFFFFFFE
#define ECDS_TYPE_MEMORY_MANAGER_LIST		0xFFFFFFFD
#define ECDS_TYPE_UNMANAGED					0xFFFFFF00

struct _ecds_memory_entry_t
{
	ecds_object_t obj;
	ecds_object_t * entry;
	uint32_t uid;
	int refcnt;
};

struct _ecds_memory_manager_t
{
	ecds_process_t process;				//!<	The memory manager itself is a process so it can register in the scheduler.
	
	ecds_list_t * memory_entry_list;	//!<	List of memory entries
};

typedef struct _ecds_memory_entry_t ecds_memory_entry_t;

ecds_memory_manager_t * ecds_memory_manager_construct();

//!< Construct an empty object of a specified size and take a reference on it.
ecds_object_t * ecds_memory_manager_create_object(ecds_memory_manager_t * mgr, size_t size, const char * object_name);

//!< Construct an empty object based on class definition.
ecds_object_t * ecds_memory_manager_construct_object(ecds_memory_manager_t * mgr, const char * class, const char * object_name);

//!< Find the UID of an object in the memory manager's memory.
uint32_t ecds_memory_manager_find_object(ecds_memory_manager_t * mgr, const char * object_name);

//!< Find an object by UID, and take a reference on it.
ecds_object_t * ecds_memory_manager_fetch_object(ecds_memory_manager_t * mgr, uint32_t object_id);


//=================================== 8< ====================================//
//				GENERAL MEMORY MANAGEMENT FUNCTIONS FOR ECDS				 //
//===========================================================================//

/**
 * @brief Use the default memory manager to create and reference a new object without a constructor.
 *		  For constructing registered objects with a specific class, use ecds_object_construct().
 *
 *		  This function is mostly used as a utility function in internal routines since it does not take
 *		  direct care of object constructors and destructors. These are still the responsibility of the user
 *		  creating the object.
 */
ecds_object_t * ecds_object_new(const char * name, size_t size, uint32_t type);

//!< Find an object with a specific name in the memory manager's memory.
ecds_object_t * ecds_object_find(const char * object_name);

//!< Take an additional reference on an object.
void ecds_object_ref(ecds_object_t * obj);

//!< Decrease reference on an object and dispose it if necessary.
void ecds_object_unref(ecds_object_t * obj);

//============= !< This goes in ecds_property_handler.h >!===================//
void ecds_set_property(ecds_object_t * obj, const char * property_name, void * property_value);
void * ecds_get_property(ecds_object_t * obj, const char * property_name);

#endif /* _ECDS_MEMORY_MANAGER_H */