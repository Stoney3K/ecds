/*****************************************************************************/
/*	@file ecds_module_manager.c											 	 */
/*	@brief ECDS module manager.											 	 */
/*																			 */
/*****************************************************************************/

#ifdef WIN32
	#include <windows.h>
#else
#endif
#include <ecds.h>
#include <common/ecds_module.h>

/**
 * @brief Loads a module from disk and registers it.
 */
void ecds_load_module(const char * path)
{
	ecds_module_manager_t * module_manager = ecds_module_manager_get_default();

	if (!module_manager)
	{
		ecds_warn("Unable to load module from %s: No module manager available", path);
		return;
	}

#ifdef WIN32
	HMODULE dl_handle = LoadLibraryA(path);
	if(dl_handle == NULL)
	{
		ecds_log(ECDS_WARN, "Unable to load module from %s", path);
		return;
	}

	ecds_module_constructor_t * ecds_module_construct = (ecds_module_constructor_t *)GetProcAddress(dl_handle, "ecds_module_construct");
#else
	/* UNIX compatible code */
	void * dl_handle = dlopen(path, RTLD_LAZY);
	if(!dl_handle)
	{
		ecds_log(ECDS_WARN, "Unable to load module from %s", path);
		return;
	}
	ecds_module_constructor_t * ecds_module_construct = (ecds_module_constructor_t *)dlsym(dl_handle, "ecds_module_construct");
#endif
	if(!ecds_module_construct)
	{
		ecds_log(ECDS_WARN, "Library file at %s is not an ECDS module", path);
		return;
	}	
	
	ecds_module_t * module = ecds_module_construct();
	if(!module)
	{
		ecds_log(ECDS_WARN, "Error loading module at %s", path);
		return;
	}	
		
	module->library_handle = dl_handle;
	ecds_module_manager_register_module(module);
}

/**
 * @brief Loads all modules in a single directory to register them.
 */
void ecds_enumerate_modules(const char * path)
{
	
}