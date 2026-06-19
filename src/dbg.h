#ifndef dbg_h 
#define dbg_h 

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

void genericLog(const char *tag, const char *current_file, int current_line, const char *fmt, ...);
void genericDebug(const char *current_file, int current_line, const char *fmt, ...);
void closeLog();

#ifndef NDEBUG
    #define debug(...) 
#else
    #define debug(...) genericDebug(__FILE__, __LINE__, __VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(...) genericLog("[ERROR]", __FILE__, __LINE__, __VA_ARGS__)

#define log_warn(...) genericLog("[WARN]", __FILE__, __LINE__, __VA_ARGS__)

#define log_info(...) genericLog("[INFO]", __FILE__, __LINE__, __VA_ARGS__)

#define check(A, ...) if(!(A)){ log_err(__VA_ARGS__); errno = 0; goto error; }

#define sentinel(...) {log_err(__VA_ARGS__);errno = 0; goto error; }

#define check_mem(A) check((A),"Out of memory.")

#define check_debug(A,...) if(!(A)) {debug(__VA_ARGS__); errno = 0; goto error; }

#endif


