/*****************************************************************************/
/*	@file ecds_dispatcher.h													 */
/*	@brief Implementation for ECDS dispatcher								 */
/*																			 */
/*****************************************************************************/

#pragma once
#ifndef _ECDS_DISPATCHER_H
#define _ECDS_DISPATCHER_H

#include <ecds.h>
#include <core/ecds_process.h>
#include <common/ecds_queue.h>

#define ECDS_DISPATCHER 0xFFFFFFFA
typedef struct _ecds_dispatcher_t ecds_dispatcher_t;

ecds_dispatcher_t * ecds_dispatcher_construct();

/**
 * @brief Attach a subscription to the dispatcher for a specific event class.
 * @param disp The dispatcher to manipulate.
 * @param event_id The event type ID of the event to subscribe to.
 * @param service The service to attach to the dispatcher.
 */
void ecds_dispatcher_subscribe(ecds_dispatcher_t * disp, unsigned int event_type, ecds_service_t * service);

void ecds_dispatcher_queue_message(ecds_dispatcher_t * disp, ecds_message_t * msg);

#endif
