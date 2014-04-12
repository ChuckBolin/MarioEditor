/*
  cGameStateMain.cpp - Controls the user interface and display for the main menu. 

*/
#include "..\include\cgamestatemain.h"
#include "..\include\keystatus.h"

cGameStateMain::cGameStateMain(void)
{
}

cGameStateMain::~cGameStateMain(void)
{
	m_TE.clear();
}

void cGameStateMain::initialize(){
  m_selection = 0; //engage

}

void cGameStateMain::activate(){

}

cGameStateObject* cGameStateMain::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    D3DXVECTOR3 position(-5.0f, 5.0f, -5.0f);
    bFirstTime = false;
  }

  //drive forward
  static int count = 0;
  count++;
  if(count > 8000)
    count = 0;

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);
  static double timeDiff = 0;
  timeDiff += timeDifference;
  if(timeDiff > 0.2){

    if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_UP)){
      m_selection--;
      if(m_selection < 0)
        m_selection = 1;
    }
    if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_DOWN)){
      m_selection++;
      if(m_selection > 1)
        m_selection = 0;
    }
    timeDiff = 0;

    if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_RETURN)){
      if(m_selection == 0)
        m_event = EVENT_GO_CONTROL;
      else if(m_selection == 1)
        m_event = EVENT_GO_CREDITS;
    }
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

void cGameStateMain::render(CGraphics &con, Camera &cam, CGameData &data){
  CLog *pLog = CLog::Instance();
  static bLostDevice = false;

  HRESULT hr;

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    pLog->Log("Failed test");
    if(hr == D3DERR_DEVICELOST)
      return;
    pLog->Log("No lost");
    if( hr == D3DERR_DEVICENOTRESET ){
      pLog->Log("Time to reset");
      con.DeleteLost();
      pLog->Log("Deleted everything");
      hr = con.m_d3ddev->Reset(&con.GetDPP());
      pLog->Log("Dev reset");
      con.ReloadLost();
      pLog->Log("ReloadLost");
      bLostDevice = false;
      return;
    }
  }

  D3DXMATRIX V;
  cam.getViewMatrix(&V, 0);
  con.m_d3ddev->SetTransform(D3DTS_VIEW, &V);

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

  GRAPHIC_IMAGE gi;
  switch(m_selection){
    //play game    
    case 0:      
      gi = con.GetSprite(1004);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width/2, data.m_screenHeight/2 - 200, 255, 255, 255);
      gi = con.GetSprite(1003);
      con.RenderGraphicModulate(gi,data.m_screenWidth/2 - gi.width/2, data.m_screenHeight/2 - 50, 255, 255, 255);
      gi = con.GetSprite(1007);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 - 250, data.m_screenHeight/2 - 200, 255, 255, 255);
      gi = con.GetSprite(1008);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 + 180, data.m_screenHeight/2 - 200, 255, 255, 255);
      break;
      
    //quit
    case 1:
      gi = con.GetSprite(1002);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width/2, data.m_screenHeight/2 - 200, 255, 255, 255);
      gi = con.GetSprite(1005);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width/2, data.m_screenHeight/2 - 50, 255, 255, 255);
      gi = con.GetSprite(1007);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 - 300, data.m_screenHeight/2 - 50, 255, 255, 255);
      gi = con.GetSprite(1008);
      con.RenderGraphicModulate(gi, data.m_screenWidth/2 + 230, data.m_screenHeight/2 - 50, 255, 255, 255);
      break;
  }  

  if(data.m_displayDebugInfo == true){
    con.Print("Version", data.m_version, F_V16, 40, 730,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16, 930, 730,  255, 255, 255, 255);
  }  
  
  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST)
      bLostDevice = true;
  }
  return;
}

void cGameStateMain::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}

void cGameStateMain::deactivate(){

}
void cGameStateMain::resume(){
}
void cGameStateMain::pause(){
}
void cGameStateMain::save(){
}

void cGameStateMain::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}