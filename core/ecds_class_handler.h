/*****************************************************************************/
/*	@file ecds_class_handler.h												 */
/*	@brief ECDS class handler												 */
/*																			 */
/*	The class handler takes care of all object types that are registered and */
/*	will call the appropriate constructor when one of those types is 		 */
/*	instantiated.															 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_CLASS_HANDLER_H
#define _ECDS_CLASS_HANDLER_H

#include <core/ecds_object.h>

//=================================== 8< ====================================//
#define ECDS_TYPE_CLASS_HANDLER 			0xFFFFFFFC
#define ECDS_TYPE_CLASS_HANDLER_ENTRY		0xFFFFFFFB
//===========================================================================//
typedef struct _ecds_class_handler_t ecds_class_handler_t;
				
/**
 * @brief Look up the type UID for a given class.
 * @param type_name The class type name to look up.
 * @return The type UID of the class if it is registered.
 */
uint32_t ecds_get_class_uid(const char * type_name);

/**
 * @brief Register a property for a given class.
 */
void ecds_register_property(const char class_name, 
							const char property_name,
							void * (* get)(ecds_object_t * obj, uint32_t property_id),
							void (* set)(ecds_object_t * obj, uint32_t property_id, void * value));

void ecds_set_property(ecds_object_t * obj, const char * property_name, void * value);

void * ecds_get_property(ecds_object_t * obj, const char * property_name);

/**
 * @brief Construct a new object of a given type UID and take a reference on it. Constructing by UID is marginally faster.
 * @param type_uid The type UID to use.
 * @return A pointer to the new object, or NULL if the construction failed.
 */
ecds_object_t * ecds_construct_object_by_uid(const char * type_uid);

ecds_class_handler_t * ecds_class_handler_get_default();


#endif /* _ECDS_CLASS_HANDLER_H */