#include "..\include\CGameData.h"

//game data constructor
CGameData::CGameData(){

  //screen and window information
  m_screenLeft = 0;
  m_screenTop = 0;
  m_screenWidth = 1024;
  m_screenHeight = 768;
  m_bFullScreen = 1;  //set to fullscreen by default

  m_worldX = 2500;
  m_worldY = 2500;
  m_worldLeft = 0;
  m_worldRight = 5119;
  m_worldTop = 0;
  m_worldBottom = 5119;
  m_highlightX = 2559;
  m_highlightY = 2559;
  m_bHighlight = false;
  m_highlightObject = 0;

  //display while programming
  m_displayDebugInfo = true;

  //sound
  m_playMusic = true;

}

bool CGameData::LoadConfigFile(std::string filename){
  CLog *pLog = CLog::Instance();

  //load config.txt file to configure game
  CINIReader cir;
  cir.LoadFile(filename);

  if(cir.IsValid() == false){
    pLog->Log("INI File invalid!");
    return false;
  }
  if(cir.GetNumberOfLines() < 1){
    pLog->Log("No parameters in config file!");
    return false;
  }

  std::string value;
  std::string key;
  pLog->Log("******** Loading Config.txt **********", filename);
  
  for(int i = 0; i < cir.GetNumberOfLines(); ++i){

      //read key and value from file
      key = cir.GetTerm(cir.GetLineFromFile(i),1);
      value = cir.GetTerm(cir.GetLineFromFile(i),2);
      //pLog->Log("config.txt data", key, value);

      //screen and window information
      if(key == "screen_left")
        m_screenLeft = atoi(value.c_str());
      if(key == "screen_top")
        m_screenTop = atoi(value.c_str());
      if(key == "screen_width")
        m_screenWidth = atoi(value.c_str());
      if(key == "screen_height")
        m_screenHeight = atoi(value.c_str());

      if(key == "full_screen")
        if(value == "1")
          m_bFullScreen = true;
        else
          m_bFullScreen = false;


      //display debug information
      if(key == "display_debug_info"){
        if(value == "1")
          m_displayDebugInfo = true;
        else
          m_displayDebugInfo = false;
      }

      //play background music
      if(key == "play_music"){
        if(value == "1")
          m_playMusic = true;
        else
          m_playMusic = false;
      }
  }

  return true;
}

void CGameData::LogKeyValue(){
  CLog *pLog = CLog::Instance();

  pLog->Log("int m_screenWidth", m_screenWidth);
  pLog->Log("int m_screenHeight", m_screenHeight);
  pLog->Log("bool m_displayDebugInfo", m_displayDebugInfo);
  pLog->Log("bool m_playMusic", m_playMusic);

}

GAME_OBJECT_TYPE CGameData::GetObjectData(std::string name){
  GAME_OBJECT_TYPE got;
  got.objectID = -1;

  for(int i = 0; i < m_objectType.size(); ++i){
    if(m_objectType[i].name == name){
      got = m_objectType[i];
      return got;
    }
  }
  return got;
}

std::string CGameData::GetObjectName(int id){
  std::string sName;
  sName = "";

  for(int i = 0; i < m_objectType.size(); ++i){
    if(m_objectType[i].objectID == id){
      sName = m_objectType[i].name;
      return sName;
    }
  }
  return sName;
}

//need to add sprite data to level data
bool CGameData::AddGraphicDataToLevelData(CGraphics & con){
  GAME_OBJECT go;
  GRAPHIC_IMAGE gi;

  for(int i = 0; i < m_level.size(); ++i){
    m_level[i].spriteID = m_level[i].spriteID1; //initial sprite
    gi = con.GetSprite(m_level[i].spriteID);
    m_level[i].alive = true;
    m_level[i].alpha = gi.alpha;
    m_level[i].angle = gi.angle;
    m_level[i].dying = false;
    m_level[i].frameCount = gi.frameCount;
    m_level[i].maxFrames = gi.frameMax;
    m_level[i].height = gi.height;
    m_level[i].width = gi.width;
    m_level[i].left = gi.sourceX;
    m_level[i].top = gi.sourceY;
    m_level[i].totalTime = 0;
    m_level[i].updateInterval = gi.updateInterval;
    m_level[i].vel.x = 0;
    m_level[i].vel.y = 0;
    m_level[i].turnRate = 0;
    m_level[i].direction = 0;

  }

  return true;
}

bool CGameData::AddObject(CGraphics &con, std::string objectName, int x, int y){
  GAME_OBJECT go;
  GRAPHIC_IMAGE gi;
  GAME_OBJECT_TYPE got = GetObjectData(objectName);
  if(got.objectID == -1)
    return false;

  m_level.push_back(go);
  int i = m_level.size() - 1;
  
  m_level[i].name = got.name;
  m_level[i].spriteID1 = got.spriteID1;
  m_level[i].spriteID2 = got.spriteID2;
  m_level[i].spriteID3 = got.spriteID3;

  m_level[i].spriteID =  m_level[i].spriteID1; //initial sprite
  m_level[i].behavior = got.behavior;
  m_level[i].layer = got.layer;
  m_level[i].objectID = got.objectID;
  
  gi = con.GetSprite(m_level[i].spriteID);
  m_level[i].alive = true;
  m_level[i].alpha = gi.alpha;
  m_level[i].angle = gi.angle;
  m_level[i].dying = false;
  m_level[i].frameCount = gi.frameCount;
  m_level[i].maxFrames = gi.frameMax;
  m_level[i].height = gi.height;
  m_level[i].width = gi.width;
  m_level[i].left = gi.sourceX;
  m_level[i].top = gi.sourceY;
  m_level[i].totalTime = 0;
  m_level[i].updateInterval = gi.updateInterval;
  m_level[i].vel.x = 0;
  m_level[i].vel.y = 0;
  m_level[i].pos.x = x;
  m_level[i].pos.y = y;
  m_level[i].turnRate = 0;
  m_level[i].direction = 0;

  return true;
}

void CGameData::LoadGameLevel(std::string filename){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  GAME_OBJECT temp;
  std::string value;
  std::string asset;
  std::string parameter;
  GAME_OBJECT_TYPE got;
  m_screenColorRed = 200;
  m_screenColorGreen = 200;
  m_screenColorBlue = 200;
  
  m_level.clear();

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){
    
    temp.pos.x = 0;
    temp.pos.y = 0;
    temp.spriteID = 0;
    asset = cfr.GetTerm(cfr.GetLineFromFile(i),1);

    //loads screen color
    if(asset == "back_color" && cfr.GetNumberOfTermsFromLine(i) == 4 ){
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//red
      m_screenColorRed = atoi(parameter.c_str());
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//green
      m_screenColorGreen = atoi(parameter.c_str());
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//blue
      m_screenColorBlue = atoi(parameter.c_str());      
      pLog->Log("Colors",m_screenColorRed, m_screenColorGreen, m_screenColorBlue);
    }

    //world boundary
    else if(asset == "world" && cfr.GetNumberOfTermsFromLine(i) == 5 ){
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//red
      m_worldLeft = atoi(parameter.c_str());
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//green
      m_worldTop = atoi(parameter.c_str());
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//blue
      m_worldRight = atoi(parameter.c_str());      
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);//blue
      m_worldBottom = atoi(parameter.c_str());      
      pLog->Log("World Boundary");
      pLog->Log( m_worldLeft, m_worldTop, m_worldRight, m_worldBottom);
    }

    //game objects
    else{
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//x
      temp.pos.x = atoi(parameter.c_str());
      parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//y
      temp.pos.y = atoi(parameter.c_str());    
      
      got = GetObjectData(asset);
      if(got.objectID > -1){
        temp.soundID = got.soundID;
        temp.spriteID1 = got.spriteID1;
        temp.spriteID2 = got.spriteID2;
        temp.spriteID3 = got.spriteID3;
        temp.name = got.name;
        temp.layer = got.layer;
        temp.behavior = got.behavior;
      }
      temp.spriteID = temp.spriteID1;
      temp.alive = true;
      m_level.push_back(temp);
    }
  }


  for(int i = 0; i < m_level.size(); ++i){
    //pLog->Log(m_level[i].name, m_level[i].pos.x, m_level[i].pos.y);
    /*
    pLog->Log("posX", m_level[i].x);
    pLog->Log("posY", m_level[i].y);
    pLog->Log("posZ", m_level[i].z);
    pLog->Log("rotX", m_level[i].rx);
    pLog->Log("rotY", m_level[i].ry);
    pLog->Log("rotZ", m_level[i].rz);*/
  }
  
}

int CGameData::LevelSize(){
    return m_level.size();
}

GAME_OBJECT CGameData::GetLevelData(int i){
  return m_level[i];
}


bool CGameData::LoadObjectFile(std::string filename){

  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return false; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  std::string parameter;
  GAME_OBJECT_TYPE got;

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){
    if(cfr.GetNumberOfTermsFromLine(i) > 1){
      if(cfr.GetNumberOfTermsFromLine(i) == 8){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 1);//object name
        got.name = parameter;
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//object id
        got.objectID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//layer id
        got.layer = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//behavior id
        got.behavior = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);//sound id
        got.soundID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 6);//sprite id1
        got.spriteID1 = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 7);//sprite id2
        got.spriteID2 = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 8);//sprite id3
        got.spriteID3 = atoi(parameter.c_str());
        m_objectType.push_back(got);
        pLog->Log("Object Added", got.name, got.spriteID1);
      }
    }
  }

  //LogObjects();

  return true;
}

void CGameData::ClearLevel(){
  m_level.clear();
}

void CGameData::SaveLevel (int level){
  std::string filename;
  if(level == 1)
    filename = "assets\\data\\level1.dat";
  else if(level == 2)
    filename = "assets\\data\\level2.dat";
  else if(level == 3)
    filename = "assets\\data\\level3.dat";
  else if(level == 4)
    filename = "assets\\data\\level4.dat";
  else
    return;

  std::string line;
  std::ostringstream oss;
  
  std::ofstream file(filename.c_str());
  if(file.is_open()){
    //add header
    file << "//Mario PAG" << std::endl;
    file << "//" + filename + " Created by the gc10_editor" << std::endl << std::endl;
    
    //game variables
    file << "//miscellaneous game variables" << std::endl;
    oss.str("");
    oss << "back_color" << ", " << m_screenColorRed << ", " << m_screenColorGreen << ", " << m_screenColorBlue << std::endl;
    file << oss.str();
    oss.str("");
    file << "world" << ", " << m_worldLeft  << ", " <<  m_worldTop  << ", " << m_worldRight  << ", " <<  m_worldBottom << std::endl;
    file << std::endl;

    //save level data
    file << "//game object data" << std::endl;
    for(int i = 0; i < m_level.size(); ++i)
      file << m_level[i].name << ", " << m_level[i].pos.x << ", " << m_level[i].pos.y << std::endl;

    file.close();
  }
}

void CGameData::OpenLevel (int level, CGraphics &con){
  CLog *pLog = CLog::Instance();

  if(level == 1)
    LoadGameLevel("assets\\data\\level1.dat");
  else if(level == 2)
    LoadGameLevel("assets\\data\\level2.dat");
  else if(level == 3)
    LoadGameLevel("assets\\data\\level3.dat");
  else if(level == 4)
    LoadGameLevel("assets\\data\\level4.dat");

  pLog->Log("Level loaded", level);

  AddGraphicDataToLevelData(con);  
}
