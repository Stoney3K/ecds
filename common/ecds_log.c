#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <ecds.h>

#define ECDS_LOG_DOMAIN "ecds-log"

int ecds_log_current_level = ECDS_INFO;

void ecds_log_format_default(char * target, int length, int level, const char * domain, const char * fmt, va_list arguments)
{
	int position = 0;

	if (level < ecds_log_current_level)
		return;
	/* TODO: Add date/time stamp to default logging messages */
	sprintf_s(target, length, "[%s] ", domain);
	position += strlen(target);

	switch (level)
	{
	case ECDS_DEBUG:
		sprintf_s(target + position, length - position, "D: ");
		break;
	case ECDS_INFO:
		sprintf_s(target + position, length - position, "I: ");
		break;
	case ECDS_WARN:
		sprintf_s(target + position, length - position, "W: ");
		break;
	case ECDS_ERROR:
		sprintf_s(target + position, length - position, "E: ");
		break;
	case ECDS_FATAL:
		sprintf_s(target + position, length - position, "F: ");
		break;
	}
	position += 3;

	vsprintf_s(target + position, length - position, fmt, arguments);
}

void ecds_log_output_default(char* message)
{
	if (!message)
		return;
	printf("%s\n", message);
}

void ecds_log(int level, const char * domain, const char* fmt, ...)
{
	char log_message[120];
	va_list arguments;
	va_start(arguments, fmt);

	/* TODO: Add testing for hooks here so more log methods can attach */
	ecds_log_format_default(log_message, 120, level, domain, fmt, arguments);
	ecds_log_output_default(log_message);

	va_end(arguments);

	if (level == ECDS_FATAL)
		exit(EXIT_FAILURE);
}

void ecds_log_set_level(int new_level)
{
	if (new_level > ECDS_FATAL)
	{
		ecds_log_error("Invalid log level specified");
		return;
	}

	ecds_log_current_level = new_level;
}