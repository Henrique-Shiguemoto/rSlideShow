#ifndef RLOGGER_H
#define RLOGGER_H

#define LOG_FILE_NAME 			"rLog.log"
#define OUTPUT_BUFFER_SIZE 		16384

typedef enum rLogger_mode {
	RLOG_TERMINAL_MODE = 0,
	RLOG_FILE_MODE,
	RLOG_TERMINAL_AND_FILE_MODE,
	RLOG_MODE_COUNT
} rLogger_mode;

typedef enum rLog_level {
	RLOG_DEBUG = 0,
    RLOG_INFO,
    RLOG_WARNING,
    RLOG_ERROR,
    RLOG_LEVEL_COUNT
} rLog_level;

int rLogger_init(rLogger_mode mode);
void rLogger_set_mode(rLogger_mode mode);
void rLogger_message(rLog_level level, const char* format, ...);
void rLogger_quit(void);

#define RLOGGER_DEBUG(message, ...) rLogger_message(RLOG_DEBUG, message, __VA_ARGS__);
#define RLOGGER_INFO(message, ...)  rLogger_message(RLOG_INFO, message, __VA_ARGS__);
#define RLOGGER_WARN(message, ...)  rLogger_message(RLOG_WARNING, message, __VA_ARGS__);
#define RLOGGER_ERROR(message, ...) rLogger_message(RLOG_ERROR, message, __VA_ARGS__);

#endif // RLOGGER_H