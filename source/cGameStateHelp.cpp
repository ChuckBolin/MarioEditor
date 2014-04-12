#include "..\include\cGameStateHelp.h"

cGameStateHelp::cGameStateHelp(){
}

cGameStateHelp::~cGameStateHelp()
{
	m_TE.clear();
}

void cGameStateHelp::initialize(){

}

void cGameStateHelp::activate(){
}

cGameStateObject* cGameStateHelp::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;
  }

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_MAIN;

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateHelp::render(CGraphics &con, Camera &cam, CGameData &data){
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

  con.Print("Help State press ESC to return to main menu", F_V16, 10, 10, 255, 0, 0, 255);
  
  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST)
      bLostDevice = true;
  }

  return;
}

void cGameStateHelp::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateHelp::deactivate(){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();

}
void cGameStateHelp::resume(){
}
void cGameStateHelp::pause(){
}
void cGameStateHelp::save(){
}

void cGameStateHelp::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
