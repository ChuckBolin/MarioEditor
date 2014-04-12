#include "..\include\cgamestateintro.h"

cGameStateIntro::cGameStateIntro(void){
}

cGameStateIntro::~cGameStateIntro(void)
{
	m_TE.clear();
}

void cGameStateIntro::initialize(){
  m_timer.initialize();
  m_bugTimer.initialize();
}

void cGameStateIntro::activate(){

}

cGameStateObject* cGameStateIntro::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  
  //initialize data first time this state is called
  static bool bFirstTime = true;
  if(bFirstTime == true){
    m_effect1 = con.GetSprite(1000);
    m_effect2 = con.GetSprite(1000);
    m_effect2.frameCount = 2;
    m_effect3 = con.GetSprite(1000);
    m_effect3.frameCount = 4;
    bFirstTime = false;
  }

  //animates sprites surrounding red locations
  static double timeEffect = 0;
  timeEffect += timeDifference;
  if(timeEffect > m_effect1.updateInterval){
    timeEffect = 0;

    m_effect1.frameCount++;
    m_effect2.frameCount++;
    m_effect3.frameCount++;

    if(m_effect1.frameCount > m_effect1.frameMax)
      m_effect1.frameCount = 0;
    if(m_effect2.frameCount > m_effect2.frameMax)
      m_effect2.frameCount = 0;
    if(m_effect3.frameCount > m_effect3.frameMax)
      m_effect3.frameCount =0;
    
  }

  //keystrokes
  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_SPACE))
    m_event = EVENT_GO_MAIN;

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateIntro::render(CGraphics &con, Camera &cam, CGameData &data){
  ::ShowCursor(false);
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
  
  //intro image
  //gi = con.GetSprite(1001);//Editor title
  //con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width * gi.scale/2, 75, 255, 255, 255); //gi.width * gi.scale/2, 75, 255, 255, 255);
  //gi = con.GetSprite(1000);//Mario graphics
  //con.RenderGraphicModulate(gi, data.m_screenWidth/2 - gi.width * gi.scale/2, data.m_screenHeight/2 - gi.height * gi.scale/2 + 50, 255, 255, 255);
  gi = con.GetSprite(1015);
  con.RenderGraphicModulate(gi, 0, 0, 255, 255, 255);
  gi = con.GetSprite(1001);//Editor title
  con.RenderGraphicModulate(gi,data.m_screenWidth/2 - gi.width/2, 25, 255, 255, 255); //gi.width * gi.scale/2, 75, 255, 255, 255);
  
  //draw animated effect
  //con.RenderGraphicModulate(m_effect1, 379 - m_effect1.width/2, 412 - m_effect1.height/2, 255, 255, 255);
  //con.RenderGraphicModulate(m_effect2, 504 - m_effect2.width/2, 419 - m_effect2.height/2, 255, 255, 255);
  //con.RenderGraphicModulate(m_effect3, 790 - m_effect3.width/2, 296 - m_effect3.height/2, 255, 255, 255);

  //player prompt
  con.Print("Press SPACEBAR to Continue!", F_V20B, data.m_screenWidth/2 - 130, data.m_screenHeight - 35, 255, 255, 0, 255);

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

void cGameStateIntro::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateIntro::deactivate(){
  //CAudioManager *pAudio = CAudioManager::Instance();
  //pAudio->KillAll();
  
}
void cGameStateIntro::resume(){
}
void cGameStateIntro::pause(){
}
void cGameStateIntro::save(){
}

void cGameStateIntro::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}