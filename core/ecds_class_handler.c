/*****************************************************************************/
/*	@file ecds_class_handler.c												 */
/*	@brief Implementation for ECDS class handler							 */
/*																			 */
/*	The class handler keeps track of all registered classes and their		 */
/*	associated public methods. In principle, all ECDS classes should expose	 */
/*	their constructor method along with property getter and setter methods.	 */
/*																			 */
/*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ECDS_LOG_DOMAIN "ecds_class_handler"

#include <ecds.h>
#include <common/ecds_list.h>

#include <core/ecds_memory_manager.h>
#include <core/ecds_class_handler.h>


typedef struct _ecds_class_handler_entry_t ecds_class_handler_entry_t;

struct _ecds_class_handler_entry_t {
	ecds_object_t obj;

	uint32_t class_uid;
	char * class_name;
	ecds_object_t * (* constructor)(const char * object_name);
};

struct _ecds_class_handler_t {
	ecds_object_t obj;

	ecds_list_t * classes;	//!<	List of registered classes
};

static ecds_class_handler_t * ecds_class_handler_default = NULL;

ecds_class_handler_t * ecds_class_handler_get_default()
{
	if (ecds_class_handler_default == NULL)
	{
		ecds_log_info("Creating default class handler");
		ecds_class_handler_default = (ecds_class_handler_t *)ecds_object_new("ecds-class-handler-default", sizeof(ecds_class_handler_t), ECDS_TYPE_CLASS_HANDLER);
		ecds_class_handler_default->classes = ecds_list_new();
	}

	return ecds_class_handler_default;
}

uint32_t ecds_register_class(
	const char * type_name,
	uint32_t type_uid,
	ecds_object_t * (*construct)(const char * object_name))
{
	char class_entry_name[120];
	ecds_list_item_t * item = NULL;
	ecds_class_handler_entry_t * entry = NULL;
	ecds_class_handler_t * class_handler = ecds_class_handler_get_default();

	/* Search if the class is already registered first */
	for (item = ecds_list_first_item(class_handler->classes); item; item = ecds_list_next_item(item))
	{
		entry = (ecds_class_handler_entry_t *)ecds_list_get_item(class_handler->classes, item);
		if (strcmp(type_name, entry->class_name) == 0)
			/* Class is already registered */
			return entry->class_uid;
	}

	sprintf(class_entry_name, "class_entry_%s", type_name);
	entry = (ecds_class_handler_entry_t *)ecds_object_new(class_entry_name, sizeof(ecds_class_handler_entry_t), ECDS_TYPE_CLASS_HANDLER_ENTRY);

	entry->class_name = strdup(type_name);
	if (type_uid == 0)
		entry->class_uid = (uint32_t)rand() & 0x00FFFFFF;
	else
		entry->class_uid = type_uid;
	entry->constructor = construct;

	ecds_log_info("Registering class: %s", type_name);
	ecds_list_add_item(class_handler->classes, ECDS_OBJECT(entry));

	return entry->class_uid;
}

uint32_t ecds_get_class_uid(const char * type_name)
{
	ecds_list_item_t * item = NULL;
	ecds_class_handler_entry_t * entry = NULL;
	ecds_class_handler_t * class_handler = ecds_class_handler_get_default();

	/* Search if the class is already registered first */
	for (item = ecds_list_first_item(class_handler->classes); item; item = ecds_list_next_item(item))
	{
		entry = (ecds_class_handler_entry_t *)ecds_list_get_item(class_handler->classes, item);
		if (strcmp(type_name, entry->class_name) == 0)
			/* Class is already registered */
			return entry->class_uid;
	}

	return 0;
}

ecds_object_t * ecds_object_construct(const char * type_name, const char * object_name)
{
	ecds_list_item_t * item = NULL;
	ecds_class_handler_entry_t * entry = NULL;
	ecds_class_handler_t * class_handler = ecds_class_handler_get_default();
	ecds_object_t * ret = NULL;

	for (item = ecds_list_first_item(class_handler->classes); item; item = ecds_list_next_item(item))
	{
		entry = (ecds_class_handler_entry_t *)ecds_list_get_item(class_handler->classes, item);
		if (strcmp(type_name, entry->class_name) == 0)
		{
			if (object_name == NULL)
			{
				char temp_name[128];
				sprintf(temp_name, "%s-obj", type_name);
				ret = entry->constructor(temp_name);
			}
			else
				ret = entry->constructor(object_name);

			return ret;
		}
	}

	/* Class type name was not found */
	ecds_log_warning("Unable to construct object of type %s: Type not registered", type_name);
	return NULL;
}

