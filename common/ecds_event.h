/*****************************************************************************/
/*	@file ecds_event.h													 	 */
/*	@brief ECDS event descriptor.											 */
/*																			 */
/*	ECDS events are very similar to events you would find in other operating */
/*	system APIs and frameworks. They can be emitted by any process and are	 */
/*	handled by the dispatcher, which will forward them to the appropriate	 */
/*	Service objects which have registered a handler for it.					 */
/*																			 */
/*	This class defines a general-purpose event which has no additional data. */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_EVENT_H
#define _ECDS_EVENT_H

#include <core/ecds_object.h>

#define EVENT(e) ((ecds_event_t *)e)

#define ECDS_EVENT_GENERIC	0x000000001

struct _ecds_event_t
{
	ecds_object_t obj;

	ecds_object_t* sender;	//!<	Pointer to object that sent the event (NULL if anyonymous)
	uint32_t event_type;	//!<	Identifier which sets the type of event

	void* user_data;		//!<	Pointer to user data to pass to event handler
};

#endif /* _ECDS_EVENT_H */