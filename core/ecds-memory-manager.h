/*****************************************************************************/
/*	ecds-memory-manager.h: Contains the main functions 						 *
/*						   for the memory manager 							 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_MEMORY_MANAGER_H
#define _ECDS_MEMORY_MANAGER_H
#include <common/ecds-object.h>

int32_t ecds_memory_manager_setup();

int32_t ecds_register_type( const char * type_name,
							(void *)() constructor,
							(void *)() destructor );

ecds_object_t * ecds_
									 
#endif /* _ECDS_MEMORY_MANAGER_H */