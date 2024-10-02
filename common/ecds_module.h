/*****************************************************************************/
/*	@file ecds_module.h													 	 */
/*	@brief Definition for ECDS module.										 */
/*																			 */
/*****************************************************************************/
#ifndef _ECDS_MODULE_H
#define _ECDS_MODULE_H

#include <core/ecds_object.h>

typedef ecds_module_t * (* ecds_module_constructor_t)();
typedef struct _ecds_module_t ecds_module_t;

struct _ecds_module_t {
	ecds_object_t obj;

	void * library_handle;

};
#endif