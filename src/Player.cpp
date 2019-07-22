#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "Dinput8.lib")
#include "dxstdafx.h"
#include ".\player.h"
#include <assert.h>
#include "vector.h"
#include <stdio.h>
#include <dinput.h>
#include "collision.h"
#include "Frustum.h"
#include "PrintText.h"

playerInfo* Player::sPlayerInfo;


Player::Player()
{
  sPlayerInfo = new playerInfo();
  
  pControl = new PlayerControl();
  frustum = new cFrustum();
   
}

void Player::transformToHuman()
{
  //change the animation set to human
  PlayerControl::transformHuman();
  PlayerControl::currSet = 1;
  sPlayerInfo->isHuman = true;  

}

void Player::setInputControls(bool m, bool e, bool moveM)
{
  PlayerControl::canMove = m;
  PlayerControl::canEnterLeave = e;
  PlayerControl::canMoveMouse = moveM;  
}


HRESULT Player::FrameMove(float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice, D3DXMATRIX carView)
{
  if(sPlayerInfo->isInCar == false)
   frustum->Construct(m_pd3dDevice, 1000000.0f, sPlayerInfo->matView);
  else
   frustum->Construct(m_pd3dDevice, 1000000.0f, carView);
  
  
  pControl->FrameMove(sPlayerInfo, m_fElapsedTime, m_pd3dDevice);
  pControl->readOtherControls(sPlayerInfo);
  

 return S_OK;
}

HRESULT Player::InitDeviceObjects(LPDIRECT3DDEVICE9 m_pd3dDevice)
{
  pControl->InitDeviceObjects(m_pd3dDevice);    
  
  return S_OK;
}


HRESULT Player::Render(LPDIRECT3DDEVICE9 m_pd3dDevice, bool useTextures,IDirect3DCubeTexture9 *cubeTexture)
{
   if(sPlayerInfo->isInCar == false){
     int code;
     if(useTextures)
       sPlayerInfo->isHuman ? code = HUMAN_FORM_CODE : code = ROBOT_FORM_CODE;
     else
       code = NPC_CODE;
     pControl->getXFileEntity()->Render(m_pd3dDevice, code, cubeTexture);
   }
   

   renderText();
   return S_OK;
}

void Player::renderText()
{
  /*if(sPlayerInfo->isInCar == true)
   printString("In car", 200, 10);
  else
   printString("Not In car", 200, 10);
*/
  //printNum(sPlayerInfo->facing.x, 300, 10);
  //printNum(sPlayerInfo->facing.y, 400, 10);
  //printNum(sPlayerInfo->facing.z, 500, 10);

  //printNum(sPlayerInfo->pos._41, 300, 30);
  //printNum(sPlayerInfo->pos._42, 400, 30);
  //printNum(sPlayerInfo->pos._43, 500, 30);
  
/*
  if(sPlayerInfo->isInReachOfCar == true && sPlayerInfo->isInCar == false)
   printString("Press enter to get into car", 500, 600);
*/
 //printNum(sPlayerInfo->interiorID, 10, 10);
 //if(sPlayerInfo->isOnMission) PrintText::printString("on mission", 500, 500);

}
HRESULT Player::RestoreDeviceObjects(LPDIRECT3DDEVICE9 m_pd3dDevice)
{
 pControl->RestoreDeviceObjects(m_pd3dDevice); 
 return S_OK;
}


void Player::invalidateDeviceObjects(){
	pControl->invalidateDeviceObjects();
	
}
void Player::destroyDeviceObjects()
{
	pControl->destroyDeviceObjects();
	
}
