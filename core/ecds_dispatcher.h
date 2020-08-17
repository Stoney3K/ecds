/*****************************************************************************/
/*	@file ecds_dispatcher.h													 */
/*	@brief Interface specification for ECDS dispatcher						 */
/*																			 */
/*****************************************************************************/
#pragma once
#ifndef _ECDS_DISPATCHER_H
#define _ECDS_DISPATCHER_H

#include <ecds.h>

#include <core/ecds_object.h>

#define ECDS_DISPATCHER 0xFFFFFFFA
#define ECDS_DISPATCHER_EVENT 0xFFFFFFAA
typedef struct _ecds_dispatcher_t ecds_dispatcher_t;

ecds_dispatcher_t * ecds_dispatcher_construct(const char * name);

/**
 * @brief Attach a subscription to the dispatcher for a specific event class.
 * @param disp The dispatcher to manipulate. When NULL is passed, the default dispatcher is used.
 * @param event_id The event type ID (virtual bus number) of the event to subscribe to.
 * @param service The service to attach to the dispatcher.
 */
void ecds_dispatcher_subscribe(ecds_dispatcher_t * disp, unsigned int event_id, ecds_service_t * service);

/**
* @brief Attach a subscription to the dispatcher for a specific event class.
* @param disp The dispatcher to manipulate.
* @param event_id The event type ID (virtual bus number) of the event to subscribe to.
* @param service The service to attach to the dispatcher.
*/
void ecds_dispatcher_queue_message(ecds_dispatcher_t * disp, ecds_message_t * msg);

struct _ecds_message_t {
	ecds_object_t obj;

	uint32_t event_id;
	uint16_t user_data_length;
	void * user_data;
};

#endif
