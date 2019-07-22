#ifndef PLAYER_INFO
#define PLAYER_INFO


struct playerInfo{
 D3DXMATRIX   pos;		//updated by Newton
 D3DXVECTOR3  vecPos;
 D3DXVECTOR3  velocity; //updated by Newton
 D3DXVECTOR3  facing;   //updated by Player
 bool		  isMoving; //updated by Player
 bool		  isTurning;//updated by Player

 bool		  isInCar;
 bool		  isInReachOfCar;
 bool		  isInInterior;
 int		  interiorID;
 float		  dist;		//distance from car

 bool		  isOnMission;
 bool		  isInDialogue;
 bool		  isHuman;

 D3DXVECTOR3  lightPos; //position of light closest to player
 
 //updated by PlayerControl
 //camera viewing params to be used by other classes if needed 
 D3DXVECTOR3  eyePos; //pos of camera eye
 D3DXVECTOR3  lookAt; //the target pt at which the camera is centered around
 D3DXMATRIX   matView;

  
 playerInfo::playerInfo()
 {
  isInCar = isInReachOfCar = isMoving = isTurning = isInInterior = isOnMission = false;
  isInDialogue = false;
  isHuman = false;
  facing = D3DXVECTOR3(0,0,0);
  velocity = lightPos = D3DXVECTOR3(0,0,0);
  D3DXMatrixIdentity(&pos);
  dist = 0.0f;
  interiorID = -1;

  eyePos = lookAt = D3DXVECTOR3(0,0,0);
  D3DXMatrixIdentity(&matView);
 }
};

#endif