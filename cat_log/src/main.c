
#include "cat_log.h"

int main(void)
{
    CLOG_ERROR  ("%s\r\n", "just test error");
    CLOG_WARNING("%s\r\n", "just test warning");
    CLOG_INFO   ("%s\r\n", "just test info");
    CLOG_DEBUG  ("%s\r\n", "just test debug");
    CLOG_TRACE  ("%s\r\n", "just test trace");

    return 0;
}