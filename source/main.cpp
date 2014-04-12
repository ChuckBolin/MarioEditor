/**************************************************************************************
Filename: main.cpp Date: December 17, 2008 (original)
Purpose: Entry point and initial setup for game
Updated: gc10_editor.exe

**************************************************************************************/

// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <string>
#include "../include/CLog.h"
#include "../include/CGraphics.h"
#include "../include/CTimer.h"
#include "../include/CGameData.h"
#include "../include/Keystatus.h"
#include "../include/camera.h"
#include "../include/CAudioManager.h"
#include "..\include\cGameStateObject.h"
#include "..\include\cGameStateControl.h"
#include "..\include\cGameStateIntro.h"
#include "..\include\cGameStateMain.h"
#include "..\include\cGameStateHelp.h"
#include "..\include\cGameStateCredits.h"
#include "..\include\cGameStateQuit.h"
#include "..\include\CINIReader.h"
#include "..\include\resource.h"

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  1024// 800//640
#define SCREEN_HEIGHT 768//600//480

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// global declarations
LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class

// function prototypes
void Shutdown();

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//global variables
//create game states
cGameStateObject* g_pStateControl = new cGameStateControl;
cGameStateObject* g_pStateMain = new cGameStateMain;
cGameStateObject* g_pStateIntro = new cGameStateIntro;
cGameStateObject* g_pStateCredits = new cGameStateCredits;
cGameStateObject* g_pStateHelp = new cGameStateHelp;
cGameStateObject* g_pCurrent = 0;
cGameStateObject* g_pNext = 0;
cGameStateObject* g_pStateQuit = new cGameStateQuit;

//frame counting
CTimer gTimerFPS;
int gLoopCount = 0;
int gSecondCount = 0;
int gFPS = 0;
bool g_bRunning = true;
bool gExitProgram = false; //this is set true with ESC, for rendering to stop properly

CMouse mouse;
CTimer gTimerKey;
CTimer gAudioTimer;

/***********************************************************************************
WinMain - Entry point for program.
***********************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    CGameData gameData;
    gameData.m_version = "gc10_editor.exe v0.016";  

    CLog *pLog = CLog::Instance();
    pLog->Log(" ");
    pLog->Log("***************************************");
    pLog->Log("Program Start", gameData.m_version);   
    pLog->LogDate();   
    pLog->Log("***************************************");

    //setup game data    
    gameData.LoadConfigFile("assets//data//config.txt");

    //create window
    HWND hWnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";
    wc.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON2)); 

    RegisterClassEx(&wc);
    pLog->Log("Function: RegisterClassEx");
    
    //screen data - need at least 800x600 
    int cxScreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
    int cyScreen = ::GetSystemMetrics(SM_CYFULLSCREEN);
    if(gameData.m_screenWidth < 800 || gameData.m_screenWidth > cxScreen)
      gameData.m_screenWidth = cxScreen;   
    if(gameData.m_screenHeight < 600 || gameData.m_screenHeight > cyScreen)
      gameData.m_screenHeight = cyScreen;    
    gameData.m_screenLeft = cxScreen/2 - gameData.m_screenWidth/2;
    gameData.m_screenTop = cyScreen/2 - gameData.m_screenHeight/2;

    //create window
    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          gameData.m_version.c_str(), 
                          gameData.m_bFullScreen ? WS_EX_TOPMOST | WS_POPUP : WS_BORDER | WS_CAPTION | WS_SYSMENU,  
                          gameData.m_screenLeft, gameData.m_screenTop, 
                          gameData.m_bFullScreen ? CW_USEDEFAULT : gameData.m_screenWidth,
                          gameData.m_bFullScreen ? CW_USEDEFAULT : gameData.m_screenHeight,
                          NULL,NULL,hInstance,NULL);

    ShowWindow(hWnd, nCmdShow);
    pLog->Log("Function: ShowWindow");
    gameData.m_hWnd = hWnd;

    // set up and initialize Direct3D
    CGraphics con(hWnd, gameData.m_screenWidth, gameData.m_screenHeight, gameData.m_bFullScreen);
    if(con.InitializeDirectX() == false){
      pLog->Log("DirectX Failure!");
      ::MessageBox(hWnd,"Failed to initialize DirectX", "Error", 0);
      return 0;
    }
    pLog->Log("DirectX Initialized");

    //load object types
    if(gameData.LoadObjectFile("assets//data//objects.dat") == false){
      pLog->Log("Failure loading assets//data//objects.dat file");
      ::MessageBox(hWnd, "Failed to load objects.dat file", "Error", 0);
      return 0;
    }

    //load assets data //<<<<< add code to release DX device
    if(con.LoadAssetFile("assets//data//editor.dat") == false){
      pLog->Log("Failure loading data//editor.dat file");
      ::MessageBox(hWnd,"Failed to load editor.dat file", "Error", 0);
      return 0;
    }

    if(con.LoadAssetFile("assets//data//assets.dat") == false){
      pLog->Log("Failure loading data//assets.dat file");
      ::MessageBox(hWnd,"Failed to load assets.dat file", "Error", 0);
      return 0;
    }

    //setup camera 
    Camera cam;
    D3DXVECTOR3 position(0.0f, 2.0f, 0.0f);
    cam.setPosition(&position); 
    gTimerKey.initialize();
    gTimerKey.ResetTimer();
    mouse.SetHandle(hWnd);
    gTimerFPS.initialize();
    
    //game timer for update
    CTimer timerGame;
    timerGame.initialize();

    //define events for changing game states
    g_pStateIntro->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateMain->addTransitionEvent(EVENT_GO_CONTROL, g_pStateControl);
    g_pStateMain->addTransitionEvent(EVENT_GO_HELP, g_pStateHelp);
    g_pStateMain->addTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);
    g_pStateControl->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateHelp->addTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateCredits->addTransitionEvent(EVENT_GO_QUIT, g_pStateQuit);
    g_pCurrent = g_pStateIntro;

    //load sound
    CAudioManager *pAudio = CAudioManager::Instance();
    if(pAudio->IsValidSound() == true)
      pLog->Log("Audio system initialized", pAudio->Size());
    else
      pLog->Log("Audio failure!");

    if(pAudio->LoadAssetFile("assets//data//assets.dat") == false){
      pLog->Log("Failure loading data//assets.dat audio files!");
      ::MessageBox(hWnd,"Failed to load assets.dat audio files!", "Error", 0);
      //return 0;
    }
    for(int i = 0; i < pAudio->Size(); i++){
      pLog->Log(pAudio->GetFilename(i), i); //write filename and file index to log file
    }
    gAudioTimer.initialize();
    if(gameData.m_playMusic == true)
      pAudio->PlaySoundClip(0, 1);

    // enter the main loop
    MSG msg;
    pLog->Log("Enter main loop");
    float timeDiff = 0.0f;
    
    while(g_bRunning)
    {
        DWORD starting_point = GetTickCount();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //manage frame count determination
        gLoopCount++;
        if(gTimerFPS.getTimer(1.0) == true){
          //gFPS = gLoopCount;
          gameData.m_FPS = gLoopCount;
          gLoopCount = 0;   
          gSecondCount++;
          if(gSecondCount > 30){ //log every 30 seconds
            gSecondCount = 0;
            pLog->Log("FPS",gameData.m_FPS);
          }
        }

        //stores mouse button status for use in other classes
        gameData.m_bLeftMouseDown =  mouse.LeftButtonDown();
        gameData.m_bRightMouseDown = mouse.RightButtonDown();

        g_pNext = g_pCurrent->update(timerGame.getTimeDifference(), cam, gameData, con);
        if(g_pNext == g_pStateQuit)
          g_bRunning = false;
        else if(NULL != g_pNext)
	      {
          //g_pCurrent->deactivate();
          g_pCurrent = g_pNext;
          //g_pCurrent->activate();
        }  
        
        g_pCurrent->render(con, cam, gameData);

        // check the 'escape' key
        if(g_bRunning == false){
          gExitProgram = true;
          PostMessage(hWnd, WM_DESTROY, 0, 0);
        }
      
    }
    pLog->Log("Exited main loop");

    // clean up DirectX and COM
    con.CleanupDirectX();
    Shutdown();

    pLog->Log("DirectX Cleaned Up");
    pLog->Log("Shutdown complete!");
    pLog->Log("***************************************");
    pLog->Log(" Program Terminated Normally");
    pLog->Log("***************************************");

    return msg.wParam;
}

/***********************************************************************************
  WindowProc - Main message handler
***********************************************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
  {
  case WM_CREATE:
    g_pStateIntro->initialize();
    g_pStateMain->initialize();
    g_pStateControl->initialize();
    g_pStateCredits->initialize();
  return 0;

  case WM_LBUTTONDOWN:
    mouse.SetLeftButtonDown(true);
    return 0;

  case WM_LBUTTONUP:
    mouse.SetLeftButtonDown(false);
    return 0;

  case WM_RBUTTONDOWN:
    mouse.SetRightButtonDown(true);
    return 0;

  case WM_RBUTTONUP:
    mouse.SetRightButtonDown(false);
    return 0;

  case WM_DESTROY:
      {
          PostQuitMessage(0);
          return 0;
      } break;
  }

  return DefWindowProc (hWnd, message, wParam, lParam);
}

//Shutdown everything NOT DirectX
void Shutdown(){
  delete g_pStateControl;
  delete g_pStateMain;
  delete g_pStateIntro;
  delete g_pStateCredits;
  delete g_pStateHelp;
}