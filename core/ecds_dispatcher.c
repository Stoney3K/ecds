#include <stdio.h>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#define ECDS_LOG_DOMAIN "ecds-dispatcher"

#include <common/ecds_queue.h>
#include <common/ecds_service.h>
#include <common/ecds_log.h>

#include <core/ecds_process.h>
#include <core/ecds_dispatcher.h>

static ecds_dispatcher_t * default_dispatcher = NULL;

void ecds_dispatcher_dispatch_message(ecds_dispatcher_t * disp, ecds_message_t * msg);

typedef struct _ecds_dispatcher_event_t ecds_dispatcher_event_t;
struct _ecds_dispatcher_event_t {
	ecds_object_t obj;
	uint32_t event_id;
	ecds_list_t * service_list;
};

struct _ecds_dispatcher_t {
	ecds_process_t proc;
	ecds_queue_t * message_queue;
	ecds_list_t * event_list;

	bool running;

	pthread_t dispatcher_thread[1];
	pthread_mutex_t dispatcher_mutex[1];
	pthread_cond_t dispatcher_cond[1];
};

static void * _dispatcher_thread(void * arg)
{
	ecds_dispatcher_t * disp = (ecds_dispatcher_t *)arg;
	bool running = true;

	while (running)
	{
		pthread_mutex_lock(disp->dispatcher_mutex);

		/* Wait for messages to become available */
		while (ecds_queue_peek(disp->message_queue) == NULL) {
			pthread_cond_wait(disp->dispatcher_cond, 
							  disp->dispatcher_mutex);
		}

		for (ecds_list_item_t * iter = ecds_queue_dequeue_item(disp->message_queue);
			iter; 
			iter = ecds_queue_dequeue_item(disp->message_queue))
		{
			/* Extract message and dequeue the next */
			ecds_message_t * msg = (ecds_message_t *)ecds_list_get_item(ECDS_LIST(disp->message_queue), iter);
			ecds_dispatcher_dispatch_message(disp, msg);
			ecds_list_dispose_item(iter);
		}

		running = disp->running;
		
		pthread_mutex_unlock(disp->dispatcher_mutex);
	}

	return NULL;
}

static void _dispatcher_init(ecds_dispatcher_t * disp)
{
	pthread_mutex_init(disp->dispatcher_mutex, 0);
	pthread_create(disp->dispatcher_thread, 0, 
				   _dispatcher_thread, disp);
}

static void _dispatcher_dispose(ecds_dispatcher_t * disp)
{
	int retval;

	pthread_mutex_lock(disp->dispatcher_mutex);
	disp->running = false;
	pthread_mutex_unlock(disp->dispatcher_mutex);

	pthread_join(disp->dispatcher_thread[0], (void **)&retval);
	pthread_mutex_destroy(disp->dispatcher_mutex);

}

ecds_object_t * ecds_dispatcher_construct(const char * name) 
{
	char queue_name[80];
	ecds_dispatcher_t * ret = (ecds_dispatcher_t *)ecds_object_new(name, sizeof(ecds_dispatcher_t), ECDS_DISPATCHER);

	sprintf(queue_name, "%s-queue", name);
	ret->message_queue = ecds_queue_new();
	ecds_object_rename(ECDS_OBJECT(ret->message_queue), queue_name);

	ecds_log_info("Constructing new dispatcher: %s", ecds_object_get_name(ECDS_OBJECT(ret)));

	return (ecds_object_t *)ret;
}

void ecds_dispatcher_queue_message(ecds_dispatcher_t * disp, ecds_message_t * msg) 
{
	pthread_mutex_lock(disp->dispatcher_mutex);
	ecds_queue_enqueue(disp->message_queue, ECDS_OBJECT(msg));
	pthread_mutex_unlock(disp->dispatcher_mutex);
}

void ecds_dispatcher_subscribe(ecds_dispatcher_t * disp,
							   unsigned int event_id, 
							   ecds_service_t * service)
{
	for (ecds_list_item_t * iter = ecds_list_first_item(disp->event_list); iter; iter = ecds_list_next_item(iter))
	{
		ecds_dispatcher_event_t * evt = (ecds_dispatcher_event_t *)ecds_list_get_item(disp->event_list, iter);
		if (evt->event_id == event_id)
		{
			/* Existing event found, subscribe */
			ecds_list_add_item(evt->service_list, ECDS_OBJECT(service));
			ecds_log_info("Adding service %s for event ID %08X", ecds_object_get_name(ECDS_OBJECT(service)), event_id);
		}
	}

	/* Event type was not found, add new */
	char event_name[128];
	sprintf(event_name, "%s-event-%8X", ecds_object_get_name(ECDS_OBJECT(disp)), event_id);

	ecds_log_info("Adding new event ID %08X", event_id);
	ecds_dispatcher_event_t * evt = (ecds_dispatcher_event_t *)ecds_object_new(event_name, sizeof(ecds_dispatcher_event_t), ECDS_DISPATCHER_EVENT);
	evt->service_list = ecds_list_new();
	ecds_list_add_item(evt->service_list, ECDS_OBJECT(service));
	ecds_log_info("Adding service %s for event ID %08X", ecds_object_get_name(ECDS_OBJECT(service)), event_id);
}

void ecds_dispatcher_dispatch_message(ecds_dispatcher_t * disp, ecds_message_t * msg)
{
	ecds_dispatcher_event_t * event = NULL;

	for (ecds_list_item_t * iter = ecds_list_first_item(disp->event_list); iter; iter = ecds_list_next_item(iter))
	{
		ecds_dispatcher_event_t * evt = (ecds_dispatcher_event_t *)ecds_list_get_item(disp->event_list, iter);
		if (evt->event_id == msg->event_id)
		{
			event = evt;
			break;
		}
	}

	if (event == NULL)
	{
		/* No handlers found, register new event type only */
		char event_name[128];
		sprintf(event_name, "%s-event-%8X", ecds_object_get_name(ECDS_OBJECT(disp)), msg->event_id);

		ecds_log_info("Received message with unknown event ID %08X, registering for future dispatch", msg->event_id);
		ecds_dispatcher_event_t * evt = (ecds_dispatcher_event_t *)ecds_object_new(event_name, sizeof(ecds_dispatcher_event_t), ECDS_DISPATCHER_EVENT);
		evt->service_list = ecds_list_new();
		return;
	}

	for (ecds_list_item_t * iter = ecds_list_first_item(event->service_list); iter; iter = ecds_list_next_item(iter))
	{
		ecds_service_t * svc = (ecds_service_t *)ecds_list_get_item(event->service_list, iter);
		ecds_service_dispatch_message(svc, disp, msg);
	}
}
