/*
 * SPCMEM - Realtime Memory Viewer for SNES SPC700 Player
 */

#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' "\
    "version='6.0.0.0' "\
    "processorArchitecture='*' "\
    "publicKeyToken='6595b64144ccf1df' "\
    "language='*'\"")

#include <windows.h>
#include <stdint.h>

#include "CWindow.h"
#include "memView.h"
#include "spcmem.h"
#include "spcapi.h"
#include "resource.h"

/*
 * Get instance handle of application.
 */
static HINSTANCE spcmem_hInstance = NULL;
HINSTANCE App_GetInstance(void)
{
	return spcmem_hInstance;
}

/**
 * Check existing process.
 */
static HANDLE spcmem_hMutex = NULL;
bool App_IsSingleRun(void)
{
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
	SECURITY_ATTRIBUTES secAttribute;
	secAttribute.nLength = sizeof(secAttribute);
	secAttribute.lpSecurityDescriptor = &sd;
	secAttribute.bInheritHandle = TRUE;

	HANDLE hMutex = CreateMutex(&secAttribute, FALSE, TEXT("SPCMEM_SINGLERUN_MUTEX"));
	bool singleRun = true;
	if(hMutex == NULL)
	{
		singleRun = false;
	}
	else if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		singleRun = false;
		CloseHandle(hMutex);
	}
	else
	{
		spcmem_hMutex = hMutex;
	}

	return singleRun;
}

#define TITLE_HOST_ATTACHED     "SPC700 Memory"
#define TITLE_HOST_NOT_ATTACHED "SPC700 Memory (Inactive)"

/**
 * Application main
 */
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// save application instance
	spcmem_hInstance = hInstance;

	if (!App_IsSingleRun())
	{
		return 0;
	}

	HACCEL hAccel = LoadAccelerators(spcmem_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	if (!RegWndClass(TEXT("MemView_ViewBox"), MemView_ViewBoxProc, 0, sizeof(CMemView*)))
		return 0;

	CMemView* MemView = new CMemView(); // will be automatically deleted
	OpenToolWindow(MemView);

	SetWindowText(MemView->hWnd, TEXT(TITLE_HOST_NOT_ATTACHED));
	SetClassLongPtr(MemView->hWnd, GCLP_HICON, (LONG_PTR)LoadIcon(App_GetInstance(), MAKEINTRESOURCE(IDI_ICON1)));

	bool spcHostAttached = false;
	bool spcHostAttachedPrev = false;
	while (TRUE)
	{
		// exit when all windows have been destroyed
		if (GetToolWindowCount() == 0)
		{
			PostQuitMessage(0);
		}

		// dispatch queued messages
		MSG msg;
		while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage (&msg, NULL, 0, 0))
				goto loop_exit; // got WM_QUIT

			if (!TranslateAccelerator (MemView->hWnd, hAccel, &msg))
			{
				if (msg.hwnd != GetDlgItem(MemView->hWnd, IDC_MEMVIEWBOX))
				{
					if (IsDialogMessage(MemView->hWnd, &msg))
					{
						continue;
					}
				}

				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}

		// search spc player
		HWND SpcHost = Spc_GetHostWindow();
		spcHostAttached = (SpcHost != NULL);
		if (spcHostAttached != spcHostAttachedPrev)
		{
			if (spcHostAttached)
			{
				SetWindowText(MemView->hWnd, TEXT(TITLE_HOST_ATTACHED));
			}
			else
			{
				SetWindowText(MemView->hWnd, TEXT(TITLE_HOST_NOT_ATTACHED));
			}
		}
		spcHostAttachedPrev = spcHostAttached;

		if (spcHostAttached)
		{
			RefreshAllToolWindows();
		}

		Sleep(30);
	}

loop_exit:

	CloseAllToolWindows(); // must be done, though
	CloseHandle(spcmem_hMutex);

	return 0;
}

