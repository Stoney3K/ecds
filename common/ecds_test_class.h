/*****************************************************************************/
/*	ecds_test_class.h: Unit test class which implements functions for an	 */
/*					   object												 */
/*																			 */
/*****************************************************************************/


#ifndef _ECDS_TEST_CLASS_H
#define _ECDS_TEST_CLASS_H

#include <ecds.h>

#define ECDS_TYPE_TEST	0x0FFFFFFF

typedef struct _ecds_test_class_t ecds_test_class_t;

ecds_object_t * ecds_test_class_construct();

#endif