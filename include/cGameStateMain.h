#ifndef GAME_STATE_MAIN_H
#define GAME_STATE_MAIN_H

#include "cgamestateobject.h"
#include "CTimer.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include <iostream>
#include <mmsystem.h>
#include <windows.h>
#include "..\resource.h"
#include "CGameData.h"
#include "camera.h"
#include "keystatus.h"

class cGameStateMain :  public cGameStateObject{
public:
  cGameStateMain(void);
  ~cGameStateMain(void);
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
  int m_event;
  CTimer m_timer;
  int m_selection;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;
  
  //prevents ENTER key bleed through to other states
  bool m_bActivated;
  CTimer m_enableTimer;
  CMouse m_mouse;

};

#endif