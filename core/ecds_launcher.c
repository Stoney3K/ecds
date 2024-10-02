/*****************************************************************************/
/*	@file ecds_launcher.c													 */
/*	@brief ECDS application launcher										 */
/*																			 */
/*  This file contains the main application entry point and the parser for	 */
/*  command-line arguments.													 */
/*																			 */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ecds.h>
#ifdef WIN32
	#include <conio.h>
#endif
#include <tests/ecds_test_class.h>
#include <core/ecds_dispatcher.h>

#define ECDS_LOG_DOMAIN "ecds-launcher"

int main(int argc, char** argv)
{
	ecds_test_class_t * ret = NULL;
	ecds_dispatcher_t * disp = NULL;

	ecds_log_set_level(ECDS_DEBUG);
	ecds_log_info("ECDS version %d.%d.%d starting up", ECDS_VERSION_MAJOR, ECDS_VERSION_MINOR, ECDS_VERSION_BUILD);

	ecds_register_class("ecds-dispatcher-class", ECDS_DISPATCHER, ecds_dispatcher_construct);
	disp = (ecds_dispatcher_t *)ecds_object_construct("ecds-dispatcher-class", "dispatcher-default");

	//	ecds_register_class("ecds-test-class", ECDS_TYPE_TEST, ecds_test_class_construct);
	//	ret = (ecds_test_class_t *)ecds_object_construct("ecds-test-class", NULL);


	ecds_object_unref(ECDS_OBJECT(ret));
	
	ecds_log_info("Program completed.");

	return EXIT_SUCCESS;
}