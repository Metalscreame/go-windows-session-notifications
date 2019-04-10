#include <windows.h>
#include <WtsApi32.h>
#include "_cgo_export.h"

#define CTRL_SHUTDOWN  10
#define CTRL_LOGOFF    20
#define CTRL_ERROR     999

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_SHUTDOWN_EVENT:
		{
            relayMessage(CTRL_SHUTDOWN, CTRL_SHUTDOWN);
			return TRUE;
		}
	case CTRL_LOGOFF_EVENT:
		{
        relayMessage(CTRL_LOGOFF, CTRL_LOGOFF);
			return TRUE;
		}
	default:
		{
            фrelayMessage(CTRL_ERROR, CTRL_ERROR);
		}
	}
	return FALSE;
}

HANDLE Start() {
    DWORD lpThreadId, lpParameter = 1;
    HANDLE hThread;

    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
           relayMessage(CTRL_ERROR, CTRL_ERROR);
}
