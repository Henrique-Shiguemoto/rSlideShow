#include "r_logger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static FILE* log_file_handle = NULL;
static int _rlog_initialized = 0;
static rLogger_mode current_rLogger_mode = RLOG_TERMINAL_MODE;

int rLogger_init(rLogger_mode mode){
	if(_rlog_initialized) return 1;
	_rlog_initialized = 1;

	int result = 0;
	switch(mode){
		case RLOG_TERMINAL_MODE:
		{
			current_rLogger_mode = RLOG_TERMINAL_MODE;
			RLOGGER_INFO("%s", "Succesfully initialized for terminal logging.");
			break;
		} 
		case RLOG_FILE_MODE: 
		{
			current_rLogger_mode = RLOG_FILE_MODE;
			log_file_handle = fopen(LOG_FILE_NAME, "w+");
			if(!log_file_handle){
				RLOGGER_WARN("%s", "Could'nt open log file.");
				result = 1;
			}
			RLOGGER_INFO("%s", "Succesfully initialized for file logging.");
			break;
		}
		case RLOG_TERMINAL_AND_FILE_MODE:
		{
			current_rLogger_mode = RLOG_TERMINAL_AND_FILE_MODE;
			log_file_handle = fopen(LOG_FILE_NAME, "w+");
			if(!log_file_handle){
				RLOGGER_WARN("%s", "Could'nt open log file.");
				result = 1;
			}
			RLOGGER_INFO("%s", "Succesfully initialized for text and file logging.");
			break;
		}
		default: 
		{
			current_rLogger_mode = RLOG_TERMINAL_MODE;
			RLOGGER_WARN("%s", "Unknown logging mode, defaulting to terminal logging mode.");
			result = 1;
			break;
		}
	}
	return result;
}

void rLogger_set_mode(rLogger_mode mode){
	if(mode >= RLOG_TERMINAL_MODE && mode <= RLOG_MODE_COUNT){
		current_rLogger_mode = mode;
	}else{
		current_rLogger_mode = RLOG_TERMINAL_MODE;
	}
}

void rLogger_message(rLog_level level, const char* format, ...){
	if(level < 0 || level > RLOG_LEVEL_COUNT || !_rlog_initialized) return;
	const char* message_prefixes[RLOG_LEVEL_COUNT] = {"[DEBUG]: ", "[INFO]:  ", "[WARN]:  ", "[ERROR]: "};
	char output_buffer[OUTPUT_BUFFER_SIZE];
	memset(output_buffer, 0, OUTPUT_BUFFER_SIZE);

	va_list args;
	va_start(args, format);
	vsnprintf(output_buffer, OUTPUT_BUFFER_SIZE, format, args);
	va_end(args);

	// we're adding 10 here because that's the most amount of bytes we'll need to add a prefix
	// this makes sure that the output of snprintf isn't truncated.
	char output_buffer_aux[OUTPUT_BUFFER_SIZE + 10];
	snprintf(output_buffer_aux, OUTPUT_BUFFER_SIZE + 10, "%s%s\n", message_prefixes[level], output_buffer);

	//this is verifying if the we either need to print to the terminal or write to a file or both
	if(((current_rLogger_mode + 1) & (RLOG_TERMINAL_MODE + 1)) != 0) 
		printf("%s", output_buffer_aux);
	if(((current_rLogger_mode + 1) & (RLOG_FILE_MODE + 1)) != 0)
	 	fwrite(output_buffer_aux, strlen(output_buffer_aux), 1, log_file_handle);
}

void rLogger_quit(void){
	if(_rlog_initialized) {
		RLOGGER_INFO("%s", "Quit from terminal logging.");
		if(log_file_handle) fclose(log_file_handle);
	}
}