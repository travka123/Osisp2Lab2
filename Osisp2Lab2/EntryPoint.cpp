#include <Windows.h>
#include "Table.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ INT nCmdShow) {

    Table::SetDataFromCSV("values.csv");

    const wchar_t className[] = L"MyWindowClass";

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className;
    wcex.hIconSm = wcex.hIcon;

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(className, L"Hello, World!",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void Paint(HWND hWnd) {
    PAINTSTRUCT ps;
    BeginPaint(hWnd, &ps);

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    HDC hBufferDC = CreateCompatibleDC(ps.hdc);
    HBITMAP hBufferBitmap = CreateCompatibleBitmap(ps.hdc, clientRect.right, clientRect.bottom);
    HGDIOBJ hOldBufferBitmap = SelectObject(hBufferDC, hBufferBitmap);

    FillRect(hBufferDC, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

    Table::Draw(hBufferDC, clientRect);

    BitBlt(ps.hdc, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);

    SelectObject(hBufferDC, hOldBufferBitmap);
    DeleteObject(hBufferDC);
    DeleteObject(hBufferBitmap);

    EndPaint(hWnd, &ps);
}

void Size(HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    InvalidateRect(hWnd, &clientRect, false);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        Paint(hWnd);
        break;

    case WM_SIZE:
        Size(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}