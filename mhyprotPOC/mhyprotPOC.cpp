#include <iostream>
#include "HID.h"
#include "ApexLegends.h"
#include "Threads.h"
#include "Utilities.h"
#include <Windows.h>
#include <chrono>
#include <Dwmapi.h> 
#include "Overlay.h"
#include <fstream>
#include "XorStr.h"
#pragma comment(lib, "dwmapi.lib")



BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_CLOSE_EVENT)
    {
        Driver::StopAndDeleteAllDrivers();
        return true;
    }
    return false;
}

const MARGINS margins = { -1 ,-1, -1, -1 };
const wchar_t g_szClassName[] = L"Luneware";
WNDCLASS wc = { };
static bool AlreadyInGame = true;
static HWND OverlayHWND;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT,
        g_szClassName,
        g_szClassName,
        WS_POPUP | WS_VISIBLE,
        0, 0, 1920, 1080,
        NULL, NULL, hInstance, NULL);
    OverlayHWND = hwnd;
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
void WindowOverlay() 
{
    WinMain((HINSTANCE)GetModuleHandle(NULL), NULL, NULL, 0);
}
void Pause(const char* message)
{
    std::cout << message;
    HANDLE hStdin;
    DWORD cNumRead;
    INPUT_RECORD irInBuf[1];
    if (HANDLE(hStdin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return;
    while (true)
    {
        if (!ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead))
            return;
        for (DWORD i = 0; i < cNumRead; ++i)
            if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
            {
                std::cout << '\n';
                return;
            }
    }
}

void InitializeApexLegends()
{
    printf("------------------------------------------------------------\n");
    printf("               Jaguar Software - Apex Legends               \n");
    printf("------------------------------------------------------------\n");
    printf("[?] Initializing exploit\n");
    static bool bActive = true;
    if (mhyprotrose::MhyDebugInitialize())
    {
        printf("   [+] Exploit initialized\n\n");

        printf("[?] Searching for target process \n");
        while (true)
        {
            DWORD dwApexPid = Utilities::GetProcessIdByName(APEXVars.wcProcessName);
            if (dwApexPid)
            {
                APEXVars.dwProcessId = dwApexPid;
                APEXVars.pBaseAddress = Utilities::GetProcessBaseAddress(APEXVars.dwProcessId);
                if (APEXVars.pBaseAddress != 0)

                printf(("   [+] Target process found\n"));
                printf(("       - PID : %d\n"), dwApexPid);
                printf(("       - Module dependencies : \n"));
                printf(("          - Base Address : 0x%X\n"), APEXVars.pBaseAddress);
                
                break;
            }
            AlreadyInGame = false;
            Sleep(5000);
        }
        if (!AlreadyInGame) {
            printf(("[?] Starting in 10 Seconds\n"));
            Sleep(10000);
        }
        else
            printf(("[?] Starting threads\n"));
        StartThreads:
        CFG.bActive = true;
        if (Configuration::FileExists(CFG.chConfigFile)) 
        {
            if (Configuration::ShouldAutoInitFromFile())
            {
                printf("   [+] Reading Variables from Config\n");
                Configuration::UpdateConfigFromFile();
            }
        }
        else
        {
            printf("Creating New Config File\n");
            Configuration::WriteToConfigFile();
        }  
  
        ThreadVars.thApexLegendsThread = thread(Threads::ApexLegendsAimThread);
        ThreadVars.ItemGlowthread = thread(Threads::Itemglow);
        ThreadVars.tempPlayerGlow = thread(Threads::Glow);
        thread overlay = thread(WindowOverlay);
      //  overlay.detach();
      //  ThreadVars.tempPlayerGlow.detach();
      //  ThreadVars.ItemGlowthread.detach();
        Sleep(300);
        printf("   [+] Threads started\n");
        printf("---------------------------------------------------------\n");
        printf("   [~] Feel free to report any bugs to the developer! \n");
        Overlay ovWindow = Overlay();
        ovWindow.Start(OverlayHWND);
        ThreadVars.thApexLegendsThread.join();
    }
    else
    {
        Driver::StopAndDeleteAllDrivers();
        printf("   [-] Unable to initialize exploit\n"));
        Sleep(5000);
        return;
    }
    printf("Not good\n");
    if(CFG.bActive)
    {
        printf("Caught An Error!!! Restarting threads in 1 second");
        Sleep(600);
        goto StartThreads;
    }
    mhyprotrose::MhyDebugUnload();
}

int main(HINSTANCE hINST)
{
    SetConsoleTitleW((LPCWSTR)L"Lunariel Framework - Jaguar Software");
    SetConsoleCtrlHandler(ConsoleHandlerRoutine, true);
    Driver::StopAndDeleteAllDrivers();
    if (Utilities::UpdateEPROCESSOffsets())
    {


        if (ShouldOverwriteCFG())
        {
            Configuration::WriteToConfigFile();
        }

        system("cls");
        InitializeApexLegends();

    }
    else
    {
        OSVERSIONINFOEXW osVersionInfo = Utilities::GetOSVersionInfo();
        string strOSVersionInfo = to_string(osVersionInfo.dwMajorVersion) + "." + to_string(osVersionInfo.dwMinorVersion) + " - " + to_string(osVersionInfo.dwBuildNumber);
        string strMessage = "Your windows [" + strOSVersionInfo + "] is unsupported, please contact administrator";
        MessageBoxA(NULL, strMessage.c_str(), "[pblLuneware] OS Unsupported", MB_OK);
    }
    return 0;
}    