#pragma once

#define ECDS_DEBUG 0
#define ECDS_INFO  1
#define ECDS_WARN  2
#define ECDS_ERROR 3
#define ECDS_FATAL 4

#define ecds_log_fatal(...) ( ecds_log(ECDS_FATAL, ECDS_LOG_DOMAIN, __VA_ARGS__) )
#define ecds_log_error(...) ( ecds_log(ECDS_ERROR, ECDS_LOG_DOMAIN, __VA_ARGS__) )
#define ecds_log_warning(...) ( ecds_log(ECDS_WARN, ECDS_LOG_DOMAIN, __VA_ARGS__) )
#define ecds_log_info(...) ( ecds_log(ECDS_INFO, ECDS_LOG_DOMAIN, __VA_ARGS__) )
#define ecds_log_debug(...) ( ecds_log(ECDS_DEBUG, ECDS_LOG_DOMAIN, __VA_ARGS__) )

/**
 * @brief Function prototype for custom log handlers
 * ecds_log_format_func is a function that can be defined if handlers want to define their own custom
 * output format for log messages, it takes the log level and sprintf syntax to output into a target string.
 *
 * ecds_log_output_func is a function that can output log text into a different channel. Each handler can define either
 * one of these functions, if they are set to NULL, the default will be used which outputs standard format to the console.
 */
typedef void (* ecds_log_format_func)(char * target, int level, const char* fmt, va_list arguments);
typedef void (* ecds_log_output_func)(char * message);

void ecds_log(int level, const char * domain, const char * fmt, ...);
void ecds_log_set_level(int new_level);