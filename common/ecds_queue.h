/*****************************************************************************/
/*	@file ecds_queue.h													 	 */
/*	@brief ECDS queue descriptor.											 */
/*																			 */
/*	A queue is a special case of list, which adds objects on one end of the  */
/*	queue while removing them from the other. It is generally used in 		 */
/*	sequential processing of events and communications messages.			 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_QUEUE_H
#define _ECDS_QUEUE_H

#include <ecds.h>
#include <common/ecds_list.h>

typedef struct _ecds_queue_t ecds_queue_t;

ecds_queue_t * ecds_queue_new();

void ecds_queue_dispose(ecds_queue_t * queue);

ecds_list_item_t * ecds_queue_enqueue_item(ecds_queue_t * queue, ecds_list_item_t * item);
ecds_list_item_t * ecds_queue_dequeue_item(ecds_queue_t * queue);
ecds_list_item_t * ecds_queue_peek(ecds_queue_t * queue);
void ecds_queue_flush(ecds_queue_t * queue, void (* flush_func)(ecds_object_t * obj));

#endif /* _ECDS_QUEUE_H */