#include <Resolve/Resolve.h>

#include <TlHelp32.h>

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

#include <iostream>

static HANDLE FindProcess(char* _Name) {

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (snapshot == INVALID_HANDLE_VALUE)
        return nullptr;
    
    PROCESSENTRY32 entry = { 0 };

    entry.dwSize = sizeof(entry);

    for (BOOL status = Process32First(snapshot, &entry); status == TRUE; status = Process32Next(snapshot, &entry)) {

        if (!_strcmpi(_Name, entry.szExeFile)) {

            CloseHandle(snapshot);

            return OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

        }

    }

    CloseHandle(snapshot);

    return nullptr;

}

int main(int argc, char** argv) {

    if (argc != 3)
        return -1;
    
    HANDLE process = FindProcess(argv[1]);

    if (!process)
        return -2;
    
    uintptr_t result;

    if (Resolve(process, argv[2], result)) {

        std::cout << (void*)result << std::endl;

        return 0;

    }
    else {

        std::cout << "Error." << std::endl;

        return -3;

    }

}