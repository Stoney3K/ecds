/*****************************************************************************/
/*	@file ecds_message.h												 	 */
/*	@brief ECDS message descriptor.											 */
/*																			 */
/*	This class describes the message format that is exchanged on the ECDS	 */
/*  message bus system.														 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_MESSAGE_H
#define _ECDS_MESSAGE_H

#include <ecds.h>

typedef struct _ecds_message_t ecds_message_t;

/**
 * @brief Construct an empty message with no data.
 */
ecds_message_t * ecds_message_new();

/**
 * @brief Construct a message with the given parameters.
 */
ecds_message_t * ecds_message_build(uint16_t bus_number, uint16_t label, void * data);

#endif
