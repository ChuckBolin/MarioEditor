#ifndef GAME_STATE_HELP_H
#define GAME_STATE_HELP_H
#include <iostream>

#include "cgamestateobject.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "CLog.h"
#include "camera.h"


class cGameStateHelp :  public cGameStateObject{
public:
  cGameStateHelp(void);
  ~cGameStateHelp(void);
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
  int m_event;
  CMouse m_mouse;
};

#endif
