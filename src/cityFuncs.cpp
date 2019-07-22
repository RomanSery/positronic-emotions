#include "dxstdafx.h"
#include "cityFuncs.h"
#include "NewtonObjects.h"
#include "Renderer.h"


//render the city with normal map shader
void renderMeshWithBump(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						D3DXVECTOR3 lightPos)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();

	UINT passes;
	D3DXMATRIX matWorld; D3DXMatrixIdentity(&matWorld);
	D3DXMATRIX worldInverse; D3DXMatrixInverse(&worldInverse, NULL, &matWorld);
	D3DXMATRIX proj, viewProjection, mWorldViewProj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );

    D3DXMATRIX mView = Player::sPlayerInfo->matView;
    if(Player::sPlayerInfo->isInCar)
      mView = NewtonObjects::carManager->car->getCarControl()->getMatView();
	D3DXMatrixMultiply(&viewProjection, &mView, &proj);
	D3DXMatrixMultiply(&mWorldViewProj, &matWorld, &viewProjection);
	
	effect->SetTechnique("diffuseBump");
	effect->Begin(&passes, 0);
	effect->BeginPass(0);
	effect->SetMatrix("WorldViewProj", &mWorldViewProj);
	effect->SetMatrix("WorldI", &worldInverse);
    effect->SetMatrix("World", &matWorld);

	D3DXVECTOR4 lightArray[2];
	lightArray[0] = D3DXVECTOR4(lightPos, 0);
	lightArray[1] = D3DXVECTOR4(lightPos, 0);
	effect->SetVectorArray("LightPos", lightArray, 2);
    
	
	for(int i = 0; i < numMaterials; i++)
	{
		if(mesh->m_pMaterials[i].Diffuse.a != 1.0f)
		 continue;

		effect->SetTexture("diffuseTexture", mesh->m_pTextures[i]);
		if(mesh->m_pBumpTextures[i] == NULL)
		  effect->SetTexture("normalMap", mesh->m_pTextures[i]);
		else
		  effect->SetTexture("normalMap", mesh->m_pBumpTextures[i]);
		effect->CommitChanges();
    	m->DrawSubset(i); 
    }
	effect->EndPass();
///////////////////
	/*effect->BeginPass(1);
	effect->SetMatrix("WorldViewProj", &mWorldViewProj);
	effect->SetMatrix("WorldI", &worldInverse);
    effect->SetMatrix("World", &matWorld);
    effect->SetVector("LightPos", &D3DXVECTOR4(100,50,100, 0));
	for(int i = 0; i < numMaterials; i++)
	{
		if(mesh->m_pMaterials[i].Diffuse.a != 1.0f)
		 continue;

		effect->SetTexture("diffuseTexture", mesh->m_pTextures[i]);
		if(mesh->m_pBumpTextures[i] == NULL)
		  effect->SetTexture("normalMap", mesh->m_pTextures[i]);
		else
		  effect->SetTexture("normalMap", mesh->m_pBumpTextures[i]);
		effect->CommitChanges();
    	m->DrawSubset(i); 
    }
	effect->EndPass();*/
	effect->End();

}

//render glow lines ALPHA=80
void renderGlowLines(IDirect3DDevice9* device, CDXUTMesh *mesh)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();
	
	for(int i = 0; i < numMaterials; i++)
	{
		if(mesh->m_pMaterials[i].Diffuse.a == 1.0f || mesh->m_pMaterials[i].Diffuse.a < 0.6f)
        	continue;
		
		device->SetMaterial(&mesh->m_pMaterials[i]);
		device->SetTexture(0, mesh->m_pTextures[i]);
    	m->DrawSubset(i); 
	}
	
}
//render lit windows ALPHA=50
void renderLitWindows(IDirect3DDevice9* device, CDXUTMesh *mesh)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();
	
	for(int i = 0; i < numMaterials; i++)
	{
		if(mesh->m_pMaterials[i].Diffuse.a != 0.5f) 
			continue;
		
		device->SetMaterial(&mesh->m_pMaterials[i]);
		device->SetTexture(0, mesh->m_pTextures[i]);
    	m->DrawSubset(i); 
	}
	
}

///render unlit windows ALPHA=0
void renderUnLitWindows(IDirect3DDevice9* device, ID3DXEffect *effect, CDXUTMesh *mesh,
						IDirect3DCubeTexture9 *cubeTex)
{
	int numMaterials = mesh->m_dwNumMaterials;
    LPD3DXMESH m = mesh->GetLocalMesh();
    D3DXMATRIX mWorld; D3DXMatrixIdentity(&mWorld);
	D3DXMATRIX proj; D3DXMatrixIdentity(&proj);
    D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );
    
	UINT passes, iPass = 0;
    effect->SetTechnique("RenderWindow");
	effect->SetMatrix("World", &mWorld);
	effect->SetMatrix("Projection", &proj);
    if(Player::sPlayerInfo->isInCar)
      effect->SetMatrix("View", &NewtonObjects::carManager->car->getCarControl()->getMatView());
    else
      effect->SetMatrix("View", &Player::sPlayerInfo->matView);
	effect->SetVector("CameraPos", &D3DXVECTOR4(Player::sPlayerInfo->eyePos, 1));
			
	effect->Begin(&passes, 0);
	effect->BeginPass(iPass);
		for(int i = 0; i < numMaterials; i++){
		  if(mesh->m_pMaterials[i].Diffuse.a != 0.0f) 
			continue;		  
 
          effect->SetTexture("EnvironmentMap", cubeTex);
          effect->SetTexture("BaseMap", mesh->m_pTextures[i]);
		  effect->CommitChanges();
			 
		  m->DrawSubset(i); 
		} 
        effect->EndPass();
        
	
	effect->End();        
	
}

//render parts of mesh with alpha values 0 < a < 1
void renderWindows(IDirect3DDevice9* device, CDXUTMesh *mesh)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();
	
	for(int i = 0; i < numMaterials; i++)
	{
		mesh->m_pMaterials[i].Ambient = mesh->m_pMaterials[i].Diffuse;
        if((mesh->m_pMaterials[i].Diffuse.a == 0.0 || mesh->m_pMaterials[i].Diffuse.a == 1.0)
			&& Renderer::SUPPORTS_SHADERS_1_1 == true)
			continue;

		device->SetMaterial(&mesh->m_pMaterials[i]);
		device->SetTexture(0, mesh->m_pTextures[i]);
    	m->DrawSubset(i); 
	}
	
}

void renderStreetLights(IDirect3DDevice9* device, CDXUTMesh *mesh)
{
	int numMaterials = mesh->m_dwNumMaterials;
	LPD3DXMESH m = mesh->GetLocalMesh();
	
	for(int i = 0; i < numMaterials; i++)
	{
	    if(mesh->m_pMaterials[i].Diffuse.a != 1.0)
			continue;
		device->SetMaterial(&mesh->m_pMaterials[i]);
		device->SetTexture(0, mesh->m_pTextures[i]);
    	m->DrawSubset(i); 
	}
}

void renderLightHalo(IDirect3DDevice9* device, CDXUTMesh *mesh, IDirect3DTexture9 *tex)
{
	device->SetTexture(0, tex);
    mesh->GetLocalMesh()->DrawSubset(0); 
}

void renderWithNoBump(IDirect3DDevice9* device, CDXUTMesh *mesh)
{
  int numMaterials = mesh->m_dwNumMaterials;
  LPD3DXMESH m = mesh->GetLocalMesh();

  for(int i = 0; i < numMaterials; i++)
	{
	    if(mesh->m_pMaterials[i].Diffuse.a != 1.0)
			continue;
		device->SetMaterial(&mesh->m_pMaterials[i]);
		device->SetTexture(0, mesh->m_pTextures[i]);
    	m->DrawSubset(i); 
	}

}
