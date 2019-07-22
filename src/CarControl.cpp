#include "dxstdafx.h"
#include ".\carcontrol.h"
#include "carFuncs.h"


CarControl::CarControl()
{
  cam = new CCamera();
  CPos target; target.set(0,0,0);
  cam->setTarget(target);
  eyePt = D3DXVECTOR3(50, 30, 0);
  
}

void CarControl::frameMove(carInfo *info, IDirect3DDevice9 *device)
{
  
  D3DXVECTOR3 front(info->velocity.x,0 , info->velocity.z);
  D3DXVec3Normalize(&front, &front);
  
  D3DXVECTOR3 pos(info->pos._41, info->pos._42, info->pos._43);
  
  if(info->state != PARK && info->holdingDownBrake == false){
    eyePt.operator +=( (pos.operator -(eyePt) / 16.0f) );
    eyePt.x -= front.x * 7;
    eyePt.z -= front.z * 7;
    eyePt.y = 45.0f;
  }
  
  D3DXVECTOR3 lookAt(pos);
  D3DXVECTOR3 up(0, 1, 0); 
  D3DXMATRIX mat;
  D3DXMatrixLookAtLH(&mat, &eyePt, &lookAt, &up);
  device->SetTransform(D3DTS_VIEW, &mat); // Set our view of the world
  
  matView = mat;
 
}

void CarControl::processInput(carInfo *info)
{
  static bool keyDownL = false; //lights
  static bool keyDownP = false; //transmission
  static int  s = 1;
  
  //lights
  if(DXUTIsKeyDown('L') == true && keyDownL == false){
   info->areLightsOn = !info->areLightsOn;
   keyDownL = true;
  }
  if(DXUTIsKeyDown('L') == false && keyDownL == true)
   keyDownL = false;

  //transmission
  if(DXUTIsKeyDown('P') == true && keyDownP == false){
   s++;  if(s == 3) s = 0;
   keyDownP = true;
   
  }
  if(DXUTIsKeyDown('P') == false && keyDownP == true)
   keyDownP = false;
  info->state = (Transmission)s;
  
}