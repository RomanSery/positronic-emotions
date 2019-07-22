#include "dxstdafx.h"
#include ".\playercontrol.h"
#include "collision.h"
#include "NPCManager.h"
#include "CityManager.h"


#define ROBOT_ID 0
#define HUMAN_ID 1

int PlayerControl::currSet = 0;//0-terminator
int PlayerControl::WALK_ID;
int PlayerControl::IDLE_ID;
bool PlayerControl::canMove = true;
bool PlayerControl::canMoveMouse = true;
bool PlayerControl::canEnterLeave = true;
bool PlayerControl::isMouseDown = false;

PlayerControl::PlayerControl(void)
{
  cam = new CCamera();
  facing = 0.0f;
  pos.set(0,0,0);
  isY = false;
  WALK_ID = 0; IDLE_ID = 1;
  m_currentAnimation = IDLE_ID;
}

void PlayerControl::readKeyboardInput(playerInfo *sPlayerInfo)
{
  
 CVector vel;
 CPos eye; // Used to hold camera's eye

 
 if( canMove && (DXUTIsKeyDown('W') == TRUE && DXUTIsKeyDown(VK_LCONTROL) == FALSE) || sPlayerInfo->isInCar){
	sPlayerInfo->isMoving = true;
		  
	if(getAnimationStatus() != WALK_ID){
	  setAnimationStatus(WALK_ID);
	  m_xFileEntity[currSet]->setAnimationSpeed(0.2f);
	}
	eye = cam->getEye();
	    
	// Move the camera's eye in the direction of the camera's forward vector
	pos.set(sPlayerInfo->pos._41,sPlayerInfo->pos._42,sPlayerInfo->pos._43);
	vel.set(sPlayerInfo->velocity.x,sPlayerInfo->velocity.y,sPlayerInfo->velocity.z);
	vel.scale(1 / DXUTGetFPS());
	  

	CPos distance = pos.operator -(eye + vel); //distance between camera eye and lookAt pt
	if(distance.magnitude() > kCamDist)
		eye += vel;
	  
	  
	cam->setEye(eye); //set the camera eye
	pos.y += 10;
	cam->setTarget(pos); // Set the camera to look at the player
 
 }
 //player not moving
 else if(DXUTIsKeyDown('W') == FALSE && getAnimationStatus() != IDLE_ID){
    sPlayerInfo->isMoving = false;
    m_xFileEntity[currSet]->setAnimationSpeed(1.0f);
    setAnimationStatus(IDLE_ID);
 }
   
}


void PlayerControl::readMouseInput(float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice,
								   playerInfo *sPlayerInfo)
{
 mouse->GetDeviceState(sizeof(DIMOUSESTATE), &state);
 long xPos = state.lX * 20;
 long yPos = state.lY * 20;
 (state.rgbButtons[0] & 0x80) ? isMouseDown = true : isMouseDown = false;
 
 if(canMoveMouse && sPlayerInfo->isInCar == false){
 if(state.lX > 0)
 {
   // Calculate amount of rotation based on cursor position
   float amt = (1.0f - (float)(kWinWid - xPos) / (float)kWinWid) * kTurnAmt;
   cam->rotateY(D3DXToRadian(amt), pos);
   
   if(DXUTIsKeyDown(VK_LCONTROL) == FALSE){
    facing += amt;
    if(facing >= 360)
     facing = 0;
   }
      
   sPlayerInfo->isTurning = true;
 }
 else if(state.lX < 0)
 {
   // Calculate amount of rotation based on cursor position
   float amt = (1.0f - (float)(kWinWid + xPos) / (float)kWinWid) * kTurnAmt;
   cam->rotateY(D3DXToRadian(-amt), pos);
   
   if(DXUTIsKeyDown(VK_LCONTROL) == FALSE){
    if(facing <= 0)
     facing = 360;
    facing -= amt;
   }

   sPlayerInfo->isTurning = true;
 }
 
 if(state.lY > 0 ) // If we've rotated up
 {
   // Calculate amount of rotation 
   float amt = (1.0f - (float)(kWinHgt - yPos) / (float)kWinHgt) * kTurnAmt;
				
   // Get the angle between the camera's forward vector and the 
   // world's up vector
   float angle = AngleBetweenVecs(cam->getCamForward(), kWorldUp);
   				
   // If we haven't rotated past our threshold
   if(angle < (180 - kPitchAmt))
    cam->pitch(D3DXToRadian(-amt), pos);

   
 }
 
 else if(state.lY < 0 ) // If we've rotated down
 {
   // Calculate amount of rotation
   float amt = (1.0f - (float)(kWinHgt + yPos) / (float)kWinHgt) * kTurnAmt;
				
   // Get the angle between the camera's forward vector and the 
   // world's up vector
   float angle = AngleBetweenVecs(cam->getCamForward(), kWorldUp);
					
   // If we haven't rotated past our threshold
   if(angle > kPitchAmt)
    cam->pitch(D3DXToRadian(amt), pos);
  
 }	
 
 if(state.lX == 0)
  sPlayerInfo->isTurning = false;
 }

}

void PlayerControl::readOtherControls(playerInfo *sPlayerInfo)
{
	static bool keyDown = false;
	static bool keyDown2 = false;
	static bool keyDown3 = false;
	//control for entering/leaving car
	if(DXUTIsKeyDown(VK_RETURN) == true && keyDown == false && (sPlayerInfo->isInReachOfCar || sPlayerInfo->isInCar)){
	sPlayerInfo->isInCar = !sPlayerInfo->isInCar;
	keyDown = true;
	}
	if(DXUTIsKeyDown(VK_RETURN) == false && keyDown == true)
	keyDown = false;
	 
	//talking to NPCs
    if(DXUTIsKeyDown(VK_SPACE) == true && keyDown2 == true)
	   NPCManager::playerRequestedTalk = false;
	if(DXUTIsKeyDown(VK_SPACE) == true && keyDown2 == false){
	   NPCManager::playerRequestedTalk = true;
	   keyDown2 = true;
	}
	if(DXUTIsKeyDown(VK_SPACE) == false && keyDown2 == true){
	   NPCManager::playerRequestedTalk = false;
	   keyDown2 = false;
	}
    

	//entering/leaving buildings 
    if(canEnterLeave){
		if(DXUTIsKeyDown('U') == true && keyDown3 == true)
			CityManager::playerRequestedEnter = false;
		if(DXUTIsKeyDown('U') == true && keyDown3 == false){
			CityManager::playerRequestedEnter = true;
			keyDown3 = true;
		}
		if(DXUTIsKeyDown('U') == false && keyDown3 == true){
			CityManager::playerRequestedEnter = false;
			keyDown3 = false;
		}
    }
 
}

void PlayerControl::updatePlayerInfo(playerInfo *pInfo)
{
	//update the facing vec and position of player
    CVector c = cam->getCamForward();
	pInfo->facing = D3DXVECTOR3(c.x,c.y,c.z);
	pInfo->vecPos = D3DXVECTOR3(pInfo->pos._41, pInfo->pos._42+HEIGHTADJUST, pInfo->pos._43);
	
    //update the lookAt pt and eye pt of the camera
    CPos p = getCamera()->getTarget();
	pInfo->lookAt = D3DXVECTOR3(p.x,p.y,p.z);
    p = getCamera()->getEye(); 
    pInfo->eyePos = D3DXVECTOR3(p.x, p.y, p.z);

    //update the viewing matrix
    D3DXMATRIX mView;
    D3DXMatrixLookAtLH(&mView, &pInfo->eyePos, &pInfo->lookAt, &D3DXVECTOR3(0,1,0));
    pInfo->matView = mView;
  
}

void PlayerControl::FrameMove(playerInfo *sPlayerInfo, float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice)
{
	readKeyboardInput(sPlayerInfo);
	//if(canMoveMouse && sPlayerInfo->isInCar == false)
	readMouseInput(m_fElapsedTime, m_pd3dDevice, sPlayerInfo);
 
 
	updatePlayerInfo(sPlayerInfo);
	
	 
	if(sPlayerInfo->isInCar == false)
	  setViewMatrix(m_pd3dDevice);
	 
	D3DXMATRIX matTrans, matRotY, matWorld;
	D3DXMatrixIdentity(&matWorld); D3DXMatrixIdentity(&matRotY);
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(facing));
	D3DXMatrixTranslation(&matTrans, sPlayerInfo->pos._41,sPlayerInfo->pos._42+HEIGHTADJUST,sPlayerInfo->pos._43);
	
	//for CNTRL+mouse camera movement    
	if(DXUTIsKeyDown(VK_LCONTROL) == TRUE && isY == false){
		tempEye = cam->getEye();
		tempTarget = cam->getTarget();
		isY = true;
	}
	if(DXUTIsKeyDown(VK_LCONTROL) == FALSE && isY == true){
		cam->setEye(tempEye);
		cam->setTarget(tempTarget);
		isY = false;
	}
 

	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotY);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
	m_xFileEntity[currSet]->FrameMove(m_fElapsedTime,matWorld);
 
}


void PlayerControl::setViewMatrix(LPDIRECT3DDEVICE9 device)
{
 D3DXMATRIXA16 matrix;

 // Create "D3D Vector" versions of our camera's eye, look at position, and up vector
 D3DXVECTOR3 eye(cam->getEye().x, cam->getEye().y, cam->getEye().z);
 D3DXVECTOR3 lookAt(cam->getTarget().x, cam->getTarget().y, cam->getTarget().z);
 D3DXVECTOR3 up(0, 1, 0); // The world's up vector
 
 D3DXMatrixLookAtLH(&matrix, &eye, &lookAt, &up);
 device->SetTransform(D3DTS_VIEW, &matrix); // Set our view of the world

}

void PlayerControl::InitDeviceObjects(LPDIRECT3DDEVICE9 device)
{
  //load robot animations
  m_xFileEntity[ROBOT_ID] = new CXfileEntity();
  m_xFileEntity[ROBOT_ID]->LoadXFile(device,L"meshes/robot.x");		
  m_xFileEntity[ROBOT_ID]->SetAnimationSet(m_currentAnimation);
  m_xFileEntity[ROBOT_ID]->setAnimationSpeed(1.0f);
  m_xFileEntity[ROBOT_ID]->initDeviceObjects(device);	

  //load human animations
  m_xFileEntity[HUMAN_ID] = new CXfileEntity();
  m_xFileEntity[HUMAN_ID]->LoadXFile(device,L"models/humanPlayer.x");		
  m_xFileEntity[HUMAN_ID]->SetAnimationSet(m_currentAnimation);
  m_xFileEntity[HUMAN_ID]->setAnimationSpeed(1.0f);
  m_xFileEntity[HUMAN_ID]->initDeviceObjects(device);	

  //init mouse
  DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8W, (VOID**)&g_pDI, NULL );
  g_pDI->CreateDevice(GUID_SysMouse, &mouse, NULL);
  mouse->SetDataFormat(&c_dfDIMouse);
  mouse->SetCooperativeLevel(DXUTGetHWND(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
  mouse->Acquire();
  
}


void PlayerControl::RestoreDeviceObjects(LPDIRECT3DDEVICE9 m_pd3dDevice)
{
  m_xFileEntity[0]->restoreDeviceObjects(m_pd3dDevice);
  m_xFileEntity[1]->restoreDeviceObjects(m_pd3dDevice);	
  
}
void PlayerControl::invalidateDeviceObjects()
{
	mouse->Release();
	g_pDI->Release();
    	
}

void PlayerControl::destroyDeviceObjects()
{
  m_xFileEntity[0]->DeleteDeviceObjects();
  m_xFileEntity[1]->DeleteDeviceObjects();
  mouse->Release();
  g_pDI->Release();
  delete m_xFileEntity[0];
  delete m_xFileEntity[1];
  
}

void PlayerControl::setAnimationStatus(int n)
{
  m_xFileEntity[currSet]->SetAnimationSet(n);
  m_currentAnimation = n;
}
