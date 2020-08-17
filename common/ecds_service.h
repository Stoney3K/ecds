/*****************************************************************************/
/*	@file ecds_service.h													 */
/*	@brief ECDS service descriptor.											 */
/*																			 */
/*	ECDS services are objects which provide asynchronous access to user		 */
/*	defined functions by means of listening to events. Service objects are   */
/*	usually created by Process objects if they want to expose functionality	 */
/*	to the user, or created when loading Modules to expose functions		 */
/*	contained in the module.												 */
/*  Services by themselves are only attachment points to the dispatcher,     */
/*  functionality is provided by individual handlers which are registered    */
/*  within the specific service.                                             */
/*																			 */
/*  For implementing new functionality into ECDS, the workflow would be as   */
/*  follows:																 */
/*																			 */
/*  - Create the appropriate Process or Module objects which contain your    */
/*    basic interaction and functionality.                                   */
/*  - Create handler functions for each of the message types you want to     */
/*    interact with.                                                         */
/*  - These handler functions can then be aggregated into a single Service   */
/*    or arranged into multiple individual Services as you see fit.          */
/*  - Once the Service objects are created you can attach them to the        */
/*    dispatcher and they will begin receiving messages.                     */
/*                                                                           */
/*  As each of the services will receive their own individual copy of a      */
/*  message there is no risk of corrupting shared memory between threads.    */
/*                                                                           */
/*****************************************************************************/
#ifndef _ECDS_SERVICE_H
#define _ECDS_SERVICE_H

#include <ecds.h>

#include <core/ecds_object.h>
#include <core/ecds_dispatcher.h>

#define ECDS_IS_SERVICE 0x20000000

/**
 * @brief Universal service handler prototype.
 * @param user_data_length The number of bytes of data that was copied from
 *						   the dispatched message.
 * @param user_data A pointer to a copy of the user-specified data structure
 *					that was passed on to the dispatcher.
 */
typedef void(* ecds_handler_func)( uint32_t user_data_length, 
								   void * user_data );

/**
 * @brief Dispatches a message to one of the handlers registered within this service.
 *		  The message is copied into the service's memory before being passed on
 *		  to the registered handlers. If there are no handlers registered, the dispatch
 *		  is ignored.
 * @param service The service to act on.
 * @param dispatcher The dispatcher which was responsible for sending the message.
 * @msg The actual message that is being dispatched.
 */
void ecds_service_dispatch_message(ecds_service_t * service, ecds_dispatcher_t * dispatcher, ecds_message_t * msg);

/**
 * @brief Registers a new handler into a service.
 * @param service The service to modify.
 * @param event_id One of the ECDS_EVENT constants for event bus types, or a user-defined bus number.
 * @user_function A pointer to the handler function which performs an action on this event type.
 */
void ecds_service_add_handler(ecds_service_t * service, uint32_t event_id, ecds_handler_func user_function);

struct _ecds_service_t
{
	ecds_object_t obj;

	ecds_list_t * handler_list;

	void (* dispatch)(ecds_service_t * service, 
				  ecds_dispatcher_t * dispatcher,
				  ecds_message_t * msg);		//!<	Called when the service receives a bus message.
};

#endif /* _ECDS_SERVICE_H */