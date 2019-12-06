/*****************************************************************************/
/*	ecds_process.h: Abstract definition of ECDS process type				 */
/*																			 */
/*****************************************************************************/

#ifndef _ECDS_PROCESS_H
#define _ECDS_PROCESS_H

#include <ecds.h>
#include <core/ecds_object.h>

struct _ecds_process_t {
	ecds_object_t obj;

	ecds_object_t* private_data;

	//!< Called when the process is first started up.
	void (* initialize)(ecds_process_t * proc);
	
	//!< Called when the process is run cyclically.
	void (* run)(ecds_process_t * proc);
	
	//!< Called when the process manager is about to dispose.
	void (* shutdown)(ecds_process_t * proc);
};

#endif /* _ECDS_PROCESS_H */