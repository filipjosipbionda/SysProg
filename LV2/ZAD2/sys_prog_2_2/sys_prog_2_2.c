#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#define PATH_BUFFER_SIZE 1024
#define LARGE_FILE_SIZE (10 * 1024 * 1024)

int largeFilesCount = 0;
FILETIME oldestFileTime;

void InitOldestFileTime() {
    oldestFileTime.dwLowDateTime = 0xFFFFFFFF;
    oldestFileTime.dwHighDateTime = 0xFFFFFFFF;
}

void PreorderTraversal(LPTSTR path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    TCHAR searchPath[PATH_BUFFER_SIZE];

    _stprintf(searchPath, _T("%s\\*"), path);
    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Cannot access directory: %s\n"), path);
        return;
    }

    _tprintf(_T("Directory: %s\n"), path);

    do {
        if (_tcscmp(findFileData.cFileName, _T(".")) == 0 || _tcscmp(findFileData.cFileName, _T("..")) == 0) {
            continue;
        }

        TCHAR fullPath[PATH_BUFFER_SIZE];
        _stprintf(fullPath, _T("%s\\%s"), path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            PreorderTraversal(fullPath);
        }
        else {
            _tprintf(_T("File: %s\n"), fullPath);

            LARGE_INTEGER fileSize;
            fileSize.LowPart = findFileData.nFileSizeLow;
            fileSize.HighPart = findFileData.nFileSizeHigh;
            if (fileSize.QuadPart > LARGE_FILE_SIZE) {
                largeFilesCount++;
            }

            if (CompareFileTime(&findFileData.ftCreationTime, &oldestFileTime) < 0) {
                oldestFileTime = findFileData.ftCreationTime;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int _tmain(int argc, TCHAR* argv[]) {
    if (argc < 2) {
        _tprintf(_T("Usage: %s <directory_path>\n"), argv[0]);
        return 1;
    }

    InitOldestFileTime();
    PreorderTraversal(argv[1]);

    SYSTEMTIME oldestFileSystemTime;
    FileTimeToSystemTime(&oldestFileTime, &oldestFileSystemTime);

    _tprintf(_T("\nNumber of large files: %d\n"), largeFilesCount);
    _tprintf(_T("Oldest file date: %04d-%02d-%02d\n"),
        oldestFileSystemTime.wYear, oldestFileSystemTime.wMonth, oldestFileSystemTime.wDay);

    return 0;
}
