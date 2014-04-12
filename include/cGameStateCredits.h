#ifndef GAME_STATE_CREDITS_H
#define GAME_STATE_CREDITS_H
#include <iostream>
using std::cout;
using std::endl;

#include "cgamestateobject.h"
#include "CGraphics.h"
#include "CAudioManager.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "camera.h"

struct SCROLL_MSG{
  std::string msg;
  int red;
  int green;
  int blue;
  int type; //0=text, 1=sprite
  int spriteID;
  int x;
  int y;
  int font;
  GRAPHIC_IMAGE gi;
};

class cGameStateCredits :  public cGameStateObject{
public:
  cGameStateCredits(void);
  ~cGameStateCredits(void);
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
  std::vector<SCROLL_MSG> m_msg;
  int m_scrollValue;
  CMouse m_mouse;

  //used to flip through images
  CTimer m_secondTimer;
  int m_secondCount;

  CTimer m_fadeTimer;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;

  //Message helper functions
  SCROLL_MSG SetMessage(int x, int font, int red, int green, int blue, std::string s);
  SCROLL_MSG SetGraphic(int x, int id);
  SCROLL_MSG ClearMessage();
};

#endif
