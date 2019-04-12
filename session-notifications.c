#include <windows.h>
#include <wtsapi32.h>
#include "_cgo_export.h"

#define CTRL_SHUTDOWN  10
#define CTRL_LOGOFF    20
#define CTRL_ERROR     999

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_QUERYENDSESSION:
        ShutdownBlockReasonCreate(hWnd, L"Don't do it!");
        relayMessage(message, lParam);
        break;
    case WM_WTSSESSION_CHANGE:
        relayMessage(message, wParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
            relayMessage(CTRL_ERROR, CTRL_ERROR);
		}
	}
	return FALSE;
}

DWORD WINAPI WatchSessionNotifications(LPVOID lpParam)
{
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    char const *lpClassName="classWatchSessionNotifications";

    wc.lpfnWndProc = WndProc;
    wc.lpszClassName=lpClassName;

    if (!RegisterClass(&wc))
        return 0;

    hwnd = CreateWindow(lpClassName,
                        lpClassName,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,CW_USEDEFAULT,100,100,
                        NULL,NULL,NULL,NULL);

    if (!hwnd)
        return 0;

    UpdateWindow(hwnd);
    WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_THIS_SESSION);

    while (GetMessage(&msg,NULL,0,0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HANDLE Start() {
    DWORD lpThreadId, lpParameter = 1;
    HANDLE hThread;

    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
           relayMessage(CTRL_ERROR, CTRL_ERROR);

        hThread = CreateThread(
                      NULL,
                      0,
                      WatchSessionNotifications,
                      &lpParameter,
                      0,
                      &lpThreadId);

        if (hThread == NULL)
            return 0;
        else
            return hThread;

}
