#include "..\include\cGameStateControl.h"
 
cGameStateControl::cGameStateControl(){
}

cGameStateControl::~cGameStateControl()
{
	m_TE.clear();
}

void cGameStateControl::initialize(){
  m_viewChoice = 0;

}

void cGameStateControl::activate(){

}
//***************************************************************
//                         U P D A T E
//***************************************************************
cGameStateObject* cGameStateControl::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  CLog *pLog = CLog::Instance();

  static bool bStop = false;
  D3DXVECTOR3 position;
  int mx = 0;
  int my = 0;
  
  //perform this code only once
  //****************************
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;
    m_viewChoice = 4;    
    srand((unsigned int)time(NULL) );
    data.m_worldX = 0;
    data.m_worldY = 0;
    data.m_worldLeft = 0;
    data.m_worldTop = 0;
    data.m_worldRight = 10107;
    data.m_worldBottom = 768;
    m_levelTop = 96;
    m_groundPlane = 588;//448;
    m_selectedObject = 0;
    m_level = 1;
    m_lastX = -1;
    m_lastY = -1;

    data.LoadGameLevel("assets\\data\\level1.dat"); //missing sprite data
    data.AddGraphicDataToLevelData(con);            //update graphic data
    //pLog->Log("**************************************");
    //for(int i=0; i < data.m_level.size(); ++i)
    //  pLog->Log(data.m_level[i].spriteID, data.m_level[i].pos.x, data.m_level[i].pos.y);
  }

  //Pause function
  static double pauseTime = 0;
  pauseTime += timeDifference;
  if(::GetActiveWindow() == data.m_hWnd && keyDown('P') == true && pauseTime > 0.3){
    pauseTime = 0;
    m_pause = !m_pause;
  }
  if(m_pause == true)
    return 0;
  
  m_mouse.SetHandle(data.m_hWnd);//needed for mouse

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))//quit this mode
    m_event = EVENT_GO_MAIN;

  static double timeDiff = 0;
  static double keyTime = 0;
  timeDiff = timeDifference;
  keyTime += timeDiff;

  
  static double undoTime;
  undoTime += timeDifference;

  //select color...piggy back off of undoTime
  int index = 0;
  if(::GetActiveWindow() == data.m_hWnd && undoTime > 0.2 && data.m_bLeftMouseDown == true){
    for(int j = 0; j < 2; ++j){
      for(int i = 0; i < 4; ++i){
        if(m_mouse.MouseX() > 590 + (i * 20) && m_mouse.MouseX() < 610 + (i * 20) &&
          m_mouse.MouseY() > 20 + (j * 20)  && m_mouse.MouseY() < 40 + (j * 20)){
          index = i + (j * 4);
          switch(index){
            case 0:
              data.m_screenColorRed = 147;data.m_screenColorGreen = 0; data.m_screenColorBlue = 0;
              break;
            case 1:
              data.m_screenColorRed = 51; data.m_screenColorGreen = 126; data.m_screenColorBlue = 0;
              break;
            case 2:
              data.m_screenColorRed = 0;data.m_screenColorGreen = 255; data.m_screenColorBlue = 255;
              break;
            case 3:
              data.m_screenColorRed = 0; data.m_screenColorGreen = 111; data.m_screenColorBlue = 255;
              break;
            case 4:
              data.m_screenColorRed = 0;data.m_screenColorGreen = 0; data.m_screenColorBlue = 0;
              break;
            case 5:
              data.m_screenColorRed = 204; data.m_screenColorGreen = 111; data.m_screenColorBlue = 0;
              break;
            case 6:
              data.m_screenColorRed = 255;data.m_screenColorGreen = 255; data.m_screenColorBlue = 105;
              break;
            case 7:
              data.m_screenColorRed = 153; data.m_screenColorGreen = 153; data.m_screenColorBlue = 138;
              break;
          }
        }
      }
    }
  }

  //add ground tiles
  int maxGround;
  if(::GetActiveWindow() == data.m_hWnd && keyDown('G') && undoTime > 0.2){
    undoTime = 0;
    maxGround = (data.m_worldRight - data.m_worldLeft)/48;
    for(int i = 0; i < maxGround; ++i){
      data.AddObject(con, "ground", i * 47, m_groundPlane);
      data.AddObject(con, "ground", i * 47, m_groundPlane + 48);
    }
  }

  //undo object
  if(::GetActiveWindow() == data.m_hWnd && undoTime > 0.2 &&
     m_mouse.MouseX() > 522 && m_mouse.MouseX() < 576 &&
     m_mouse.MouseY() > 10 && m_mouse.MouseY() < 43 &&  data.m_bLeftMouseDown == true){
       undoTime = 0;
       data.m_level.pop_back();
  }

  //delete object
  if(::GetActiveWindow() == data.m_hWnd && undoTime > 0.2 && data.m_bRightMouseDown == true){
    
    for(int i = 0; i < data.m_level.size(); ++i){
      m_gridSpaceX = data.m_level[i].width;
      m_gridSpaceY = data.m_level[i].height;
      mx = ((m_mouse.MouseX()+ data.m_worldX)/m_gridSpaceX) * m_gridSpaceX;
      my = ((m_mouse.MouseY()/m_gridSpaceY) * m_gridSpaceY) - 2 * m_gridSpaceY;
      m_mx = mx;
      m_my = my;  

      if(m_mx > data.m_level[i].pos.x - data.m_level[i].width/2 && 
         m_mx < data.m_level[i].pos.x + data.m_level[i].width/2 &&
         m_my > data.m_level[i].pos.y - data.m_level[i].height/2 && 
         m_my < data.m_level[i].pos.y + data.m_level[i].height/2){
          //pLog->Log("Object found here.........");
        data.m_level.erase(data.m_level.begin() + i, data.m_level.begin() + i + 1);
        undoTime = 0;
        break;
      }
    }
  }

  //update all objects
  //for(int i = 0; i < data.m_pObject.size(); ++i){
  //  data.m_pObject[i]->Update(timeDifference);
  //}
  double scrollSpeed = 1000;

  //only process keys if this window has focus
  if(::GetActiveWindow() == data.m_hWnd){// && gTimerKey.getTimer(0.03) == true){

    if(keyDown(VK_LEFT) == true ||(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseX() < 64)){
      data.m_worldX = data.m_worldX - (scrollSpeed * timeDiff);
      if(data.m_worldX < data.m_worldLeft)
        data.m_worldX = data.m_worldLeft;
    }
    if(keyDown(VK_RIGHT) == true ||(::GetActiveWindow() == data.m_hWnd && m_mouse.MouseX() > con.GetScreenWidth() - 64)){
      data.m_worldX = data.m_worldX + (scrollSpeed * timeDiff);
      if(data.m_worldX > data.m_worldRight)
        data.m_worldX = data.m_worldRight;
    }
    if(keyDown(VK_NEXT) == true && keyTime > 0.2){
      m_display++;
      if(m_display > 2)
        m_display = 0;
      keyTime = 0;
    }
    if(keyDown(VK_PRIOR) == true && keyTime > 0.2){
      m_display--;
      if(m_display < 0)
        m_display = 2;
      keyTime = 0;
    }
  }

  //user selects level
  if(m_mouse.MouseX() > 118 && m_mouse.MouseX() < 248 &&
    m_mouse.MouseY() > 14 && m_mouse.MouseY() < 39 &&
    ::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true){
      if(m_mouse.MouseX() > 118 && m_mouse.MouseX() <= 152)
        m_level = 1;
      else if(m_mouse.MouseX() > 152 && m_mouse.MouseX() <= 180 )
        m_level = 2;
      else if(m_mouse.MouseX() > 180 && m_mouse.MouseX() <= 215 )
        m_level = 3;
      else if(m_mouse.MouseX() > 215 && m_mouse.MouseX() < 248)
        m_level = 4;
    }

  //clear
  if(m_mouse.MouseX() > 19 && m_mouse.MouseX() < 240 &&  
    m_mouse.MouseY() > 51 && m_mouse.MouseY() < 77 &&
    ::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true)
      data.ClearLevel();
  
  //open 
  else if(m_mouse.MouseX() > 279 && m_mouse.MouseX() < 506 &&
    m_mouse.MouseY() > 15 && m_mouse.MouseY() < 40 &&
    ::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true)
      data.OpenLevel(m_level, con);
  
  //save
  else if(m_mouse.MouseX() > 279 && m_mouse.MouseX() < 506 &&
    m_mouse.MouseY() > 51 && m_mouse.MouseY() < 77 &&
    ::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true)
      data.SaveLevel(m_level);

  //determine if mouse if over the panel
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(1014);
  objectPanelX = con.GetScreenWidth() - 64 - gi.width;
  objectPanelY = 96;
  bool bOnPanel = false;
  if(m_mouse.MouseX() > objectPanelX && m_mouse.MouseX() < objectPanelX + gi.width &&
     m_mouse.MouseY() > objectPanelY && m_mouse.MouseY() < objectPanelY + gi.height)
     bOnPanel = true;

  //mouse click on panel...selecting an object
  if(::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true && bOnPanel == true){
    for(int i = 0; i < 20; ++i){
      if(m_mouse.MouseY() > objectPanelY + 68 + (i * 20) && m_mouse.MouseY() < objectPanelY + 68 + ((i + 1) * 20))
        m_selectedObject = i;
    }
  }

  //add an object
  bool bFound = false;
  GAME_OBJECT go;
  GAME_OBJECT_TYPE got;

  std::string sName;

  if(::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true &&
    bOnPanel == false && m_mouse.MouseY() >  96){
    sName = data.GetObjectName(m_selectedObject);
    
    //name is valid
    if(sName.length() > 0){      
      got = data.GetObjectData(sName);
      gi = con.GetSprite(got.spriteID1);
      m_gridSpaceX = gi.width - 1;
      m_gridSpaceY = gi.height;
      mx = ((m_mouse.MouseX()+ data.m_worldX)/m_gridSpaceX) * m_gridSpaceX;
      my = ((m_mouse.MouseY()/m_gridSpaceY) * m_gridSpaceY) - 2 * m_gridSpaceY;

      if(sName == "small_hill")
        my = m_groundPlane - 54;
      else if(sName == "big_hill")
        my = m_groundPlane - 108;
      else if(sName == "small_pipe")
        my = m_groundPlane - 96;
      else if(sName == "medium_pipe")
        my = m_groundPlane - 147;
      else if(sName == "big_pipe")
        my = m_groundPlane - 192;
      else if(sName == "small_bush")
        my = m_groundPlane - 49;
      else if(sName == "big_bush")
        my = m_groundPlane - 49;

      if(sName == "ground" || sName == "block" || sName == "brick" || sName == "question_object")
        my = my + 13;

      m_mx = mx;
      m_my = my;  

      //add object
      if(m_lastX != mx || m_lastY != my)
        data.AddObject(con, sName, mx, my);

      m_lastX = mx;
      m_lastY = my;
    }    
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

//***************************************************************
//                         R E N D E R 
//***************************************************************
void cGameStateControl::render(CGraphics &con, Camera &cam, CGameData &data){
  //::ShowCursor(true);
  CLog *pLog = CLog::Instance();
  static bLostDevice = false;
  static int lostCount = 0;
  GRAPHIC_IMAGE gi;
  HRESULT hr;

  //set aircraft camera to 1000 feet
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;
  }

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    pLog->Log("Failed test");
    if(hr == D3DERR_DEVICELOST)
      return;
    pLog->Log("No lost");
    if( hr == D3DERR_DEVICENOTRESET){// && bLostDevice == true  ){
      lostCount++;
      pLog->Log("Time to reset");
      con.DeleteLost();
      pLog->Log("Deleted everything");
      data.LoadConfigFile("assets//data//config.txt");
      data.LoadGameLevel("assets//data//level1.dat");

      hr = con.m_d3ddev->Reset(&con.GetDPP());
      pLog->Log("Dev reset");
      con.ReloadLost();
      pLog->Log("ReloadLost");
      bLostDevice = false;
      return;
    }
  }

  D3DXMATRIX V;
  cam.getViewMatrix(&V, m_viewChoice);
  con.m_d3ddev->SetTransform(D3DTS_VIEW, &V);

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(data.m_screenColorRed, data.m_screenColorGreen, data.m_screenColorBlue), 1.0f, 0);//0, 111, 255
  con.m_d3ddev->BeginScene();    // begins the 3D scene
 
  GAME_OBJECT go;
  
  //render beginning with smallest layer number
  //*******************************************
  for(int j = 2; j < 14; j+=2){
    for(int i = 0; i < data.m_level.size(); ++i){
      go = data.m_level[i];
      if(go.layer == j)
        con.RenderGraphicModulate(go,  (go.pos.x - data.m_worldX), (m_levelTop + go.pos.y ), 255, 255, 255);
    }
  }

  //top menu bar - and highlighted stuff
  gi = con.GetSprite(1011);//left
  con.RenderGraphicModulate(gi, 0, 0, 255, 255, 255);
  con.DrawRect(86 + (m_level * 33), 14, 108 + (m_level * 33), 39,255, 0, 0); //highlight level #
  //clear
  if(m_mouse.MouseX() > 19 && m_mouse.MouseX() < 240 &&  m_mouse.MouseY() > 51 && m_mouse.MouseY() < 77)
      con.DrawRect(19, 51, 240, 77, 255, 255, 255);
  
  //open 
  else if(m_mouse.MouseX() > 279 && m_mouse.MouseX() < 506 &&  m_mouse.MouseY() > 15 && m_mouse.MouseY() < 40)
      con.DrawRect(279, 13, 506, 40, 255, 255, 255);
  
  //save
  else if(m_mouse.MouseX() > 279 && m_mouse.MouseX() < 506 &&  m_mouse.MouseY() > 51 && m_mouse.MouseY() < 77)
      con.DrawRect(279, 51, 506, 79, 255, 255, 255);


  //dropdown menu
  gi = con.GetSprite(1014);
  objectPanelX = con.GetScreenWidth() - 64 - gi.width;
  objectPanelY = 96;
  gi.alpha = 120;
  con.RenderGraphicModulate(gi, objectPanelX, objectPanelY, 255, 255, 255);

  //display all game object names
  for(int i = 0; i < data.m_objectType.size(); ++i){    
    if(i == m_selectedObject)
      con.Print(data.m_objectType[i].name, F_V16B, objectPanelX + 10, objectPanelY + 68 + (i * 20), 255, 0, 0, 255);
    else
      con.Print(data.m_objectType[i].name, F_V16B, objectPanelX + 10, objectPanelY + 68 + (i * 20), 0, 0, 0, 255);
  }
 
  //display mouse arrows
  if(m_mouse.MouseX() < 64){
    gi = con.GetSprite(1012);
    con.RenderGraphicModulate(gi,4, 256, 255, 255, 255);
  }
  else if(m_mouse.MouseX() > 960){
    gi = con.GetSprite(1013);
    con.RenderGraphicModulate(gi, 917, 256, 255, 255, 255);
  }

  //draw mouse if on menu bar
  int mx;
  int my;
  
  //below menu bar
  if(m_mouse.MouseY() > 96){
    mx = m_mx;
    my = m_my;
    //con.DrawCircle( mx, my, 255, 0, 0, 3);
    //con.DrawCircle( mx, my, 255, 0, 0, 5);
    con.DrawCircle(m_mouse.MouseX(), m_mouse.MouseY(), 255, 0, 0, 5);
    con.DrawLine(m_mouse.MouseX()- 18 , m_mouse.MouseY()- 18, m_mouse.MouseX() - 6, m_mouse.MouseY()- 6, 0, 0, 0);
    con.DrawLine(m_mouse.MouseX()- 18 , m_mouse.MouseY()+ 18, m_mouse.MouseX() - 6, m_mouse.MouseY()+ 6, 0, 0, 0);
    con.DrawLine(m_mouse.MouseX()+ 18 , m_mouse.MouseY()- 18, m_mouse.MouseX() + 6, m_mouse.MouseY()- 6, 0, 0, 0);
    con.DrawLine(m_mouse.MouseX()+ 18 , m_mouse.MouseY()+ 18, m_mouse.MouseX() + 6, m_mouse.MouseY()+ 6, 0, 0, 0);
  }
  
  //above menu bar
  else{
    con.DrawCircle(m_mouse.MouseX(), m_mouse.MouseY(), 255, 255, 255, 3);
    con.DrawCircle(m_mouse.MouseX(), m_mouse.MouseY(), 255, 255, 255, 5);
    con.DrawLine(m_mouse.MouseX()- 18 , m_mouse.MouseY()- 18, m_mouse.MouseX() - 6, m_mouse.MouseY()- 6, 255, 255, 255);
    con.DrawLine(m_mouse.MouseX()- 18 , m_mouse.MouseY()+ 18, m_mouse.MouseX() - 6, m_mouse.MouseY()+ 6, 255, 255, 255);
    con.DrawLine(m_mouse.MouseX()+ 18 , m_mouse.MouseY()- 18, m_mouse.MouseX() + 6, m_mouse.MouseY()- 6, 255, 255, 255);
    con.DrawLine(m_mouse.MouseX()+ 18 , m_mouse.MouseY()+ 18, m_mouse.MouseX() + 6, m_mouse.MouseY()+ 6, 255, 255, 255);
  }


  //player prompt
  con.Print("Press ESCAPE to Exit!", F_V20B, data.m_screenWidth/2 - 130, data.m_screenHeight - 30, 0, 0, 0, 255);

  if(m_pause == true){
    //gi = con.GetSprite(9);
    //con.RenderGraphicModulate(gi, con.GetScreenCenterX() - gi.width/2, con.GetScreenCenterY() - gi.height/2, 255, 255, 255);
  }

  if(data.m_displayDebugInfo == true){
    con.Print("FPS", data.m_FPS , F_V20, 20, 710,  255, 255, 255, 255);   
    con.Print("Pos X", data.m_worldX , F_V20, 880, 35,  255, 255, 0, 255);   
    con.Print("Pos Y", data.m_worldY , F_V20, 880, 55,  255, 255, 0, 255);   
    con.Print("Mouse X", m_mouse.MouseX() + data.m_worldX , F_V20, 720, 35,  255, 255, 0, 255);   
    con.Print("Mouse Y", m_mouse.MouseY() + data.m_worldY  , F_V20, 720, 55, 255, 255, 0, 255);   


  }  

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    if(hr == D3DERR_DEVICELOST){
      bLostDevice = true;
      lostCount++;
    }
  }
}

void cGameStateControl::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateControl::deactivate(){
  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->KillAll();

  
}
void cGameStateControl::resume(){
}
void cGameStateControl::pause(){
}
void cGameStateControl::save(){
}

void cGameStateControl::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
/*
float cGameStateControl::GetDistance(CObject *obj1, CObject *obj2){
  return sqrt((obj1->go.pos.x - obj2->go.pos.x) * (obj1->go.pos.x - obj2->go.pos.x) + 
              (obj1->go.pos.y - obj2->go.pos.y) * (obj1->go.pos.y - obj2->go.pos.y));
}
*/