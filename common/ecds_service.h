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

#include <ecds.h>
#include <core/ecds_dispatcher.h>

#define ECDS_IS_SERVICE 0x20000000

void ecds_service_dispatch_message(ecds_service_t * service, ecds_dispatcher_t * dispatcher, ecds_message_t * msg);

struct _ecds_service_t
{
	void (* dispatch)(ecds_service_t * service, 
				  ecds_dispatcher_t * dispatcher,
				  ecds_message_t * msg);		//!<	Called when the service receives a bus message.
};

#endif /* _ECDS_SERVICE_H */