#include <common/ecds_test_class.h>

#include <core/ecds_object.h>
#include <core/ecds_memory_manager.h>

#define ECDS_LOG_DOMAIN "ecds_test"

struct _ecds_test_class_t {
	ecds_object_t obj;
};

ecds_object_t * ecds_test_class_construct()
{
	ecds_test_class_t * ret = (ecds_test_class_t *)ecds_object_new("ecds-test-class", sizeof(ecds_test_class_t), ECDS_TYPE_TEST);

	ecds_log_info("Constructing new test class");

	return (ecds_object_t *)ret;
}