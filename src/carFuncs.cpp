#include "dxstdafx.h"
#include "carFuncs.h"
#include "Player.h"

void renderCarWithShader(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						  D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX proj,
						  D3DXVECTOR3 camPos, IDirect3DCubeTexture9 *cTex)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();

	UINT passes = 2, iPass;
    float offset = 0;
	//ALPHA VALUES: car body & windows = 100  lines,lights = 0
	float aValues[3] = {1.0, 0.0};
	effect->SetTechnique("RenderCarBody");
	effect->SetMatrix("World", &mWorld);
	effect->SetMatrix("View", &mView);
	effect->SetMatrix("Projection", &proj);
	effect->SetTexture("cubeTexture", cTex);
	effect->SetVector("CameraPos", &D3DXVECTOR4(camPos, 1));
	effect->SetVector("lightPos", &D3DXVECTOR4(Player::sPlayerInfo->lightPos, 1));
	
	effect->Begin(&passes, 0);
	for(iPass = 0; iPass < passes; iPass++)
	{
		effect->BeginPass(iPass);
		for(int i = 0; i < numMaterials; i++)
		{
		  if(mesh->m_pMaterials[i].Diffuse.a != aValues[iPass])
              continue;
		  
		  effect->SetTexture("baseTex", mesh->m_pTextures[i]);
		  effect->CommitChanges();
			 
		  m->DrawSubset(i); 
		} 
        effect->EndPass();
        
	}
	effect->End();
}

void renderWheelsWithShader(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
				     D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX proj, D3DXVECTOR3 camPos)
{
    int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();

	UINT passes;
    D3DXVECTOR4 lightV(50, 25, 50, 1);
	D3DXVec4Normalize(&lightV, &lightV);
	
	effect->SetTechnique("RenderCarWheels");
	effect->SetMatrix("World", &mWorld);
	effect->SetMatrix("View", &mView);
	effect->SetMatrix("Projection", &proj);
	D3DXVECTOR4 v4(camPos, 1);
	effect->SetVector("CameraPos", &v4);
	effect->SetVector("lightPos", &lightV);
	
	effect->Begin(&passes, 0);
	effect->BeginPass(0);
	for(int i = 0; i < numMaterials; i++)
	{
	  effect->SetTexture("baseTex", mesh->m_pTextures[i]);
	  effect->CommitChanges();
			 
	  m->DrawSubset(i); 
	} 
    effect->EndPass();
    effect->End();

}

void renderWithoutShader(IDirect3DDevice9* device, CDXUTMesh *mesh, D3DXMATRIX mWorld)
{
  //set world matrix and render mesh
  device->SetTransform(D3DTS_WORLD, &mWorld);
  mesh->Render(device);
  
  //reset world matrix
  D3DXMATRIX mIdentity;
  D3DXMatrixIdentity(&mIdentity);
  device->SetTransform(D3DTS_WORLD, &mIdentity);

}