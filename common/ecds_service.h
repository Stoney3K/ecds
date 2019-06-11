/*****************************************************************************/
/*	@file ecds_service.h													 */
/*	@brief ECDS service descriptor.											 */
/*																			 */
/*	ECDS services are objects which provide asynchronous access to user		 */
/*	defined functions by means of listening to events. Service objects are   */
/*	usually created by Process objects if they want to expose functionality	 */
/*	to the user, or created when loading Modules to expose functions		 */
/*	contained in the module.												 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_SERVICE_H
#define _ECDS_SERVICE_H

#include <ecds_object.h>

typedef struct _ecds_dispatcher_t * ecds_dispatcher_t;
typedef struct _ecds_event_t * ecds_event_t;
typedef struct _ecds_service_t * ecds_service_t;

struct _ecds_service_t
{
	ecds_object_t obj;
	
	void (* trap)(ecds_sevice_t * service, 
				  ecds_dispatcher_t * dispatcher,
				  ecds_event_t * event);		//!<	Called when the service receives an event.
}

#endif /* _ECDS_SERVICE_H */