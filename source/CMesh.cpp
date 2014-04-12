#include "..\include\CMesh.h"

CMesh::CMesh(){

}

CMesh::~CMesh(){
 m_mesh->Release();
 //m_pD3D->Release();

}

bool CMesh::LoadMesh(LPDIRECT3DDEVICE9 pD3D, std::string sFilename){
  CLog *pLog = CLog::Instance();
  pLog->Log("Mesh 1");
  
  LPD3DXBUFFER bufMaterial;
  HRESULT hr = 0;
  pLog->Log("Mesh 1.01");

  hr = D3DXLoadMeshFromX(sFilename.c_str(),
                         D3DXMESH_SYSTEMMEM,
                         pD3D, 
                         NULL,
                         &bufMaterial, 
                         NULL, 
                         &m_numMaterials, 
                         &m_mesh);
  
  if(FAILED(hr)){
    m_bValidMesh = false;
    pLog->Log("Mesh 1.1");
    return false;
  }

  pLog->Log("Mesh 2");
  m_bValidMesh = true;
  m_pD3D = pD3D;
  m_filename = sFilename;
  pLog->Log("Mesh 3");
  D3DXMATERIAL* tempMaterial = (D3DXMATERIAL*)bufMaterial->GetBufferPointer();

  pLog->Log("Mesh 4");
  m_material = new D3DMATERIAL9[m_numMaterials];
  m_texture = new LPDIRECT3DTEXTURE9[m_numMaterials];

  for(DWORD i = 0; i < m_numMaterials; ++i){
    m_material[i] = tempMaterial[i].MatD3D;
    m_material[i].Ambient = m_material[i].Diffuse;

    if(FAILED(D3DXCreateTextureFromFile(
      pD3D, tempMaterial[i].pTextureFilename,
      &m_texture[i])))
      m_texture[i] = NULL;
  }
pLog->Log("Mesh 10");
  return true;
}


void CMesh::Reset(){
  //delete m_material;
  //delete m_texture;
  //delete m_mesh;
}

//render mesh
void CMesh::Render(){
  if(m_bValidMesh == false)
    return;

  for(DWORD i = 0; i < m_numMaterials;++i){
    m_pD3D->SetMaterial(&m_material[i]);
    if(m_texture[i] != NULL)
      m_pD3D->SetTexture(0, m_texture[i]);

    m_mesh->DrawSubset(i);
  }
}

