#ifndef GAME_STATE_QUIT_H
#define GAME_STATE_QUIT_H

#include "cgamestateobject.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "camera.h"


class cGameStateQuit :  public cGameStateObject{
public:
  cGameStateQuit(void);
  ~cGameStateQuit(void);
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
};

#endif
