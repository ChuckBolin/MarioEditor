#include "..\include\cgamestatecredits.h"

cGameStateCredits::cGameStateCredits(void){
}

cGameStateCredits::~cGameStateCredits(void)
{
	m_TE.clear();
}

SCROLL_MSG cGameStateCredits::SetMessage(int x, int font, int red, int green, int blue, std::string s){
  SCROLL_MSG msg;
  msg.x = x;
  msg.spriteID = -1;
  msg.type = 0; //font
  msg.font = font;
  msg.red = red;
  msg.green = green;
  msg.blue = blue;
  msg.msg = s;
  return msg;
}


SCROLL_MSG cGameStateCredits::SetGraphic(int x, int id){
  SCROLL_MSG msg;
  msg.type = 1;//graphic
  msg.x = x;
  msg.spriteID = id;
  msg.font = F_V20;
  msg.red = 0;
  msg.green = 0;
  msg.blue = 0;
  msg.msg = "";
  return msg;
}

SCROLL_MSG cGameStateCredits::ClearMessage(){
  SCROLL_MSG msg;
  msg.type = 0; //font
  msg.font = F_V20;
  msg.red = 0;
  msg.green = 0;
  msg.blue = 0;
  msg.spriteID = -1;
  msg.msg = "";
  msg.x = -1;
  return msg;
}


void cGameStateCredits::initialize(){

  SCROLL_MSG msg;
 // msg = ;
  m_msg.push_back(SetGraphic(-1, 1006));//credits graphic
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(SetMessage(-1,F_V20, 180, 0, 0, "Team \"Pacific - Atlantic Gameworks\" "));
  m_msg.push_back(ClearMessage());
  m_msg.push_back(SetGraphic(50, 1009));//large cloud
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(SetMessage(500,F_V20, 0, 70, 0, "www.gameinstitute.com"));
  m_msg.push_back(SetMessage(500,F_V20, 0, 70, 0, "  Game Challenge 10"));
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(SetMessage(-1,F_V20, 0, 0, 0, "Editor Programmed by Chuck Bolin"));
  m_msg.push_back(SetMessage(-1,F_V16, 0, 0, 0, "January 2009"));
  m_msg.push_back(ClearMessage());
  m_msg.push_back(SetGraphic(712, 1010));//credits graphic
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());
  m_msg.push_back(ClearMessage());

  m_timer.initialize(); 
  m_scrollValue = 0;

  //used to show several images in the background
  m_secondTimer.initialize();
  m_secondCount = 0;

  //used for fading
  m_red = 255;//255;
  m_green =255;//255;
  m_blue = 255;//255;
  m_fadeTimer.initialize();
  
}

void cGameStateCredits::activate(){

}

cGameStateObject* cGameStateCredits::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  static double timeDiffTotal = 0;
  GRAPHIC_IMAGE gi;

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);

  //one time code
  static bool bFirstTime = false;
  int width = 0;
  int letterCount = 0;

  //setup message scrolling parameters
  if(bFirstTime == false){

    for(int i = 0; i < m_msg.size(); ++i){

      //graphic image
      if (m_msg[i].type == 1){
        m_msg[i].gi = con.GetSprite(m_msg[i].spriteID);
        if(m_msg[i].x == -1) //center sprite
          m_msg[i].x = con.GetScreenWidth()/2 - m_msg[i].gi.width/2;
      }

      //font size - attempt to center text
      else {
        letterCount = m_msg[i].msg.size();
        width = 9;
        if(m_msg[i].font == 0 || m_msg[i].font == 1 || m_msg[i].font == 6 || m_msg[i].font == 7)
          width = 5;
        else if(m_msg[i].font == 2 || m_msg[i].font == 3 || m_msg[i].font == 8 || m_msg[i].font == 9)
          width = 6;
        else if(m_msg[i].font == 4 || m_msg[i].font == 5 || m_msg[i].font == 10 || m_msg[i].font == 11)
          width = 7;
        else if(m_msg[i].font == 12 || m_msg[i].font == 13)
          width = 9;
        if(m_msg[i].x == -1) //center font
          m_msg[i].x = con.GetScreenWidth()/2 - (letterCount * width)/2;
      }
    }
    bFirstTime = true;
  }

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))
    m_event = EVENT_GO_QUIT;

  //terminate program after scrolling greater than 15 seconds
  if(m_secondCount > 20)
    m_event = EVENT_GO_QUIT;

  //update counter for displaying images
  if(m_secondTimer.secondTimer() == true){
    m_secondCount++;
  }

  //tracks time
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > 0.025){
    timeDiffTotal = 0;
    m_scrollValue += 2;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateCredits::render(CGraphics &con, Camera &cam, CGameData &data){

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
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 200, 200), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

  //render credits
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(2);
  //con.RenderGraphicModulate(gi, data.m_screenWidth - 240, 40, 255, 255, 255);

  //scrolls message
  int posY = 0;
  
  for(int i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(m_msg[i].type == 0)  //font
      con.Print(m_msg[i].msg, m_msg[i].font , m_msg[i].x, con.GetScreenHeight() - posY, m_msg[i].red, m_msg[i].green, m_msg[i].blue,255);
    else{//graphic
      con.RenderGraphicModulate(m_msg[i].gi, m_msg[i].x, con.GetScreenHeight() -posY, 255, 255, 255);
    }
  }

  con.Print("Press ESCAPE to Continue!", F_V20B, data.m_screenWidth/2 - 130, data.m_screenHeight - 60, 255, 255, 0, 255);

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

void cGameStateCredits::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateCredits::deactivate(){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();

}
void cGameStateCredits::resume(){
}
void cGameStateCredits::pause(){
}
void cGameStateCredits::save(){
}

void cGameStateCredits::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
