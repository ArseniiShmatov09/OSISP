﻿#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Глобальные переменные
HWND hEdit;
TCHAR szFileName[MAX_PATH] = L"";

// Прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OpenFile(HWND);
void SaveFile(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"TextEditor", NULL };
    RegisterClassEx(&wcex);

    // Создание окна
    HWND hWnd = CreateWindow(L"TextEditor", L"Простой текстовый редактор", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, L"Не удалось создать окно", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Создание элемента управления для редактирования текста
    hEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 10, 10, 764, 500, hWnd, NULL, hInstance, NULL);

    if (!hEdit) {
        MessageBox(NULL, L"Не удалось создать элемент управления для редактирования текста", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Создание кнопки "Открыть"
    CreateWindow(L"BUTTON", L"Открыть", WS_CHILD | WS_VISIBLE, 10, 520, 100, 30, hWnd, (HMENU)1, hInstance, NULL);

    // Создание кнопки "Сохранить"
    CreateWindow(L"BUTTON", L"Сохранить", WS_CHILD | WS_VISIBLE, 120, 520, 100, 30, hWnd, (HMENU)2, hInstance, NULL);

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // Нажата кнопка "Открыть"
            OpenFile(hWnd);
            break;
        case 2: // Нажата кнопка "Сохранить"
            SaveFile(hWnd);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OpenFile(HWND hWnd) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFileName);
    ofn.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        std::wifstream file(szFileName);
        if (file) {
            std::wstringstream buffer;
            buffer << file.rdbuf();
            SetWindowText(hEdit, buffer.str().c_str());
        }
    }
}

void SaveFile(HWND hWnd) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFileName);
    ofn.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn)) {
        std::wofstream file(szFileName);
        if (file) {
            int length = GetWindowTextLength(hEdit);
            if (length > 0) {
                std::vector<wchar_t> buffer(length + 1);
                GetWindowText(hEdit, buffer.data(), length + 1);
                file << buffer.data();
                MessageBox(NULL, L"Файл успешно сохранен!", L"Успех", MB_ICONINFORMATION);
            }
            else {
                MessageBox(NULL, L"Нельзя сохранить пустой файл.", L"Ошибка", MB_ICONERROR);
            }
        }
        else {
            MessageBox(NULL, L"Не удалось создать файл для сохранения.", L"Ошибка", MB_ICONERROR);
        }
    }
}