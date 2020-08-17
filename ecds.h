/*****************************************************************************/
/*	@file ecds.h												 		     */
/*	@brief Main user space header file										 */
/*																			 */
/* 	This file contains the ecds_core module which contains all of the 		 */
/*	services required for basic operation of ECDS, including the dispatcher, */
/*	module manager, configuration file loader and main loop.				 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_H
#define _ECDS_H

#ifdef WIN32
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#endif

#include <common/ecds_version.h>
#include <common/ecds_log.h>

//=================================== 8< ====================================//
//								  DATA TYPES								 //
//===========================================================================//
#define ECDS_OBJECT(o) ( ((o) == NULL) ? NULL : (ecds_object_t *)((o)) )
#define ECDS_PROCESS(o) ( ((o) == NULL) ? NULL : (ecds_process_t *)((o)) )
#define ECDS_SERVICE(o) ( ((o) == NULL) ? NULL : (ecds_service_t *)((o)) )
#define ECDS_LIST(o) ( ((o) == NULL) ? NULL : (ecds_list_t *)((o)) )

typedef struct _ecds_message_t ecds_message_t;
typedef struct _ecds_service_t ecds_service_t;
typedef struct _ecds_object_t ecds_object_t;
typedef struct _ecds_process_t ecds_process_t;
typedef struct _ecds_module_t ecds_module_t;
typedef struct _ecds_list_t ecds_list_t;
typedef struct _ecds_list_item_t ecds_list_item_t;

typedef struct _ecds_memory_manager_t ecds_memory_manager_t;

//=================================== 8< ====================================//
//				GENERAL MEMORY MANAGEMENT FUNCTIONS FOR ECDS				 //
//===========================================================================//
/**
* @brief Construct a new object of a given size and take a reference on it.
* @param name The object name for the new object, or NULL to leave it at default.
* @param size The amount of memory to allocate for the object in bytes.
* @param type The integer type identifier to use for this object.
* @return A pointer to the new object, or NULL if the construction failed.
*/
ecds_object_t * ecds_object_new(
					const char * name, 
					size_t size, 
					uint32_t type);

/**
* @brief Construct a new object of a given class and take a reference on it.
* @param type_name The class type name to use.
* @param object_name The name for the new object, or NULL to leave it at default.
* @return A pointer to the new object, or NULL if the construction failed.
*/
ecds_object_t * ecds_object_construct(
					const char * type_name, 
					const char * object_name);

/**
* @brief Modify the name of an object.
* @param target The object to manipulate.
* @param new_name The new name for the object.
*/
void ecds_object_rename(
	ecds_object_t * target,
	const char * new_name);

//!< @brief Take an additional reference on an object.
void ecds_object_ref(ecds_object_t * obj);

//!< @brief Decrease reference on an object and dispose it if necessary.
void ecds_object_unref(ecds_object_t * obj);

//!< @brief Get the canonical name of an object.
char * ecds_object_get_name(ecds_object_t * obj);

/**
* @brief Register a new class.
* @param type_name The type name to use.
* @param type_uid The explicit type UID to use, or 0 to let the class handler select it.
* @param construct A pointer to the constructor function for the class.
* @return The type UID of the class if registration was succesful, or 0 if it failed.
*/
uint32_t ecds_register_class(
			const char * type_name,
			uint32_t type_uid,
			ecds_object_t * (*construct)());

//=================================== 8< ====================================//
//						 MODULE LOADING AND UNLOADING						 //
//===========================================================================//
/**
* @brief Loads a module from disk and registers it.
*/
void ecds_load_module(const char * path);

/**
* @brief Loads all modules in a single directory to register them.
*/
void ecds_enumerate_modules(const char * path);

//=================================== 8< ====================================//
//							 MESSAGE BUS CONNECTION							 //
//===========================================================================//
/**
* @brief Register a new event handler
* @param event_id The integer event type to register. When the type is already registered,
*		  the service will be added to the subscriptions.
* @param service The service to attach to the global dispatcher.
*/
void ecds_message_bus_attach(unsigned int bus_number, ecds_service_t * service);

/**
* @brief Post a new event to the global dispatcher.
* @param event The ecds_event_t * structure that contains the event to post.
*/
void ecds_message_post(ecds_message_t * event);

/**
* @brief Flush the event queue and dispatch all remaining events.
*/
void ecds_message_flush();

#endif /* _ECDS_H */