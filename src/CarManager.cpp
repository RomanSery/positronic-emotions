#include "dxstdafx.h"
#include ".\carmanager.h"
#include "structs.h"
#include "PrintText.h"
#include "NewtonObjects.h"
#include "Player.h"
#include "OcclusionCuller.h"

CarManager::CarManager(void)
{
  mercCar = new CDXUTMesh(L"meshes/car.x");
  mercWheel = new CDXUTMesh(L"meshes/wheel.x");
	
  car = new NewtonVehicle();
  
}

void CarManager::addCars(NewtonWorld *nWorld, int matID)
{
  D3DXMATRIX m;D3DXMatrixIdentity(&m);D3DXMatrixTranslation(&m, 100, 30, 100);
  car->create(nWorld, m);
  car->createCar();
  NewtonBodySetAutoFreeze(car->GetRigidBody(), 0);
  NewtonBodySetMaterialGroupID(car->GetRigidBody(), matID);
  
}

void CarManager::frameMove(IDirect3DDevice9 *device)
{
  	car->frameMove(device);
	car->processInput();
	
    playerInfo *pInfo = Player::sPlayerInfo;	
	//determine if player is in reach of car
	D3DXVECTOR3 d;
	D3DXVec3Subtract(&d, &car->getVecPos(), &pInfo->vecPos);
    pInfo->dist = max(abs(d.x), abs(d.z));
	pInfo->dist < 50.0f ? pInfo->isInReachOfCar = true : pInfo->isInReachOfCar = false;
    
	
	if(pInfo->isInCar)
	  car->setPlayerControl(true);
	else
	  car->setPlayerControl(false);

	
}



void CarManager::render(IDirect3DDevice9 *device, cFrustum *frustum)
{
    D3DXVECTOR3 carPos(car->getCarInfo()->pos._41, car->getCarInfo()->pos._42, car->getCarInfo()->pos._43);
   
    if( frustum->CheckSphere(carPos.x, carPos.y, carPos.z, 30) ){
      if(OcclusionCuller::isObjectOccludedEx(carPos, car->getCarInfo()->boundingBox) == false){
		if(car->isUnderPlayerControl == false)
			car->render(mercCar, mercWheel, device, Player::sPlayerInfo->matView, Player::sPlayerInfo->eyePos);
		else
			car->render(mercCar, mercWheel, device, car->getCarControl()->getMatView(),
							car->getCarControl()->getEyePt()); 	 
      }
    }    
}

void CarManager::InitDeviceObjects(IDirect3DDevice9 *device)
{
  mercCar->Create(device, L"meshes/car.x");
  mercWheel->Create(device, L"meshes/wheel.x");

  car->initDeviceObjects(device);
    
}

void CarManager::RestoreDeviceObjects(IDirect3DDevice9 *device)
{
  mercCar->RestoreDeviceObjects(device);
  mercWheel->RestoreDeviceObjects(device);
  
  car->restoreDeviceObjects(device);
}

void CarManager::invalidateDeviceObjects()
{
  mercCar->InvalidateDeviceObjects();
  mercWheel->InvalidateDeviceObjects();
  car->invalidateDeviceObjects();
}
void CarManager::destroyDeviceObjects()
{
	mercCar->Destroy();
	mercWheel->Destroy();

  car->destroyDeviceObjects();
}


