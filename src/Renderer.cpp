#include "dxstdafx.h"
#include ".\renderer.h"
#include <vector>
#include "cityFuncs.h"
#include "PrintText.h"

bool Renderer::SUPPORTS_SHADERS_1_1 = false;

Renderer::Renderer(Player *player)
{
	ground = new CDXUTMesh();
	city = new CityManager();
	house = new CDXUTMesh();
	lights = new Lights();
	  
	//cubeTexture = new CubeTexture();
	renderToTex = new RenderToTexture();
    filter = new FilterBlit();
	npcs = new NPCManager();

	D3DXMatrixIdentity(&matWorld);
	p = player;
 
}

void Renderer::initDeviceObjects(IDirect3DDevice9* device)
{
  pd3dDevice = device;
	
  ground->Create(device, L"meshes/ground.x");
  house->Create(pd3dDevice, L"meshes/ground.x");
   
  
  lights->initDeviceObjects(device);
  //cubeTexture->initDeviceObjects(device);
  renderToTex->initDeviceObjects(device);
  filter->initDeviceObjects(device);
  npcs->InitDeviceObjects(device);
  city->InitDeviceObjects(device);  

    
  WCHAR str[100];
  DXUTFindDXSDKMediaFileCch( str, 100, L"windows.fx");
  D3DXCreateEffectFromFile(pd3dDevice, str, NULL, NULL, D3DXFX_NOT_CLONEABLE,NULL,&effect, NULL);
  DXUTFindDXSDKMediaFileCch( str, 100, L"normalMap.fx");
  D3DXCreateEffectFromFile(pd3dDevice, str, NULL, NULL, D3DXFX_NOT_CLONEABLE,NULL,&effectNormalMap, NULL);

  D3DXCreateCubeTextureFromFile(pd3dDevice, L"media/cubeMap.dds", &cubeTex);
    
}

void Renderer::renderGlow()
{
  //render glow lines ALPHA=80
  renderToTex->startRendering(pd3dDevice);
	pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
    renderGlowLines(pd3dDevice, city->getBlocks());
  renderToTex->finishRendering(pd3dDevice);  
  filter->RenderGlow(pd3dDevice, renderToTex->getRenderTexture(), "Glow");
   
  //render glowing lit windows ALPHA=50  
  renderToTex->startRendering(pd3dDevice);
     renderLitWindows(pd3dDevice, city->getBlocks());
  renderToTex->finishRendering(pd3dDevice);  
  filter->RenderGlow(pd3dDevice, renderToTex->getRenderTexture(), "Window", 8);
  
    
}


void Renderer::renderToCubeMap()
{
  cubeTexture->startRendering(pd3dDevice);  
  	
  for(DWORD i=0; i<6; i++)
  {
	cubeTexture->buildCubeMap(pd3dDevice, Player::sPlayerInfo->eyePos, i);
  
    //render scene
	//renderSceneElements(true);
  }
  cubeTexture->finishRendering(pd3dDevice, Player::sPlayerInfo->eyePos, Player::sPlayerInfo->lookAt);
    
}

void Renderer::render()
{
    p->Render(pd3dDevice, SUPPORTS_SHADERS_1_1, cubeTex);
    renderSceneElements(false);
		
    if(Player::sPlayerInfo->isInInterior == false && SUPPORTS_SHADERS_1_1)
       renderGlow();
			

	renderToTex->generateTexture(pd3dDevice);
	//filter->Render(pd3dDevice, renderToTex->getGeneratedTexture());	

	//renderToCubeMap();
		
}

void Renderer::frameMove(float fElapsedTime)
{
	npcs->FrameMove(fElapsedTime, pd3dDevice);
	city->frameMove(fElapsedTime, pd3dDevice);
    lights->frameMove();

	//update the position of the closest light to player
    Player::sPlayerInfo->lightPos = lights->getClosestLight(Player::sPlayerInfo->vecPos);
 
}

void Renderer::renderSceneElements(bool renderingToCubeMap)
{
  pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
  
 
  if(renderingToCubeMap == false){
      if(Player::sPlayerInfo->isInInterior == false){
        if(Renderer::SUPPORTS_SHADERS_1_1)
		  renderUnLitWindows(pd3dDevice, effect, city->getBlocks(), cubeTex); //ALPHA=0
        ground->Render(pd3dDevice);
        npcs->render(pd3dDevice, p->getFrustum());
	  }
      pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
      city->render(pd3dDevice, p->getFrustum(), effectNormalMap, lights->getLight(0));
      lights->renderLights(pd3dDevice);
  }

}


void Renderer::restoreDeviceObjects()
{
  ground->RestoreDeviceObjects(pd3dDevice);
  house->RestoreDeviceObjects(pd3dDevice);
  lights->restoreDeviceObjects(pd3dDevice);
  effect->OnResetDevice();
  effectNormalMap->OnResetDevice();
  renderToTex->restoreDeviceObjects(pd3dDevice);
  npcs->RestoreDeviceObjects(pd3dDevice);
  city->RestoreDeviceObjects(pd3dDevice);
  filter->RestoreDeviceObjects(pd3dDevice);
  
}

void Renderer::invalidateDeviceObjects()
{
  ground->InvalidateDeviceObjects();
  house->InvalidateDeviceObjects();
  lights->invalidateDeviceObjects();
  effect->OnLostDevice();
  effectNormalMap->OnLostDevice();
  SAFE_RELEASE( effect);
  SAFE_RELEASE( effectNormalMap);
  SAFE_RELEASE( cubeTex);
  //cubeTexture->invalidateDeviceObjects();
  renderToTex->invalidateDeviceObjects();
  npcs->invalidateDeviceObjects();
  city->invalidateDeviceObjects();
  filter->InvalidateDeviceObjects(pd3dDevice);
}

void Renderer::destroyDeviceObjects()
{
  ground->Destroy();
  house->Destroy();
  lights->destroyDeviceObjects();
  renderToTex->destroyDeviceObjects();
  //cubeTexture->destroyDeviceObjects();
  npcs->destroyDeviceObjects();
  city->destroyDeviceObjects();
  
  SAFE_DELETE(ground);
  SAFE_DELETE(house);
  SAFE_DELETE(lights);
  SAFE_DELETE(renderToTex);
  SAFE_DELETE(effect);
  SAFE_DELETE(effectNormalMap);
  SAFE_DELETE(cubeTex);
}
