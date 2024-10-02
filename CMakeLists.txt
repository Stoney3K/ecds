cmake_minimum_required(VERSION 3.10)

project(ecds) 

add_library(ecds_core   core/ecds_class_handler.c
                        core/ecds_dispatcher.c
                        core/ecds_memory_manager.c
                        core/ecds_module_manager.c
                        core/ecds_service.c)

add_library(ecds        common/ecds_list.c 
                        common/ecds_log.c 
                        common/ecds_queue.c)

target_include_directories(ecds_core PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(ecds PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(ecds_launcher core/ecds_launcher.c)
target_link_libraries(ecds_core ecds)
target_link_libraries(ecds_launcher ecds_core)
target_include_directories(ecds_launcher PRIVATE ${CMAKE_SOURCE_DIR})
