#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <windows.h>
#include <Shlobj.h>
#include <filesystem>
#include "Engine/Engine/ChromeEngine.h"
#include "Engine/Filesystem/Filesystem.h"
#include "Engine/Engine/Interfaces/IGame.h"
#include "Engine/Filesystem/crashlog.h"
#include "Resources/resource.h"
//#include "MinHook/MinHook.h" will be used later for when I get a mod loader working

#define SPLASH_BITMAP IDB_SPLASH
#define SPLASH_TITLE  IDS_TITLE
#define SPLASH_ICON   IDI_ICON1

typedef HRESULT(WINAPI* SHGETFOLDERPATH)(HWND, int, HANDLE, DWORD, LPSTR);


LogPrintCallback g_original = nullptr;
void __cdecl LogCallback(const char* msg)
{
    printf("%s", msg);

    if (g_original)
        g_original(msg);
}


void InitConsole()
{
#ifdef  _DEBUG
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
#endif
}


SHGETFOLDERPATH LocateAndLoadGetFolderPath()
{
    HMODULE hModule;
    hModule = LoadLibraryA("shell32.dll");

    if (!hModule)
        return NULL;

    // Return the function pointer
    return (SHGETFOLDERPATH)GetProcAddress(hModule, "SHGetFolderPathA");
}


int WINAPI WinMain(HINSTANCE hWinInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    MSG stMsg = { 0 };

    //while (!::IsDebuggerPresent()) //just 4 me twin
    //::Sleep(100);
    InitConsole();

    char drive[_MAX_DRIVE];
    char dir[MAX_PATH];
    char szExeDir[MAX_PATH];

    GetModuleFileNameA(0, szExeDir, MAX_PATH);
    _splitpath(szExeDir, drive, dir, 0, 0);
    strcpy_s(szExeDir, MAX_PATH, drive);
    strcat_s(szExeDir, MAX_PATH, dir);

    char szCurrDir[MAX_PATH];
    if (strstr((PCSTR)lpCmdLine, "--wd")) {
        GetCurrentDirectoryA(MAX_PATH, szCurrDir);
        strcat_s(szCurrDir, MAX_PATH, "\\");
    }
    else
    {
    strcpy_s(szCurrDir, MAX_PATH, szExeDir);
    }

    const char* game_directory = "DI";

    char Data[MAX_PATH] = { 0 };
    int write_path_flags = 1;

    HICON gameIcon = (HICON)LoadImageA(hWinInstance, MAKEINTRESOURCEA(SPLASH_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
    ShowSplashscreen(hWinInstance, MAKEINTRESOURCEA(SPLASH_BITMAP), SPLASH_TITLE, gameIcon);

    DWORD dwData = MAX_PATH;
    bool bUseMyDocuments = false;

    static const char* REGISTRY_KEY = "SOFTWARE\\Techland\\Riptide";
    HKEY hKey;

    if (!RegOpenKeyExA(HKEY_LOCAL_MACHINE, REGISTRY_KEY, 0, KEY_READ, &hKey)) // == ERROR_SUCCESS)
    {
        LONG lResult = RegQueryValueExA(hKey, "WriteDir", NULL, NULL, (LPBYTE)Data, &dwData);
        RegCloseKey(hKey);
        bUseMyDocuments = (lResult == ERROR_SUCCESS);
    }

    char szPath[MAX_PATH];
    SHGETFOLDERPATH FolderPath = LocateAndLoadGetFolderPath();
    if (bUseMyDocuments && Data[0] && FolderPath != NULL &&
        FolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath) >= 0) {
        strcat_s(szPath, MAX_PATH, "\\");
        strcat_s(szPath, MAX_PATH, Data);
    }
    else
    {
        strcpy_s(szPath, MAX_PATH, szCurrDir);
        strcat_s(szPath, MAX_PATH, game_directory);
        strcat_s(szPath, MAX_PATH, "\\out");
        write_path_flags = 5;
    }


    fs::init(szPath, write_path_flags, "out/cache", false, false);
    LogSetPrintCallback(LogCallback);


    char name[MAX_PATH];
    char path[MAX_PATH];

    for (int i = 0; i < 64; ++i)
    {
        sprintf_s(name, MAX_PATH, "%s%s/data%d.pak", szCurrDir, game_directory, i);
        fs::add_source(name, FFSAddSourceFlags::SUBDIRS);
    }

    sprintf_s(name, MAX_PATH, "%s%s/dataDLC0.pak", szCurrDir, game_directory); //redo later with IGame::MountDlc
    fs::add_source(name, FFSAddSourceFlags::SUBDIRS);
    sprintf_s(name, MAX_PATH, "%s%s/dataDLC1.pak", szCurrDir, game_directory);
    fs::add_source(name, FFSAddSourceFlags::SUBDIRS);

    sprintf_s(name, MAX_PATH, "%s%s/dataen.pak", szCurrDir, game_directory);
    fs::add_source(name, FFSAddSourceFlags::SUBDIRS);

    strcpy_s(path, MAX_PATH, szCurrDir);
    strcat_s(path, MAX_PATH, game_directory);
    strcat_s(path, MAX_PATH, "/Data");
    fs::add_source(
        path, FFSAddSourceFlags::SUBDIRS | FFSAddSourceFlags::APPEND | FFSAddSourceFlags::STRIP_LAST_DIR);

    strcpy_s(path, MAX_PATH, szCurrDir);
    strcat_s(path, MAX_PATH, game_directory);
    strcat_s(path, MAX_PATH, "_temp/Data");
    fs::add_source(path, FFSAddSourceFlags::SUBDIRS | FFSAddSourceFlags::STRIP_LAST_DIR);


    strcpy_s(path, MAX_PATH, szCurrDir);
    strcat_s(path, MAX_PATH, "Engine/Data");
    fs::add_source(
        path, FFSAddSourceFlags::SUBDIRS | FFSAddSourceFlags::APPEND | FFSAddSourceFlags::STRIP_LAST_DIR);

    for (int j = 0; j < 64; ++j)
    {
        sprintf_s(name, MAX_PATH, "%s%s/data%d.mpak", szCurrDir, game_directory, j);
        fs::add_source(name, FFSAddSourceFlags::SUBDIRS | FFSAddSourceFlags::APPEND | FFSAddSourceFlags::PRELOAD);
    }

    char szClass[MAX_PATH];
    char szGameScriptDLL[MAX_PATH];
    strcpy_s(szClass, MAX_PATH, "Game");
    strcpy_s(szGameScriptDLL, MAX_PATH, szExeDir);
    bool bFastDIAInit = false;

    strcat_s(szGameScriptDLL, MAX_PATH, "gamedll"); //this loads the game DLL.
    InitializeGameScript(hWinInstance, szGameScriptDLL, bFastDIAInit);

    strcpy_s(szClass, MAX_PATH, "GameDI");
    IGame* Game = CreateGame(szClass, hWinInstance, true, (char*)game_directory);
    HideSplashscreen();


    if (Game->Initialize(lpCmdLine, nCmdShow, (HICON)stMsg.message, 0, 0))
        ExitProcess(1);

    ShowCursor(FALSE);
    while (true) {
        while (!PeekMessageA(&stMsg, 0, 0, 0, 1u))
            Game->OnPaint();
        Game->WaitForPipelineThreads();

        if (stMsg.message == WM_QUIT)
            break;

        TranslateMessage(&stMsg);
        DispatchMessageA(&stMsg);
    }

    return 0;
}