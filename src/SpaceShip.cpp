#include "dxstdafx.h"
#include ".\spaceship.h"
#include "PrintText.h"
#include "MeshHeirarchy.h"
#include "Frustum.h"
#include "OcclusionCuller.h"

D3DXVECTOR3 SpaceShip::shipPos;
bool		SpaceShip::startAnim = false;

SpaceShip::SpaceShip(float x, float y, float z)
{
  pos = D3DXVECTOR3(x, y, z);
  shipPos = D3DXVECTOR3(x, y, z);
  animationOver = false;
  triggered = false;
  time = 0.0;

  bb = new sBoundingBox(DOCTOR);
  bb->height = 110;
  bb->length = 180;
  bb->width = 180;
 
}

void SpaceShip::initDeviceObjects(IDirect3DDevice9 *device)
{
  mesh = new CXfileEntity();
  mesh->LoadXFile(device, L"meshes/spaceship.x");
  mesh->initDeviceObjects(device);

}

void SpaceShip::render(IDirect3DDevice9 *device, cFrustum *frustum)
{
  if( animationOver == false && isInVeiwingFrustum(frustum) ){
	device->SetRenderState(D3DRS_LIGHTING, true);  
	mesh->Render(device, SPACESHIP_CODE, NULL);
	device->SetRenderState(D3DRS_LIGHTING, false);  
  }


}

void SpaceShip::frameMove(IDirect3DDevice9 *device)
{
  D3DXMATRIX matWorld; D3DXMatrixIdentity(&matWorld);
  D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);

  if(startAnim && triggered == false){
     mesh->SetAnimationSet(0);
     mesh->setAnimationSpeed(1.0f);     
     triggered = true;
  }
  else if(triggered){
     time = time + DXUTGetElapsedTime();
     if(time > 11.700) animationOver = true;
  }

  if(animationOver == false)
	 mesh->FrameMove(DXUTGetElapsedTime(), matWorld);

}

bool SpaceShip::isInVeiwingFrustum(cFrustum *frustum)
{
	if(startAnim == false){
	  if( frustum->CheckSphere(pos.x, pos.y, pos.z, 30) == false)
        return false;
      else
        return !OcclusionCuller::isObjectOccludedEx(shipPos, bb); 

    }
	
	//always return true if ship is in flight
	return true;

}

void SpaceShip::restoreDeviceObjects(IDirect3DDevice9 *device)
{
  mesh->restoreDeviceObjects(device);

}

void SpaceShip::destroyDeviceObjects()
{
  mesh->DeleteDeviceObjects();
}

void SpaceShip::invalidateDeviceObjects()
{
 
}



