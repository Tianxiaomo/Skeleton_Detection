#include "sys.h"
#include "stdio.h"
#include "stdarg.h"
#include "log.h"

static u8 debuglevel;
static u8 debugcolor;
void debug_init(u8 level,u8 color){
	debuglevel = level;
	debugcolor = color;
}

//#define print_debug(format,...) printf("%s"format"%s","\x1b[0;31;40m", ##__VA_ARGS__,"\x1b[0m")
//#define LOG(level, format, ...) 	do{fprintf(stdout, "\x1b[0;31;40m[%s|%s@%s,%d] " format "\x1b[0m\r\n",level, __func__, __FILE__, __LINE__, ##__VA_ARGS__ );} while (0) 


/** @internal
Do not use directly, use the debug macro */
void
_debug(const char *filename, int line, int level, const char *format, ...)
{
    va_list vlist;
	if(debugcolor == 0){
    if (debuglevel >= level) {
		switch (level){
			case DEBUG:
				printf("[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\r\n");
				break;
			case INFO:
				printf("[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\r\n");
				break;
			case WARN :
				printf("[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\r\n");
				break;
			case ERROR:
				printf("[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\r\n");
				break;
			case FATAL:
				printf("[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\r\n");
				break;
	}}}
	else{
    if (debuglevel >= level) {
		switch (level){
			case DEBUG:
				printf("\x1b[0;32;40m[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\x1b[0m\r\n");
				break;
			case INFO:
				printf("\x1b[0;34;40m[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\x1b[0m\r\n");
				break;
			case WARN :
				printf("\x1b[0;33;40m[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\x1b[0m\r\n");
				break;
			case ERROR:
				printf("\x1b[0;31;40m[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\x1b[0m\r\n");
				break;
			case FATAL:
				printf("\x1b[0;37;41m[%d](%s:%d)",level,filename, line);
				va_start(vlist, format);
				vfprintf(stdout,format,vlist);
				va_end(vlist);
				printf("\x1b[0m\r\n");
				break;
	}}}	
}
