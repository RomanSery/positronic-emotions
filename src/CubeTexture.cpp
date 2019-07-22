#include "dxstdafx.h"
#include ".\cubetexture.h"


CubeTexture::CubeTexture()
{
 
}
IDirect3DCubeTexture9* CubeTexture::getCubeTexture()
{
 return cubeTexture;
}

void CubeTexture::initDeviceObjects(IDirect3DDevice9* device)
{
 D3DDISPLAYMODE info;
 device->GetDisplayMode(1, &info); 

 D3DXCreateCubeTexture(device, 256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R8G8B8,
					   D3DPOOL_DEFAULT, &cubeTexture);


 D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/2, 1.0f, 0.1f, 100000.0f);

}

void CubeTexture::startRendering(IDirect3DDevice9* device)
{
   // Store the current back buffer
   device->GetRenderTarget(0, &origBackBuffer);
   device->GetDepthStencilSurface(&origDepthStencil);
   
   // Use 90-degree field of view in the projection
   device->SetTransform(D3DTS_PROJECTION, &matProj);

   device->BeginScene();

}

void CubeTexture::buildCubeMap(IDirect3DDevice9* device, D3DXVECTOR3 eyePos, DWORD i)
{
   // Standard view that will be overridden below
   D3DXVECTOR3 vEnvEyePt = eyePos;
   D3DXVECTOR3 vLookatPt, vUpVec;

   switch(i)
   {
            case D3DCUBEMAP_FACE_POSITIVE_X:
                vLookatPt = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
                vUpVec    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
                break;
            case D3DCUBEMAP_FACE_NEGATIVE_X:
                vLookatPt = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
                vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
                break;
            case D3DCUBEMAP_FACE_POSITIVE_Y:
                vLookatPt = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
                vUpVec    = D3DXVECTOR3(0.0f, 0.0f,-1.0f);
                break;
            case D3DCUBEMAP_FACE_NEGATIVE_Y:
                vLookatPt = D3DXVECTOR3(0.0f,-1.0f, 0.0f);
                vUpVec    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
                break;
            case D3DCUBEMAP_FACE_POSITIVE_Z:
                vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
                vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
                break;
            case D3DCUBEMAP_FACE_NEGATIVE_Z:
                vLookatPt = D3DXVECTOR3(0.0f, 0.0f,-1.0f);
                vUpVec    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
                break;
   }

   D3DXMATRIX matView;
   D3DXMatrixLookAtLH(&matView, &vEnvEyePt, &vLookatPt, &vUpVec);
   device->SetTransform(D3DTS_VIEW, &matView);

   // Get pointer to surface in order to render to it
   cubeTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &cubeMapFace);
   device->SetRenderTarget(0, cubeMapFace);
   device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
   SAFE_RELEASE(cubeMapFace);   
		
}

void CubeTexture::finishRendering(IDirect3DDevice9* device, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt)
{
  device->EndScene();

  // Change the render target back to the main back buffer and reset the proj matrix
  D3DXMATRIX proj;
  D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );
  device->SetTransform(D3DTS_PROJECTION, &proj);

  //reset view params
  D3DXMATRIX view;D3DXVECTOR3 up(0,1,0);
  D3DXMatrixLookAtLH(&view, &eyePos, &lookAt, &up);
  device->SetTransform(D3DTS_VIEW, &view);

  device->SetRenderTarget(0, origBackBuffer);
  device->SetDepthStencilSurface(origDepthStencil);
  SAFE_RELEASE(origBackBuffer);
  SAFE_RELEASE(origDepthStencil);
}

void CubeTexture::restoreDeviceObjects(IDirect3DDevice9* device)
{
  
}

void CubeTexture::destroyDeviceObjects()
{
  SAFE_DELETE(cubeTexture);
  SAFE_DELETE(cubeMapFace);
  SAFE_DELETE(origBackBuffer);
}

void CubeTexture::invalidateDeviceObjects()
{
 SAFE_RELEASE(cubeTexture);
 SAFE_RELEASE(cubeMapFace);
 SAFE_RELEASE(origBackBuffer);
}
