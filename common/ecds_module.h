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

#ifndef _ECDS_MODULE_H
#define _ECDS_MODULE_H

#include <ecds_object.h>

typedef struct _ecds_module_manager_t * ecds_module_manager_t;
typedef struct _ecds_module_t * ecds_module_t;

struct _ecds_module_t
{
	ecds_object_t obj;
	
	ecds_module_manager_t * manager;			//!<	Pointer to module manager
	
	unsigned int version;						//!<	Version number (Major * 1000 + minor)
	char * description;							//!<	Optional descriptive text which is displayed when enumerating
	
	void (* load)(ecds_module_t * module);		//!<	Called when the module is first loaded.
	void (* unload)(ecds_module_t * module);	//!<	Called when the module is about to be unloaded.
}

/**
 *	Implemented by shared library. This function is extracted from the libary and called to create the module
 *	contained within.
 */
ecds_module_t * ecds_construct_module();

#endif /* _ECDS_PROCESS_H */