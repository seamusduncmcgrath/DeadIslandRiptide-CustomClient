#pragma once

#define ENGINE_API __declspec(dllexport)
class IGame;

//GetEngineVersion

extern "C" ENGINE_API int GetEngineDllVersion();
#pragma comment(linker, "/alternatename:_GetEngineDllVersion=GetEngineDllVersion")
extern "C" ENGINE_API void InitializeGameScript(HINSTANCE hInstance, const char* gameScriptDll, bool fastDIAInit);
#pragma comment(linker, "/alternatename:_InitializeGameScript=InitializeGameScript")
extern "C" ENGINE_API void UninitializeGameScript();
#pragma comment(linker, "/alternatename:_UninitializeGameScript=UninitializeGameScript")
extern "C" ENGINE_API IGame* CreateGame(char* pszClassName, HINSTANCE hInstance, bool bPlaceHolder, char* gameDirName);
#pragma comment(linker, "/alternatename:_CreateGame=CreateGame")
extern "C" ENGINE_API void ShowSplashscreen(HINSTANCE hInstance, LPCSTR bitmap_res, UINT app_title_res, HICON icon);
#pragma comment(linker, "/alternatename:_ShowSplashscreen=ShowSplashscreen")
extern "C" ENGINE_API void HideSplashscreen();
#pragma comment(linker, "/alternatename:_HideSplashscreen=HideSplashscreen")
extern "C" ENGINE_API void DestroyGame();
#pragma comment(linker, "/alternatename:_DestroyGame=DestroyGame")
