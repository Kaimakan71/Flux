/*
 * File utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <flux/file.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <errno.h>
#include <stdarg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
logFileError(const char *format, ...)
{
    va_list argumentList;

    fputs("\033[1;91m[error]\033[0m ", stderr);
    va_start(argumentList, format);
    vfprintf(stderr, format, argumentList);
    va_end(argumentList);
    fputc('\n', stderr);
}

char *
fluxFileLoadText(const char *path)
{
#if defined(_WIN32)
    HANDLE hFile;
    DWORD dwFileSize, dwBytesRead;
    PCHAR pBuffer;

    hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        logFileError("failed to open file \"%s\": 0x%08x", path, GetLastError());
        return NULL;
    }

    dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE) {
        logFileError("failed to get the size of file \"%s\": 0x%08x", path, GetLastError());
        CloseHandle(hFile);
        return NULL;
    }

    /* Allocate enough space for file data and NULL terminator */
    pBuffer = malloc((size_t)dwFileSize + sizeof('\0'));
    if (pBuffer == NULL) {
        logFileError("failed to allocate buffer for file \"%s\"", path);
        CloseHandle(hFile);
        return NULL;
    }

    if (!ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL)) {
        logFileError("failed to read file \"%s\": 0x%08x", path, GetLastError());
        free(pBuffer);
        CloseHandle(hFile);
        return NULL;
    }

    if (dwBytesRead < dwFileSize) {
        logFileError("failed to read entirety of file \"%s\"", path);
        free(pBuffer);
        CloseHandle(hFile);
        return NULL;
    }

    /* Terminate the buffer */
    pBuffer[dwFileSize] = '\0';

    CloseHandle(hFile);
    return pBuffer;
#else
    FILE *file;
    long size;
    char *buffer;

    file = fopen(path, "r");
    if (file == NULL) {
        logFileError("failed to open file \"%s\": %s", path, strerror(errno));
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        logFileError("failed to seek to the end of file \"%s\": %s", path, strerror(errno));
        fclose(file);
        return NULL;
    }

    /* Get the position to determine the file's size */
    size = ftell(file);
    if (size < 0) {
        logFileError("failed to get the size of file \"%s\": %s", path, strerror(errno));
        fclose(file);
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        logFileError("failed to rewind to the beginning of file \"%s\": %s", path, strerror(errno));
        fclose(file);
        return NULL;
    }

    /* Allocate enough space for file data and NULL terminator */
    buffer = malloc(size + sizeof('\0'));
    if (buffer == NULL) {
        logFileError("failed to allocate buffer for file \"%s\"", path);
        fclose(file);
        return NULL;
    }

    if (fread(buffer, sizeof(char), (size_t)size, file) < (size_t)size) {
        logFileError("failed to read entirety of file \"%s\"", path);
        free(buffer);
        fclose(file);
        return NULL;
    }

    /* Terminate the buffer */
    buffer[size] = '\0';

    fclose(file);
    return buffer;
#endif
}
