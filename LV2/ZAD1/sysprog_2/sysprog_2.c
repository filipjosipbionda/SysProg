#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[]) {
    if (argc < 2) {
        _ftprintf(stderr, _T("Usage: %s <directory>\n"), argv[0]);
        return 1;
    }

    // Save the initial directory path
    TCHAR directoryPath[MAX_PATH];
    _tcscpy(directoryPath, argv[1]);

    // Append a wildcard (*) for searching
    _tcscat(directoryPath, _T("\\*"));

    WIN32_FIND_DATA findData;
    HANDLE findHandle = FindFirstFile(directoryPath, &findData);

    if (findHandle == INVALID_HANDLE_VALUE) {
        _ftprintf(stderr, _T("Unable to open directory: %s\n"), argv[1]);
        return 1;
    }

    int fileCount = 0;
    int directoryCount = 0;

    do {
        // Skip the current and parent directory entries
        if (_tcscmp(findData.cFileName, _T(".")) == 0 || _tcscmp(findData.cFileName, _T("..")) == 0) {
            continue;
        }

        // Check if the item is a directory or a file
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            directoryCount++;
        }
        else {
            fileCount++;
        }
    } while (FindNextFile(findHandle, &findData) != 0);

    // Close the handle
    FindClose(findHandle);

    // Print the results
    _tprintf(_T("Directory: %s\n"), argv[1]);
    _tprintf(_T("Number of directories: %d\n"), directoryCount);
    _tprintf(_T("Number of files: %d\n"), fileCount);

    return 0;
}
