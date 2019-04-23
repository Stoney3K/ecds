/*****************************************************************************/
/*	ecds_object.h: Abstract definition of ECDS object type					 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_OBJECT_H
#define _ECDS_OBJECT_H

#define ECDS_OBJECT(o)((o) == NULL) ? NULL : & (ecds_object_t *)(o))

typedef struct _ecds_object_h ecds_object_h;

struct _ecds_object_h {
	const char * name;	//!<	Object instance name
	uint32_t uid;		//!<	Object instance unique identifier
	uint32_t type_uid;	//!<	Object type unique identifier
		
	//!< @brief Object constructor, called when the object is newly created.
	void (* construct)(ecds_object_t * obj);
	
	//!< @brief Object destructor, called when the object is about to be disposed.
	void (* dispose)(ecds_object_t * obj);
}

#endif /* _ECDS_OBJECT_H */