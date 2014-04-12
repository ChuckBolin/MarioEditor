/**************************************************************************************
Filename: CGameData.h Date: December, 2008
Purpose: Manages all game data.
**************************************************************************************/
#ifndef CGAMEDATA_H
#define CGAMEDATA_H

#include <string>
#include <windows.h>
#include "CLog.h"
#include "CINIReader.h"
#include "CFileReader.h"
#include "CGraphics.h"

class CGraphics;

struct GAME_OBJECT_TYPE{
  std::string name;
  int objectID;
  int layer;
  int behavior;
  int soundID;
  int spriteID1;
  int spriteID2;
  int spriteID3;
};

struct GAME_OBJECT{

  //data object from m_objectType
  std::string name;
  int objectID;
  int layer;
  int behavior;
  int soundID;
  int spriteID1;
  int spriteID2;
  int spriteID3;

  //mobility data
  D3DXVECTOR3 pos;
  D3DXVECTOR3 vel;
  float direction;
  float turnRate;
  bool alive;
  bool dying;
  float radius;
  int mode;
  
  //gi data
  int spriteID; //current sprite
  int alpha;
  float angle;
  int width;
  int height;
  int left;
  int top;
  int asset;
  int frameCount;
  double totalTime;
  int maxFrames;
  double updateInterval;
  float collisionDiameter;
    

};
class CGameData{
public:
  CGameData();
  bool LoadConfigFile(std::string filename);
  
  int m_FPS;
  std::string m_version;
  HWND m_hWnd;

  //screen information
  int m_screenLeft;
  int m_screenTop;
  int m_screenWidth;
  int m_screenHeight;
  bool m_bFullScreen;

  //environmental boundaries
  int m_worldX;
  int m_worldY;


  int m_highlightX;
  int m_highlightY;
  bool m_bHighlight;
  int m_highlightObject;
  
  bool m_bLeftMouseDown;
  bool m_bRightMouseDown;

  //std::vector<CObject*> m_pObject;

  //display debug info on screen
  bool m_displayDebugInfo;

  //sound
  bool m_playMusic;
  void LogKeyValue();
  void LoadGameLevel(std::string filename);
  int LevelSize();
  std::vector<GAME_OBJECT> m_level;
  GAME_OBJECT GetLevelData(int i);
  bool LoadObjectFile(std::string filename);
  std::vector<GAME_OBJECT_TYPE> m_objectType;

  bool AddGraphicDataToLevelData(CGraphics &con);
  bool AddObject(CGraphics &con, std::string objectName, int x, int y);
  //bool AddObject(CGraphics &con, int objectID, int x, int y);
  std::string GetObjectName(int id);

  void LogObjects();
  GAME_OBJECT_TYPE GetObjectData(std::string name);

  void ClearLevel();
  void SaveLevel (int level);
  void OpenLevel (int level, CGraphics &con);

  //variables loaded from level1.dat file
  int m_screenColorRed;
  int m_screenColorGreen;
  int m_screenColorBlue;
  int m_worldLeft;
  int m_worldRight;
  int m_worldTop;
  int m_worldBottom;


//  void InitializeObjectsFromAssets(CGraphics &con);
private:
  
};

#endif