/*****************************************************************************/
/*	@file ecds_module.h													 	 */
/*	@brief ECDS module descriptor.											 */
/*																			 */
/*	An ECDS module is an object that is compiled into a shared library		 */
/*	and will provide certain functions to the ECDS subsystem. Usually, this  */
/*	functionality should be encapsulated into several objects (typically 	 */
/*	services) which can provide the functionality through emitting or 		 */
/*	responding to events.													 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_MODULE_MANAGER_H
#define _ECDS_MODULE_MANAGER_H

#include <ecds.h>
#include <core/ecds_object.h>

typedef struct _ecds_module_manager_t * ecds_module_manager_t;

typedef enum
{
	EMS_INVALID = -1,
	EMS_INIT = 0,		//!<	Module entry is created but nothing attached yet
	EMS_LOADED = 1,		//!<	Module is loaded (library file detected)
	EMS_REGISTERED = 2,	//!<	Module is registered in memory
	EMS_UNLOAD = 3,		//!<	Module is flagged for unload
	EMS_ERROR = 255		//!<	Module loading failed
} ecds_module_status_t;

/**
 * Descriptor for a single entry in the module manager's list.
 */
struct _ecds_module_entry_t
{
	ecds_module_t * module;
	char * path;
	int status;
};

struct _ecds_module_manager_t
{
	ecds_object_t obj;
	
	ecds_list_t * module_list;	//!<	List of registered modules
};

/**
 * @brief Creates the default instance for the module manager.
 */
void ecds_module_manager_create_default();

/**
 * @brief Returns the default module manager or creates one if it does not exist yet.
 */
ecds_module_manager_t * ecds_module_manager_get_default();

/**
 *	@brief Register a new module in a module manager.
 */
void ecds_module_manager_register_module(ecds_module_manager_t * manager, ecds_module_t * module);

/**
*	@brief Unregister an existing module from the module manager.
*/
void ecds_module_manager_unregister_module(ecds_module_manager_t * manager, ecds_module_t * module);


#endif /* _ECDS_PROCESS_H */