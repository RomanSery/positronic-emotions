#include "dxstdafx.h"
#include ".\rendertotexture.h"

RenderToTexture::RenderToTexture(void)
{
 renderTexture = NULL;
 renderSurface = originalSurface = NULL;
 destSurface = NULL;
 sprite = NULL;
}

void RenderToTexture::initDeviceObjects(IDirect3DDevice9 *device)
{
 D3DXCreateSprite(device, &sprite);

 D3DDISPLAYMODE info;
 device->GetDisplayMode(1, &info); 

 device->CreateRenderTarget(1024,768, info.Format, D3DMULTISAMPLE_NONE, 0, TRUE, &destSurface, NULL);
 D3DXCreateTexture(device, 1024,768, 1, D3DUSAGE_RENDERTARGET,
   				D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &renderTexture);
 
 D3DXCreateTexture(device, 512,512, 1, D3DUSAGE_RENDERTARGET,
  				info.Format, D3DPOOL_DEFAULT, &generatedTexture);
 

}

void RenderToTexture::renderSprite(IDirect3DDevice9* device)
{
 sprite->Begin(0);
  sprite->Draw(renderTexture,NULL,NULL,NULL, 0xFFFFFFFF);
 sprite->End();
  
}

void RenderToTexture::renderSprite(IDirect3DDevice9* device, IDirect3DTexture9 *tex)
{
 sprite->Begin(0);
  sprite->Draw(tex,NULL,NULL,NULL, 0xFFFFFFFF);
 sprite->End();

}

void RenderToTexture::generateTexture(IDirect3DDevice9* device)
{
 IDirect3DSurface9* sourceSurface = NULL;
 
 device->GetRenderTarget(0, &sourceSurface);
 device->StretchRect(sourceSurface, NULL, destSurface, NULL, D3DTEXF_NONE);
 generatedTexture->GetSurfaceLevel(0, &destSurface);
 
 SAFE_RELEASE(sourceSurface);
   
}

void RenderToTexture::startRendering(IDirect3DDevice9 *device, bool clear)
{
  renderTexture->GetSurfaceLevel(0, &renderSurface);
  
  //save old render target
  device->GetRenderTarget(0, &originalSurface);
   
  //set new render target  
  device->SetRenderTarget(0, renderSurface);
  device->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
  device->BeginScene();

}

void RenderToTexture::clear(IDirect3DDevice9 *device)
{
  device->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
}
void RenderToTexture::finishRendering(IDirect3DDevice9 *device)
{
  device->EndScene();
  //restore original render target
  device->SetRenderTarget(0, originalSurface);
  SAFE_RELEASE(originalSurface);
  SAFE_RELEASE(renderSurface);
}

void RenderToTexture::restoreDeviceObjects(IDirect3DDevice9 *device)
{
	sprite->OnResetDevice();
}

void RenderToTexture::invalidateDeviceObjects()
{
	sprite->OnLostDevice();
	SAFE_RELEASE(renderTexture);
	SAFE_RELEASE(generatedTexture);
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(destSurface);
	SAFE_DELETE(destSurface);
}

void RenderToTexture::destroyDeviceObjects()
{
  SAFE_RELEASE(destSurface);
  SAFE_DELETE(destSurface);
  SAFE_DELETE(sprite);
  SAFE_DELETE(renderTexture);
  SAFE_DELETE(generatedTexture);

     
}