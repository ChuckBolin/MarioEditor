#ifndef GAME_STATE_INTRO_H
#define GAME_STATE_INTRO_H

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

class cGameStateIntro :  public cGameStateObject{
public:
  cGameStateIntro(void);
  ~cGameStateIntro(void);
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

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  CTimer m_bugTimer;

  int m_event;
  CMouse m_mouse;

  //used to animate 3 locations
  GRAPHIC_IMAGE m_effect1;//Steve
  GRAPHIC_IMAGE m_effect2;//Chuck
  GRAPHIC_IMAGE m_effect3;//Richard
};

#endif