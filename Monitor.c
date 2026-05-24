//C-Task CLI System Monitor only For WindowsOS,it Will Display The Current 
//Memory,Central Processes Units, Disk Usage, And Hardware Specifications of The System
//Mainly Used Windows API and Librabries To Get The Required Information, And It Will Update Every 3 Seconds To Provide Real-Time Monitoring Of The System's Performance And Resources.
#include <stdio.h>
#include <stdlib.h>
#include <synchapi.h>
#include <windows.h>
#include <psapi.h>
#include <minwindef.h>
#include <tlhelp32.h>
#define MAX_BUFFER 1024 

void GetMemoryUsage(){
    PROCESS_MEMORY_COUNTERS pmc;
    if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))){
        printf("Current Memory Usage: %lu bytes\n", pmc.WorkingSetSize);
    } else {
        printf("Failed to get memory usage.\n");
    }
}
void GetProcessUsage(){
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 );
    if(hProcessSnap == INVALID_HANDLE_VALUE){
        printf("Failed To Take Snapshot of Processes. \n");
        return;
    }
    else{
        if(Process32First(hProcessSnap, &pe32)){
            do{
                printf("Process Name: %s | Process ID: %lu\n", pe32.szExeFile, pe32.th32ProcessID);
            } while(Process32Next(hProcessSnap, &pe32));
        }
        else{
            printf("Failed To Get First Process. \n");
        }
    }
    CloseHandle(hProcessSnap);
}
void StorageUsage(){
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    if(GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)){
        printf("Total Storage: %llu bytes\n", totalNumberOfBytes.QuadPart);
        printf("Free Storage: %llu bytes\n", totalNumberOfFreeBytes.QuadPart);
    } else {
        printf("Failed To Get Storage Usage.\n");
    }    
}
typedef struct spec{
    char cpu[50];
    char gpu[50];
    char ram[50];
    char storage[50];
}Specifications;

void GetHardwareSpecification(Specifications* spec){
    HKEY hKey;
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
        DWORD bufferSize = MAX_BUFFER;
        RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)spec->cpu, &bufferSize);
        RegCloseKey(hKey);
    } else {
        printf("Failed To Get CPU Information.\n");
    }
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\VideoPorts\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
        DWORD bufferSize = MAX_BUFFER;
        RegQueryValueExA(hKey, "VideoProcessor", NULL, NULL, (LPBYTE)spec->gpu, &bufferSize);
        RegCloseKey(hKey);
    } else {
        printf("Failed To Get GPU Information.\n");
    }
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", 0, KEY_READ, &hKey) == ERROR_SUCCESS){
        DWORD bufferSize =MAX_BUFFER;
        RegQueryValueExA(hKey, "SystemMemorySize", NULL, NULL,(LPBYTE)spec->ram, &bufferSize);
        RegCloseKey(hKey);
    } else {
        printf("Failed To Get RAM Information.\n");
    }
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",0, KEY_READ, &hKey) == ERROR_SUCCESS){
        DWORD bufferSize = MAX_BUFFER;
        RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)spec->storage, &bufferSize);
        RegCloseKey(hKey);
    } else{
        printf("Failed To Get Disk Information.\n");
    }
}


int main(){
    Specifications spec;
    while(1){
        printf("===== System Monitor =====\n");
        printf("PROCESSES USAGE: ");
        GetProcessUsage();
        printf("\nMEMORY USAGE: ");
        GetMemoryUsage();
        printf("\nSTORAGE USAGE: ");
        StorageUsage();
        printf("\nHARDWARE SPECIFICATIONS: ");
        GetHardwareSpecification(&spec);
        Sleep(3000); // Update every 3 seconds
        system("cls");
    }
}