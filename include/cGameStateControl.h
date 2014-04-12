#ifndef GAME_STATE_CONTROL_H
#define GAME_STATE_CONTROL_H
//#include <iostream>
//using std::cout;
//using std::endl;
#include <iomanip>

#include "cgamestateobject.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "..\resource.h"
#include "CGameData.h"
#include "CLog.h"
#include "camera.h"

class cGameStateControl :  public cGameStateObject{
public:
  cGameStateControl(void);
  ~cGameStateControl(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con);
  void render(CGraphics &con, Camera &cam, CGameData &data);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);
//  float GetDistance(CObject *obj1, CObject *obj2);

private:

  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  bool m_bDone;
  int m_event;
  CMouse m_mouse;
  int m_viewChoice;

  //animated arrow stuff
  GRAPHIC_IMAGE giLeft;
  GRAPHIC_IMAGE giRight;
  GRAPHIC_IMAGE giUp;
  GRAPHIC_IMAGE giDown;
  double m_timeLeft;
  double m_timeRight;
  double m_timeUp;
  double m_timeDown;

  //screen reference information for editor
  int m_groundPlane; //horizontal line dividing between sky and ground
  int m_levelTop;    //world top starts here
  int m_gridSpaceX;       //snap grid spacing
  int m_gridSpaceY;       //snap grid spacing

  //gui information
  int objectPanelX;
  int objectPanelY;
  int m_selectedObject;
  int mapX;
  int mapY;
  int mapZoom;
  int infoX;
  int infoY;
  int m_display;
  int m_displayType;
  int m_mx;
  int m_my;
  int m_level; //selected level
  int m_lastX;
  int m_lastY;

  //pause function
  bool m_pause;
  bool m_id;

};

#endif
