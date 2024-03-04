#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>
using namespace std;

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
DWORD WINAPI Write(LPVOID lp);
DWORD WINAPI Read(LPVOID lp);
DWORD WINAPI WriteEditBox(LPVOID lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

DWORD WINAPI Write(LPVOID lp) {
    HWND hWnd = HWND(lp);
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
    if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
    {
        MessageBox(0, L"Write", L"ALT+F4", MB_OK);
        wofstream myfile("Test.txt", ios::out);
        myfile << "Aslan Gadzhyyev\n";
        myfile.close();
    }
    ResetEvent(hEvent);
    return 0;
}

DWORD WINAPI Read(LPVOID lp) {
    HWND hWnd = HWND(lp);
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
    TCHAR buff[100];
    if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) {
        wifstream myfile("Test.txt", ios::in);
        wstring res;
        while (!myfile.eof()) {
            myfile >> buff;
            res += buff;
            res += L" ";
        }
        myfile.close();
        MessageBox(0, res.c_str(), L"ALT+F4", MB_OK);
    }
    ResetEvent(hEvent);
    return 0;
}

DWORD WINAPI WriteEditBox(LPVOID lp)
{
    HWND hWnd = HWND(lp);
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
    TCHAR buff[100];
    if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) {
        wifstream myfile("Test.txt", ios::in);
        wstring res;
        while (!myfile.eof()) {
            myfile.getline(buff, 100);
            res += buff;
            res += L"\r\n";
        }
        myfile.close();

        HWND hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
        SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)res.c_str());
        SYSTEMTIME systime;
        GetLocalTime(&systime);
        wchar_t datetime[100];
        swprintf(datetime, 100, L"%04d-%02d-%02d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
        HWND hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
        SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)datetime);
    }
    ResetEvent(hEvent);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
    switch (mes)
    {
    case WM_INITDIALOG:
    {
        CreateEvent(NULL, TRUE, FALSE, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
        HANDLE h = CreateThread(NULL, 0, Write, 0, 0, NULL);
        CloseHandle(h);
        h = CreateThread(NULL, 0, Read, 0, 0, NULL);
        CloseHandle(h);
        h = CreateThread(NULL, 0, WriteEditBox, hWnd, 0, NULL);
        CloseHandle(h);
    }
    break;
    case WM_COMMAND:
    {
        HANDLE h = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
        SetEvent(h);
    }
    break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    }
    return FALSE;
}
