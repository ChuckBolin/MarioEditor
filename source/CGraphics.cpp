#include "../include/CGraphics.h"

//skybox vertex
struct Vertex3
{
	Vertex3(){}
	Vertex3(
		float x, float y, float z,
		//float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		//_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
    float _x, _y, _z;
    //float _nx, _ny, _nz;
    float _u, _v; // texture coordinates

	static const DWORD SKYFVF;
};
const DWORD Vertex3::SKYFVF = D3DFVF_XYZ |  D3DFVF_TEX1;//D3DFVF_NORMAL | D3DFVF_TEX1;

/*******************************************************************
  CGraphics constructor
*******************************************************************/
CGraphics::CGraphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen){
  m_hWnd = hWnd;
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_fullScreen = fullScreen;
}

/*******************************************************************
  Initialize DirectX.
*******************************************************************/
bool CGraphics::InitializeDirectX(){
  m_d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
  if(!m_d3d){
    ::MessageBox(m_hWnd, "DirectX failed!", "Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return false;
  }

  //D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information
  ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));    // clear out the struct for use
	D3DDISPLAYMODE        CurrentMode;
  
  // Select back buffer format etc
	m_d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &CurrentMode);

  //controlled by config.txt
  m_d3dpp.Windowed = !m_fullScreen;  //windowed
  m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
  m_d3dpp.BackBufferWidth = m_screenWidth;    // set the width of the buffer
  m_d3dpp.BackBufferHeight = m_screenHeight;    // set the height of the buffer
  m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
  m_d3dpp.hDeviceWindow = m_hWnd;    // set the window to be used by Direct3D
  m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
  m_d3dpp.FullScreen_RefreshRateInHz = m_fullScreen ? CurrentMode.RefreshRate : 0;
  m_d3dpp.EnableAutoDepthStencil = TRUE;
 // m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  m_d3dpp.AutoDepthStencilFormat		=  FindDepthStencilFormat( D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_HAL );

  // create a device class using this information and the info from the d3dpp stuct
  m_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hWnd,
                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_d3ddev);
  
  if(FAILED( D3DXCreateSprite(m_d3ddev, &m_pSprite)) )
  {
    ::MessageBox(m_hWnd, "Failed to create a sprite!", "Fatal Error!", 0);//MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return false;
  }

  ReloadLost();

  return true;
}

/*******************************************************************
  Clean up DirectX
*******************************************************************/
void CGraphics::CleanupDirectX(){
  CLog *pLog = CLog::Instance();
  pLog->Log("Cleanup");

  DeleteLost();
  pLog->Log("cleanup 1");
  m_d3ddev->Release(); // close and release the 3D device
  m_d3d->Release(); // close and release Direct3D
  
  pLog->Log("cleanup 2");
    return;
}

/*******************************************************************
  Define all fonts
*******************************************************************/
void CGraphics::InitializeFonts(){

  //load 12 different fonts for Arial and Verdana, 12-16, normal and bold
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16B] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16] );    
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16B] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20B] );

}

void CGraphics::ReloadLost(){
  CLog *pLog = CLog::Instance();
  pLog->Log("ReloadLost", "1");
  InitializeFonts();
  pLog->Log("ReloadLost", "2");
  


//  LoadSkybox();
//  LoadGround();
  pLog->Log("ReloadLost", "3");
  SetupStuff();
  pLog->Log("ReloadLost", "4");

  //lighting stuff
  m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
  D3DLIGHT9 dir;
  ::ZeroMemory(&dir, sizeof(dir));
  dir.Type = D3DLIGHT_DIRECTIONAL;
  dir.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
  dir.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.3f;
  dir.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 2.6f;
  dir.Direction = D3DXVECTOR3(0.0f, -100.0f, 0.0f);
/*
	m_mat[0].Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	m_mat[0].Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	m_mat[0].Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	m_mat[0].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_mat[0].Power    = 5.0f;

  m_mat[1].Ambient  = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
  m_mat[1].Diffuse  = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	m_mat[1].Specular = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	m_mat[1].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_mat[1].Power    = 5.0f;

	m_mat[2].Ambient  = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	m_mat[2].Diffuse  = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	m_mat[2].Specular = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	m_mat[2].Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_mat[2].Power    = 5.0f;
*/
//  m_d3ddev->SetMaterial(&mtrl);

  m_d3ddev->SetLight(0, &dir);
  m_d3ddev->LightEnable(0, true);
  m_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
  m_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void CGraphics::DeleteLost(){
  CLog *pLog = CLog::Instance();

  //font stuff
//  m_skybox = NULL;
//  m_groundV = NULL;

  for(int i = 0; i < C_NUMBER_FONTS; i++)
    m_Font[i] = NULL; 

  m_assetSpriteData.empty();// .clear();
  m_assetFileData.empty();// .erase( .erase( );//clear();
  m_assetMeshData.empty();//clear();
}

void CGraphics::SetupStuff(){
  CLog *pLog = CLog::Instance();
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
			                       D3DX_PI * 0.25f, // 90 - degree
			                       (float)m_screenWidth / (float)m_screenHeight,                              
			                       0.1f,
			                       800.0f);
  
	m_d3ddev->SetTransform(D3DTS_PROJECTION, &proj);
  m_d3ddev->SetRenderState(D3DRS_ZENABLE, true);
  //m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//D3DFILL_SOLID);// D3DFILL_WIREFRAME);
}

//*****************************************************************************
// Renders a sprite based upon graphic image sprite data
//*****************************************************************************
void CGraphics::RenderGraphicModulate(GRAPHIC_IMAGE gi, int positionX, int positionY, int red, int green, int blue){
  //CLog *pLog = CLog::Instance();
  if(gi.bValid == false)
    return;

  RECT rect;
  if(gi.frameMax > 1){//animated frames
    rect.left = gi.sourceX + (gi.frameCount * gi.width);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  else{ //static frame
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}

void CGraphics::RenderGraphicModulate(GAME_OBJECT go, int positionX, int positionY, int red, int green, int blue){
  GRAPHIC_IMAGE gi;  
  gi = GetSprite(go.spriteID);
  if(gi.bValid == false)
    return;

  RECT rect;
  if(go.maxFrames > 1){//animated frames
    rect.left = gi.sourceX + (go.frameCount * gi.width);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  else{ //static frame
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }

  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  pos.x = positionX;//go.pos.x;
  pos.y = positionY;//go.pos.y;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, go.direction, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}

void CGraphics::RenderGraphicModulate(GAME_OBJECT go, int red, int green, int blue){
  GRAPHIC_IMAGE gi;  
  gi = GetSprite(go.spriteID);
  if(gi.bValid == false)
    return;

  RECT rect;
  if(go.maxFrames > 1){//animated frames
    rect.left = gi.sourceX + (go.frameCount * gi.width);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  else{ //static frame
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }

  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  D3DXVECTOR2 pos;
  pos.x = go.pos.x;
  pos.y = go.pos.y;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2;
  rotcenter.y = (rect.bottom - rect.top)/2;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, go.direction, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
}



/*****************************************************
 Reads and loads data from assets.dat file into one of
 the vectors below.

 std::vector<GRAPHIC_FILE_DATA> m_assetFileData;
 std::vector<SPRITE_DATA> m_assetSpriteData;
 std::vector<MESH_DATA> m_assetMeshData;
 std::vector<SOUND_FILE_DATA> m_assetSoundData;

 Parameters in cfr begin with 1 not 0
*****************************************************/
bool CGraphics::LoadAssetFile(std::string filename){
  CLog *pLog = CLog::Instance();
  
  //read assets.dat file
  CFileReader cfr(filename);
  if(cfr.IsValid() == false)
    return false; //file is invalid or contains no usable data

  //validate asset data and add to appropriate vectors
  std::string parameter;
  std::string assetType;
  GRAPHIC_FILE_DATA   tempGraphic;
  SPRITE_DATA         tempSprite;
  MESH_DATA           tempMesh;

  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){

    if(cfr.GetNumberOfTermsFromLine(i) > 1){
      assetType = cfr.GetTerm(cfr.GetLineFromFile(i), 1);
      
      if(assetType == "texture" && cfr.GetNumberOfTermsFromLine(i) == 4){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//fileID
        tempGraphic.fileID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//filename
        tempGraphic.filename = parameter;
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//description
        tempGraphic.description = parameter;
        m_assetFileData.push_back(tempGraphic);
      }

      else if(assetType == "sprite" && cfr.GetNumberOfTermsFromLine(i) == 14){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//spriteID
        tempSprite.spriteID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//fileID
        tempSprite.fileID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//left
        tempSprite.x = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 5);//right
        tempSprite.y = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 6);//width
        tempSprite.width = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 7);//height
        tempSprite.height = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 8);//scale
        tempSprite.scale =  atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 9);//angle
        tempSprite.angle =  atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 10);//alpha
        tempSprite.alpha = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 11);//total number of frames
        tempSprite.numberFrames = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 12);//framecount, starting frame
        tempSprite.frameCount = atoi(parameter.c_str());        
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 13);//time between frames
        tempSprite.timeBetweenFrames = atof(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 14);//description
        tempSprite.description = parameter;
        m_assetSpriteData.push_back(tempSprite);
      }

/*
      else if(assetType == "soundfile" && cfr.GetNumberOfTermsFromLine(i) == 4){
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 2);//sound ID
        tempSound.soundID = atoi(parameter.c_str());
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 3);//filename
        tempSound.filename = parameter;
        parameter = cfr.GetTerm(cfr.GetLineFromFile(i), 4);//description
        tempSound.description = parameter;
        m_assetSoundData.push_back(tempSound);
      }*/
    }
  }

  //log results of load
  pLog->Log("Number of graphic files",  m_assetFileData.size());
  pLog->Log("Number of sprites",        m_assetSpriteData.size());
  pLog->Log("Number of meshes",         m_assetMeshData.size());
  //pLog->Log("Number of sounds",         m_assetSoundData.size());

  //load graphics files
  for(int i = 0; i < m_assetFileData.size(); ++i){
    //pLog->Log("******* graphic file added *******");
    pLog->Log("graphic file", m_assetFileData[i].filename);
    LoadGraphicFile(m_assetFileData[i].fileID, "assets//graphics//" + m_assetFileData[i].filename);
  }

  //load sprite information
  GRAPHIC_IMAGE gi;
  for(int i = 0; i < m_assetSpriteData.size(); ++i){
    gi.spriteID = m_assetSpriteData[i].spriteID;
    gi.fileID = m_assetSpriteData[i].fileID;
    gi.sourceX = m_assetSpriteData[i].x;
    gi.sourceY = m_assetSpriteData[i].y;
    gi.width = m_assetSpriteData[i].width;
    gi.height = m_assetSpriteData[i].height;
    gi.scale = m_assetSpriteData[i].scale;    
    gi.alpha = m_assetSpriteData[i].alpha;
    gi.angle = m_assetSpriteData[i].angle;
    gi.frameMax = m_assetSpriteData[i].numberFrames;
    gi.updateInterval = m_assetSpriteData[i].timeBetweenFrames;
    gi.frameCount = 0;
    
    //ensure the fileID really exists
    gi.bValid = false; //assume file is invalid
    for(int j = 0; j < m_assetFileData.size(); ++j){
      if(gi.fileID == m_assetFileData[j].fileID){
        gi.bValid = true;
        m_sprites.push_back(gi);
        break;
      }
    }
  }


  return true;
}

/****************************************************
 Loads texture files and skins
****************************************************/
void CGraphics::LoadGraphicFile(int fileID, std::string filename){
  S_TEXTURE temp;

  CLog *pLog = CLog::Instance();
  HRESULT hr = D3DXCreateTextureFromFile(m_d3ddev,
                                         filename.c_str(),
                                         &temp.pBuffer);

  if(FAILED(hr)){
    pLog->Log("Failed to load graphic file", filename);
    return;
  }
  pLog->Log("Success....................", filename);
  temp.id = fileID;
  m_texture.push_back(temp);
}

/****************************************************
 Returns pointer to texture graphic...using file ID
****************************************************/
IDirect3DTexture9* CGraphics::GetTexture(int id){
  
  for (int i = 0; i < m_texture.size(); ++i){
    if(m_texture[i].id == id){
      return m_texture[i].pBuffer;
    }
  }
  return NULL;
}

/****************************************************
 Returns sprite data based upon sprite ID
****************************************************/
GRAPHIC_IMAGE CGraphics::GetSprite(int id){
  for(int i = 0; i < m_sprites.size(); ++i){
    if(m_sprites[i].spriteID == id)
      return m_sprites[i];
  }
  GRAPHIC_IMAGE gi;
  gi.bValid = false;
  return gi;
}

//-----------------------------------------------------------------------------
// Name : FindDepthStencilFormat ()		(private)
// Desc : This function simply determines the best depth format that is
//        available for the specified mode.
// Note : No tests for stencil active depth buffers are made.
//-----------------------------------------------------------------------------
D3DFORMAT CGraphics::FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType )
{

    // Test for 24 bith depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D32 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D32 ))) return D3DFMT_D32;
    
    } // End if 32bpp Available

    // Test for 24 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D24X8 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D24X8 ))) return D3DFMT_D24X8;
    
    } // End if 24bpp Available

    // Test for 16 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D16 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D16 ))) return D3DFMT_D16;
    
    } // End if 16bpp Available

    // No depth buffer supported
    return D3DFMT_UNKNOWN;

}


void CGraphics::DrawLine(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 2;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1-1 ;
  line[0].y = y1 ;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2-1;
  line[1].y = y2;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);


  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &line[0],sizeof(line[0]));

}


void CGraphics::DrawRect(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 5;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1;
  line[0].y = y1;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2;
  line[1].y = y1;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);

  line[2].x = x2;
  line[2].y = y2;
  line[2].z = 0;
  line[2].rhw = 1.0f;
  line[2].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[3].x = x1;
  line[3].y = y2;
  line[3].z = 0;
  line[3].rhw = 1.0f;
  line[3].color = D3DCOLOR_XRGB(red, green, blue);

  line[4].x = x1;
  line[4].y = y1;
  line[4].z = 0;
  line[4].rhw = 1.0f;
  line[4].color = D3DCOLOR_XRGB(red, green, blue);

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &line[0],sizeof(line[0]));

}

//draws circle
void CGraphics::DrawCircle(float x, float y, int red, int green, int blue, float radius){

  const int NUMPOINTS = 24;//24
  const float PI = 3.14159;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex circle[NUMPOINTS + 1];
  const DWORD line_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  
  float theta;
  float wedgeAngle;
  wedgeAngle = (float)((2 * PI)/NUMPOINTS);
  for (int i = 0; i <= NUMPOINTS; i++){
    theta = i * wedgeAngle;
    circle[i].x = (float)(x + radius * cos(theta));
    circle[i].y = (float)(y - radius * sin(theta));
    circle[i].z = 0;
    circle[i].rhw = 1.0f;
    circle[i].color = D3DCOLOR_XRGB(red, green, blue);
  }

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &circle[0],sizeof(circle[0]));
  
}