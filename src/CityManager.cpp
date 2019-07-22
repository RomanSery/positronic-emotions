#include "dxstdafx.h"
#include ".\citymanager.h"
#include "PrintText.h"
#include "cityFuncs.h"
#include "Player.h"
#include "Renderer.h"

#define NUM_BLOCKS 1


bool      CityManager::playerRequestedEnter;
Interior* CityManager::interiors[3];
bool	  CityManager::isInteriorBeingRendered[3];

CityManager::CityManager(void)
{
	ship = new SpaceShip(-278, 53, 603);
	streetLights = new CDXUTMesh(); 
    triggerManager = new TriggerManager();
    
    for(int i = 0; i < NUM_BLOCKS; i++)
	 blocks[i] = new CDXUTMesh();

	fileNames[0] = L"meshes/blocks/testBlock.x";  //block1.x";
    //fileNames[0] = L"meshes/blocks/block1.x";
    fileNames[1] = L"meshes/blocks/block2.x";
	fileNames[2] = L"meshes/blocks/block3.x";
	fileNames[3] = L"meshes/blocks/block4.x";
	fileNames[4] = L"meshes/blocks/block5.x";
	fileNames[5] = L"meshes/blocks/block6.x";
	fileNames[6] = L"meshes/blocks/block7.x";
	
	//directx up/down = y value		3dmax up/down = z value
	//																width,height,depth
	positions[0] = D3DXVECTOR3(-740, -327, 166); sizes[0] = D3DXVECTOR3(762, 375, 333);
	positions[1] = D3DXVECTOR3(-146, -574, 83);  sizes[1] = D3DXVECTOR3(214, 287, 175);
	positions[2] = D3DXVECTOR3(367, -583, 65);   sizes[2] = D3DXVECTOR3(227, 361, 130);
	positions[3] = D3DXVECTOR3(657, 29, 195);    sizes[3] = D3DXVECTOR3(787, 436, 386);
	positions[4] = D3DXVECTOR3(-1, 428, 57);     sizes[4] = D3DXVECTOR3(201, 853, 114);
	positions[5] = D3DXVECTOR3(-329, -69, 64);   sizes[5] = D3DXVECTOR3(231, 188, 128);
	positions[6] = D3DXVECTOR3(187, -69, 64);    sizes[6] = D3DXVECTOR3(231, 285, 128);


    //set up interiors
    interiors[0] = new Interior(0, D3DXVECTOR3(244, 0, -187), "Guns N Ammo", L"meshes/gunStore.x", L"meshes/gunStoreCollision.x" );
	interiors[1] = new Interior(1, D3DXVECTOR3(298, 0, 29), "Hardware Store", L"meshes/hardwareStore.x", L"meshes/hardwareStoreCollision.x" );

	numBlocks = NUM_BLOCKS;
    numInteriors = 2;
    numFaces = 0;
}

void CityManager::InitDeviceObjects(IDirect3DDevice9 *device)
{
  ship->initDeviceObjects(device);
  streetLights->Create(device, L"meshes/streetLights.x");
  
  for(int i = 0; i < NUM_BLOCKS; i++){
    blocks[i]->Create(device, fileNames[i]);
  }
  
  for(int i = 0; i < numInteriors; i++)
      interiors[i]->InitDeviceObjects(device);
}

void CityManager::render(IDirect3DDevice9 *device, cFrustum *frustum, ID3DXEffect* effect,
						 D3DXVECTOR3 lightPos)
{
    
  //render the city only if player is outside
  if(Player::sPlayerInfo->isInInterior == false){
	if(Renderer::SUPPORTS_SHADERS_1_1){
      
	  //renderMeshWithBump(device, effect, blocks[0], lightPos);
	  renderWithNoBump(device, blocks[0]);
    }
	else{
      blocks[0]->Render(device);
    }
	renderWindows(device, blocks[0]);     


	//render street lights and spaceship
	//renderStreetLights(device, streetLights);  	 
    ship->render(device, frustum);
  }

  //render interior that is inhabited by player
  for(int i = 0; i < numInteriors; i++)
      isInteriorBeingRendered[i] = interiors[i]->render(device);
  
  triggerManager->render(device);  

}


void CityManager::processEnterRequest()
{
  float min = interiors[0]->getDistanceFromPlayer();
  int index = 0;  
 
  for(int x = 1; x < numInteriors; x++)
  {
     float distance = interiors[x]->getDistanceFromPlayer();
     if(min > distance){
       min = distance;
	   index = x;
     } 
  }
  if(min < 30.0f)
    interiors[index]->processEnterRequestByPlayer();
}

void CityManager::frameMove(float m_fElapsedTime, IDirect3DDevice9 *device)
{ 
  ship->frameMove(device);
  triggerManager->frameMove();

  for(int i = 0; i < numInteriors; i++)
    interiors[i]->frameMove(m_fElapsedTime, device);

  if(playerRequestedEnter)
   processEnterRequest();
    
   
}

void CityManager::RestoreDeviceObjects(IDirect3DDevice9 *device)
{
  ship->restoreDeviceObjects(device);
  streetLights->RestoreDeviceObjects(device);
  
  for(int i = 0; i < NUM_BLOCKS; i++){
   	blocks[i]->RestoreDeviceObjects(device);
  }

  for(int i = 0; i < numInteriors; i++)
      interiors[i]->RestoreDeviceObjects(device);
}

void CityManager::invalidateDeviceObjects()
{
	ship->invalidateDeviceObjects();
    streetLights->InvalidateDeviceObjects();
    
    for(int i = 0; i < NUM_BLOCKS; i++){
       blocks[i]->InvalidateDeviceObjects();
    }
    
    for(int i = 0; i < numInteriors; i++)
      interiors[i]->invalidateDeviceObjects();
}

void CityManager::destroyDeviceObjects()
{
	ship->destroyDeviceObjects();
    streetLights->Destroy();
    
    for(int i = 0; i < NUM_BLOCKS; i++){
	   blocks[i]->Destroy();
    }
    
    for(int i = 0; i < numInteriors; i++)
      interiors[i]->destroyDeviceObjects();
}
	 //for(int i = 0; i < NUM_BLOCKS; i++){
	   //if(frustum->CheckRectangle(positions[i].x,positions[i].z,positions[i].y,
		//					   sizes[i].y / 2, sizes[i].z / 2, sizes[i].x / 2) == TRUE ){