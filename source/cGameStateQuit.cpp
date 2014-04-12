#include "..\include\cgamestatequit.h"

cGameStateQuit::cGameStateQuit(void){
}

cGameStateQuit::~cGameStateQuit(void)
{
	m_TE.clear();
}

void cGameStateQuit::initialize(){

}

void cGameStateQuit::activate(){
}

cGameStateObject* cGameStateQuit::update(double timeDifference, Camera &cam, CGameData &data, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
 

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateQuit::render(CGraphics &con, Camera &cam, CGameData &data){
 
  return;
}

void cGameStateQuit::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateQuit::deactivate(){
}
void cGameStateQuit::resume(){
}
void cGameStateQuit::pause(){
}
void cGameStateQuit::save(){
}

void cGameStateQuit::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
