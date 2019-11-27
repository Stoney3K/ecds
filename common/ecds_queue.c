#include <common/ecds_queue.h>

#include <core/ecds_object.h>
#include <core/ecds_list_internal.h>

struct _ecds_queue_t
{
	ecds_list_t list;
};

ecds_queue_t * ecds_queue_new()
{
	return (ecds_queue_t *)ecds_object_new("ecds-queue", sizeof(ecds_queue_t), ECDS_TYPE_QUEUE);
}

void ecds_queue_dispose(ecds_queue_t * queue)
{
	ecds_list_dispose((ecds_list_t *)queue);
}

ecds_list_item_t * ecds_queue_enqueue_item(ecds_queue_t * queue, ecds_list_item_t * item)
{
	return ecds_list_take_item((ecds_list_t *)queue, item);
}

ecds_list_item_t * ecds_queue_dequeue_item(ecds_queue_t * queue)
{
	ecds_list_item_t * first = NULL;
	ecds_list_item_t * last = NULL;

	if (!queue)
		return NULL;

	last = ecds_list_last_item((ecds_list_t *)queue);
	first = ecds_list_first_item((ecds_list_t *)queue);

	if (first)
	{
		ecds_list_drop_item(first);
		_ecds_list_reorder((ecds_list_t *)queue);
	}

	return first;
}

ecds_list_item_t * ecds_queue_peek(ecds_queue_t * queue)
{
	ecds_list_item_t * first = NULL;

	if (!queue)
		return NULL;

	return ecds_list_first_item((ecds_list_t *)queue);
}

void ecds_queue_flush(ecds_queue_t * queue, void(*flush_func)(ecds_object_t * obj))
{
	ecds_list_item_t * item = NULL;

	if (!queue)
		return;
	if (!flush_func)
		return;

	while (item = ecds_list_first_item((ecds_list_t *)queue))
	{
		(* flush_func)(ecds_list_get_item((ecds_list_t *)queue, item));
		ecds_list_dispose_item(item);
	}
}
