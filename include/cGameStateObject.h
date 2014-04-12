//cGameStateObject.h
//base class...all other game states are derived from here
#ifndef GAME_STATE_OBJECT_H
#define GAME_STATE_OBJECT_H

#include <windows.h>
#include <vector>
#include "CGraphics.h"
#include "CGameData.h"
#include "camera.h"

//constants for game state transition events
const int EVENT_GO_NO_WHERE = 0;
const int EVENT_GO_INTRO = 1;
const int EVENT_GO_MAIN = 2;
const int EVENT_GO_CREDITS = 3;
const int EVENT_GO_CONTROL = 4;
const int EVENT_GO_HELP = 5;
const int EVENT_GO_QUIT = 6;

class cGameStateObject;//forward initialization

struct TRANSITION_EVENT{
  int event;
  cGameStateObject* p_gso;
};

class cGameStateObject{
public:
  cGameStateObject();
  virtual ~cGameStateObject();

  virtual void initialize();
  virtual void activate();
  virtual cGameStateObject* update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con);
  virtual void render(CGraphics &con, Camera &cam, CGameData &data);
  virtual void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  virtual void deactivate();
  virtual void resume();
  virtual void pause();
  virtual void save();
  virtual void addTransitionEvent(int event, cGameStateObject* p_Next);

protected:

private:
  
  

};

#endif //GAME_STATE_OBJECT_H