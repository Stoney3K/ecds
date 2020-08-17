
#include <ecds.h>
#include <common/ecds_service.h>
#include <common/ecds_list.h>

struct _ecds_service_handler_t {
	ecds_object_t obj;
	uint32_t event_id;
	ecds_handler_func user_function;
};

void ecds_service_dispatch_message(ecds_service_t * service, ecds_dispatcher_t * dispatcher, ecds_message_t * msg)
{
	ecds_list_item_t * i;

	if (service == NULL)
		/* Throw a fatal error here */
		return;

	/* Dispatch message to self */
	if(service->dispatch)
		service->dispatch(service, dispatcher, msg);

	/* Dispatch message to any appropriate listeners */
	for (i = ecds_list_first_item(service->handler_list); i; i = ecds_list_next_item(i))
	{
		struct _ecds_service_handler_t * handler = (struct _ecds_service_handler_t *)ecds_list_get_item(service->handler_list, i);
		if (handler->event_id == msg->event_id)
			(*handler->user_function)(msg->user_data_length, msg->user_data);
	}
}