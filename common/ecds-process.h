/*****************************************************************************/
/*	ecds_process.h: Abstract definition of ECDS process type				 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_OBJECT_H
#define _ECDS_OBJECT_H

#include <common/ecds-object.h>

#define ECDS_PROCESS(o)((o) == NULL) ? NULL : & (ecds_process_t *)(o))

typedef struct _ecds_process_h ecds_process_h;

struct _ecds_process_h {
	ecds_object_t obj;
			
	//!< Called when the process is first started up.
	void (* initialize)(ecds_process_t * proc);
	
	//!< Called when the process is run cyclically.
	void (* run)(ecds_process_t * proc);
	
	//!< Called when the process manager is about to dispose.
	void (* shutdown)(ecds_process_t * proc);
}

#endif /* _ECDS_PROCESS_H */