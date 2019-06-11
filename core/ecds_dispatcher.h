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

#ifndef _ECDS_DISPATCHER_H
#define _ECDS_DISPATCHER_H

#include <ecds_process.h>
#include <ecds_list.h>

#define ECDS_DISPATCHER 0xFFFFFFFD

typedef struct _ecds_dispatcher_t * ecds_dispatcher_t;

/* Storage class for a list of services that subscribe to a certain event class. */
struct _ecds_subscription_t
{
	unsigned int event_type;
	ecds_list_t * subscribers;
}

struct _ecds_dispatcher_t
{
	ecds_process_t process;				//!<	The memory manager itself is a process so it can register in the scheduler.
	
	ecds_queue_t * event_queue;			//!<	Queue of incoming events
	ecds_list_t * subscriptions;		//!<	List of maintained subscriptions
}

extern ecds_memory_manager_t * default_dispatcher = NULL;

ecds_dispatcher_t * ecds_dispatcher_construct();

/**
 * @brief Attach a subscription to the dispatcher for a specific event class.
 * @param disp The dispatcher to manipulate.
 * @param event_id The event type ID of the event to subscribe to.
 * @param service The service to attach to the dispatcher.
 */
void ecds_dispatcher_subscribe(ecds_dispatcher_t * disp, unsigned int event_type, ecds_service_t * service);

void ecds_dispatcher_queue_event(ecds_dispatcher_t, ecds_event_t * event);

//================================== 8< ======================================//
//	FOR ECDS.H																  //
//============================================================================//
/**
 * @brief Register a new event handler
 * @param event_type The integer event type to register. When the type is already registered, 
 *		  the service will be added to the subscriptions.
 * @param service The service to attach to the global dispatcher.
 */
void ecds_register_event_handler(unsigned int event_type, ecds_service_t * service);

/**
 * @brief Post a new event to the global dispatcher.
 * @param event The ecds_event_t * structure that contains the event to post.
 */
void ecds_post_event(ecds_event_t * event);

/**
 * @brief Flush the event queue and dispatch all remaining events.
 */
void ecds_flush_events();

#endif /* _ECDS_MEMORY_MANAGER_H */