/*****************************************************************************/
/*	ecds_object.h: Abstract definition of ECDS object type					 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_OBJECT_H
#define _ECDS_OBJECT_H

#include <ecds.h>

struct _ecds_object_t {
	char * name;		//!<	Object instance name
	uint32_t uid;		//!<	Object instance unique identifier
	uint32_t type_uid;	//!<	Object type unique identifier
	ecds_memory_manager_t * manager;
		
	//!< @brief Object constructor, called when the object is newly created.
	void (* construct)(ecds_object_t * obj);
	
	//!< @brief Object destructor, called when the object is about to be disposed.
	void (* dispose)(ecds_object_t * obj);

	//!< Property getters and setters
	void * (* get_property)(ecds_object_t * obj, uint32_t property_id);
	void (* set_property)(ecds_object_t * obj, uint32_t property_id, void * value);
};

#endif /* _ECDS_OBJECT_H */