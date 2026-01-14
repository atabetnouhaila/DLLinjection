# DLL Injection Tool

## Description

This project implements a DLL injection tool using the **CreateRemoteThread** technique on Windows. DLL injection is a process manipulation technique that allows loading a dynamic-link library (DLL) into a running process's address space. This tool demonstrates how to inject a DLL into any target process by leveraging Windows API functions for process manipulation, memory allocation, and thread creation.

The implementation uses a classic DLL injection method that:
1. Opens a handle to the target process
2. Allocates memory within the target process
3. Writes the DLL path to the allocated memory
4. Creates a remote thread that executes `LoadLibraryW` to load the DLL

## Purpose

The primary purpose of this project is to:
- **Educational**: Understand Windows process manipulation and DLL injection techniques
- **Security Research**: Study how malware and security tools interact with processes
- **Learning**: Gain hands-on experience with Windows API functions for process management
- **Defensive Security**: Understand injection techniques to better detect and prevent them

**⚠️ Important Note**: This tool is intended for educational and authorized security testing purposes only. Unauthorized use of DLL injection techniques may violate laws and system policies.

## What Built

This project consists of a C++ console application (`DLLinjec.exe`) that implements DLL injection using the following Windows API functions:

### Core Components:
- **Process Handle Acquisition**: Uses `OpenProcess()` to obtain a handle to the target process
- **Memory Allocation**: Uses `VirtualAllocEx()` to allocate memory in the remote process with read/write permissions
- **Memory Writing**: Uses `WriteProcessMemory()` to write the DLL path into the allocated memory
- **Function Address Resolution**: Uses `GetModuleHandleW()` and `GetProcAddress()` to get the address of `LoadLibraryW` from `kernel32.dll`
- **Remote Thread Creation**: Uses `CreateRemoteThread()` to create a thread in the target process that executes `LoadLibraryW` with the DLL path as an argument
- **Thread Synchronization**: Uses `WaitForSingleObject()` to wait for the remote thread to complete execution

### Features:
- Command-line interface that accepts a Process ID (PID) as an argument
- Comprehensive error handling and logging with prefix-based messages (`[+]` for success, `[*]` for info, `[-]` for errors)
- Proper resource cleanup (closes handles after execution)
- Support for both x86 and x64 architectures

### Project Structure:
```
DLLinjection/
├── DLLinjec/
│   ├── DLLinjec.cpp          # Main source code
│   ├── DLLinjec.vcxproj      # Visual Studio project file
│   └── DLLinjec.vcxproj.filters  # Visual Studio file organization
├── DLLinjec.sln              # Visual Studio solution file
└── README.md                 # Project documentation
```

**Note**: Build artifacts (executables, object files, logs) are generated in `x64/Debug/` or `x64/Release/` directories when you build the project. These directories are not included in the source repository as they are regenerated during compilation.

## How to Run/Test

### Prerequisites:
- **Windows Operating System** (Windows 10 or later)
- **Visual Studio 2019 or later** with C++ development tools
- **Administrator privileges** (required for process manipulation)
- A target DLL file to inject

### Building the Project:

1. **Open the Solution**:
   - Open `DLLinjec.sln` in Visual Studio

2. **Select Build Configuration**:
   - Choose either `Debug` or `Release` configuration
   - Select `x64` or `x86` platform (match your target process architecture)

3. **Build the Project**:
   - Press `Ctrl+Shift+B` or go to `Build > Build Solution`
   - The executable will be generated in:
     - `x64/Debug/DLLinjec.exe` for Debug builds
     - `x64/Release/DLLinjec.exe` for Release builds
     - Or `DLLinjec/x64/Debug/DLLinjec.exe` depending on your Visual Studio configuration

### Configuration:

**⚠️ Important**: Before running, you must update the DLL path in the source code:

1. Open `DLLinjec/DLLinjec.cpp`
2. Locate line 13:
   ```cpp
   wchar_t dllPath[MAX_PATH] = L"Path to DLLinjection.dll";
   ```
3. Replace the path with your actual DLL path (use double backslashes `\\` for Windows paths)

### Running the Tool:

1. **Find a Target Process ID**:
   - Open Task Manager (`Ctrl+Shift+Esc`)
   - Go to the "Details" tab
   - Note the PID of the target process
   - Alternatively, use PowerShell:
     ```powershell
     Get-Process | Select-Object Id, ProcessName
     ```

2. **Run the Injector** (as Administrator):
   ```cmd
   DLLinjec.exe <Process_ID>
   ```
   
   Example:
   ```cmd
   DLLinjec.exe 1234
   ```

3. **Expected Output**:
   ```
   [*] Info: Targeting process with ID: 1234
   [+] Success: Successfully got a handle to the process (0x0000012345678900)
   [+] Success: Allocated buffer to process memory with RW permissions
   [+] Success: wrote [C:\path\to\your\DLLinjection.dll] to process memory
   [+] Success: Successfully got a handle to the kernel32.dll (0x00007FF123456789)
   [+] Success: Successfully got the address of LoadLibraryW() (0x00007FF123456789)
   [+] Success: Remote thread created successfully with ID: 5678
   [*] Info: Waiting for the remote thread to finish execution...
   [+] Success: Remote thread finished executing
   ```

### Testing:

1. **Test with a Simple Target**:
   - Use `notepad.exe` as a test target
   - Start Notepad
   - Get its PID from Task Manager
   - Run the injector with that PID

2. **Verify Injection**:
   - Use Process Explorer or Process Hacker to verify the DLL is loaded
   - Check the "Modules" tab in Process Explorer for your DLL
   - Use PowerShell to list loaded modules:
     ```powershell
     Get-Process -Id <PID> | Select-Object -ExpandProperty Modules | Select-Object ModuleName, FileName
     ```

3. **Common Issues**:
   - **Access Denied**: Run as Administrator
   - **Process Not Found**: Verify the PID is correct
   - **Architecture Mismatch**: Ensure injector and target process are same architecture (x64/x86)
   - **DLL Not Found**: Verify the DLL path is correct and accessible

## What I Learned

Through building this DLL injection tool, I gained valuable knowledge in several areas:

### Windows API Mastery:
- **Process Management**: Learned how to interact with processes using `OpenProcess()`, understanding process handles and access rights
- **Memory Management**: Gained experience with `VirtualAllocEx()` and `WriteProcessMemory()` for cross-process memory operations
- **Thread Manipulation**: Understood how `CreateRemoteThread()` works and how to execute code in a remote process context
- **Module and Function Resolution**: Learned how to use `GetModuleHandleW()` and `GetProcAddress()` to resolve function addresses dynamically

### Security Concepts:
- **Process Injection Techniques**: Understood the mechanics of DLL injection and how malware uses similar techniques
- **Defensive Measures**: Learned why modern security tools (EDR, antivirus) detect and prevent these techniques
- **Privilege Escalation**: Recognized the importance of proper access rights and administrator privileges

### Programming Skills:
- **Error Handling**: Implemented comprehensive error checking using `GetLastError()` and proper return codes
- **Resource Management**: Learned the importance of closing handles to prevent resource leaks
- **Unicode Handling**: Worked with wide character strings (`wchar_t`) for Windows API compatibility
- **Command-Line Arguments**: Implemented argument parsing and user-friendly error messages

### System Architecture Understanding:
- **Process Address Spaces**: Understood how processes have isolated memory spaces and how injection bridges this isolation
- **DLL Loading Mechanism**: Learned how `LoadLibraryW` works and why it's a common target for injection
- **Thread Execution Context**: Understood how threads execute in the context of their parent process

### Practical Insights:
- **Debugging Techniques**: Learned to use Process Monitor, Process Explorer, and debuggers to verify injection
- **Architecture Compatibility**: Recognized the importance of matching x86/x64 architectures between injector and target
- **Security Implications**: Gained awareness of how these techniques are used in both offensive and defensive security contexts

This project provided hands-on experience with low-level Windows system programming and deepened my understanding of process manipulation techniques used in both security research and malware analysis.

