#include "dxstdafx.h"
#include ".\lights.h"
#include "Player.h"
#include "cityFuncs.h"

Lights::Lights(void)
{
 light = new CDXUTMesh(L"meshes/light.x"); 

 streetLights[0] = sLight( D3DXVECTOR3(13, 56, 78) );
 streetLights[1] = sLight( D3DXVECTOR3(-50, 50, -50));
 streetLights[2] = sLight( D3DXVECTOR3(210, 80, 10) );
 streetLights[3] = sLight( D3DXVECTOR3(0, 50, 100)  );
 streetLights[4] = sLight( D3DXVECTOR3(300, 100, 10));

 numBigLights = 1;
 numSmallLights = 0;
}

void Lights::initDeviceObjects(IDirect3DDevice9* device)
{
 light->Create(device, L"meshes/light.x");
 D3DXCreateTextureFromFileEx(device, L"Media/particle.jpg",0,0,D3DX_FROM_FILE,
							  0, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_DEFAULT,
							  D3DX_DEFAULT, D3DCOLOR_ARGB(255,0,0,0), NULL, NULL, &texture);

 device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

}

void Lights::restoreDeviceObjects(IDirect3DDevice9* device)
{
 light->RestoreDeviceObjects(device);
}

void Lights::frameMove()
{ 
  if(DXUTIsKeyDown(VK_RIGHT))
    streetLights[0].pos.z -= 3;
  else if(DXUTIsKeyDown(VK_LEFT))
    streetLights[0].pos.z += 3;
  else if(DXUTIsKeyDown(VK_UP))
    streetLights[0].pos.x += 3;
  else if(DXUTIsKeyDown(VK_DOWN))
    streetLights[0].pos.x -= 3;

}
void Lights::renderLights(IDirect3DDevice9* device)
{
  D3DXMATRIX matLight, matTrans, matBillboard;
  D3DXMatrixIdentity(&matTrans);
  
  //start alpha blending and turn off writing to depth buffer
  device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  device->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
  
  //construct a billboard matrix for the light
  D3DXVECTOR3 vDir = Player::sPlayerInfo->lookAt - Player::sPlayerInfo->eyePos;
  if( vDir.x > 0.0f )
   D3DXMatrixRotationY( &matBillboard, -atanf(vDir.z/vDir.x)+D3DX_PI/2 );
  else
   D3DXMatrixRotationY( &matBillboard, -atanf(vDir.z/vDir.x)-D3DX_PI/2 );
  
  
  //draw the street lights
  for(int i = 0; i < numBigLights; i++)
  {
   sLight l = streetLights[i];
   D3DXMatrixTranslation(&matTrans, l.pos.x, l.pos.y, l.pos.z);
   D3DXMatrixMultiply(&matLight, &matBillboard, &matTrans);
   device->SetTransform(D3DTS_WORLD,&matLight);
   //light->Render(device);
   renderLightHalo(device, light, texture);
   
   //reset matrices
   D3DXMatrixIdentity(&matTrans);
   D3DXMatrixIdentity(&matLight);
  }
  
  //draw the small lights
  for(int i = 0; i < numSmallLights; i++)
  {
   sLight l = smallLights[i];
   D3DXMatrixTranslation(&matTrans, l.pos.x, l.pos.y, l.pos.z);
   D3DXMatrixMultiply(&matLight, &matBillboard, &matTrans);
   device->SetTransform(D3DTS_WORLD,&matLight);
   light->Render(device);//diffrent
   
   //reset matrices
   D3DXMatrixIdentity(&matTrans);
   D3DXMatrixIdentity(&matLight);
  }
  
  //end alpha blending and turn on writing to depth buffer
  device->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
  device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

D3DXVECTOR3 Lights::getClosestLight(D3DXVECTOR3 playerPos)
{
 sLight light = streetLights[0];
 double dist = 0;
 int index = 0;
 D3DXVECTOR3 distVec = light.pos.operator -(playerPos);
 double closest = D3DXVec3Length(&distVec);
 
 for(int i = 1; i < numBigLights; i++){
  light = streetLights[i];
  
  distVec = light.pos.operator -(playerPos);
  dist = D3DXVec3Length(&distVec);
  
  if(dist < closest){
   closest = dist;
   index = i;
  }
 }
 light = streetLights[index];
 return light.pos;
}

void Lights::destroyDeviceObjects()
{
	light->Destroy();
	SAFE_RELEASE(texture);
}

void Lights::invalidateDeviceObjects()
{
	light->InvalidateDeviceObjects();
}

