//CMesh.h
#ifndef CMESH_H
#define CMESH_H

#include <string>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "CLog.h"

class CMesh{
public:
  CMesh();
  ~CMesh();
  bool LoadMesh(LPDIRECT3DDEVICE9 pD3D, std::string sFilename);
  //bool IsValid(){return m_bValidMesh;};
  void Reset();
  void Render();

private:
  bool m_bValidMesh;
  LPD3DXMESH m_mesh;
  D3DMATERIAL9* m_material;
  LPDIRECT3DTEXTURE9* m_texture;  
  LPDIRECT3DDEVICE9 m_pD3D;
  DWORD m_numMaterials;
  std::string m_filename;
};

#endif