//entry point for program, initializes all components and calls their render
//and frame move functions at the appropriate time.

#include "dxstdafx.h"
#include "resource.h"
#include "d3dfont.h"
#include <stdio.h>
#include "Player.h"
#include "Renderer.h"
#include "NewtonObjects.h"
#include "SoundManager.h"
#include "PrintText.h"
#include "MissionManager.h"
#include "OcclusionCuller.h"

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CDXUTDialog         *dialogUI;             // dialog for sample specific controls
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs

Renderer			*renderer;				//rendering manager
NewtonObjects		*newton;				//newton objects manager

D3DXMATRIX			matWorld;
CD3DFont*           m_pFont;                //Font for drawing text

Player			    *p;						//player pointer

SoundManager		*soundM;
MissionManager		*missionM;
PrintText			*text;
OcclusionCuller		*occ;



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext  );
void    CALLBACK OnLostDevice(void* pUserContext);
void    CALLBACK OnDestroyDevice(void* pUserContext);

void    InitApp();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
	
    InitApp();
	
	// Dont parse the command line, handle the default hotkeys, and show msgboxes
    DXUTInit( false, true, true ); 
    DXUTCreateWindow( L"Terminator");
	DXUTCreateDevice( D3DADAPTER_DEFAULT, false, 1024, 768, NULL, ModifyDeviceSettings, NULL );
		

    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();

    return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
 m_pFont =  new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
 p = new Player();
 renderer = new Renderer(p);
 newton =   new NewtonObjects(p);
 
 dialogUI = new CDXUTDialog();
 dialogUI->Init(&g_DialogResourceManager);
 text = new PrintText(m_pFont, dialogUI);
 missionM = new MissionManager();

 occ = new OcclusionCuller();
 D3DXMatrixIdentity(&matWorld);

}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// the sample framework will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         (pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) || pCaps->PixelShaderVersion < D3DPS_VERSION(1,1)))
      pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else
      pDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    
	//check if device supports 1.1 shaders
    if(pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) || pCaps->PixelShaderVersion < D3DPS_VERSION(1,1))
      Renderer::SUPPORTS_SHADERS_1_1 = false;
    else
      Renderer::SUPPORTS_SHADERS_1_1 = true;         
	//Renderer::SUPPORTS_SHADERS_1_1 = false;         
    
    // This application is designed to work on a pure device by not using 
    // IDirect3D9::Get*() methods, so create a pure device if supported and using HWVP.
    if ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0 && 
        (pDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 )
        pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;
	
    
	D3DDISPLAYMODE d3ddm;
    DXUTGetD3DObject()->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	pDeviceSettings->pp.Windowed = false;
	pDeviceSettings->pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pDeviceSettings->pp.BackBufferFormat = d3ddm.Format;
	pDeviceSettings->pp.EnableAutoDepthStencil = true;
	pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pDeviceSettings->pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	pDeviceSettings->pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return true;
	
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	g_DialogResourceManager.OnCreateDevice( pd3dDevice );

	newton->initDeviceObjects(pd3dDevice);
	m_pFont->InitDeviceObjects( pd3dDevice );
	renderer->initDeviceObjects(pd3dDevice);
	p->InitDeviceObjects(pd3dDevice);  
    text->initDeviceObjects(pd3dDevice);  
    occ->initDeviceObjects(pd3dDevice);
	
	D3DXMATRIX matProj;
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(60.0f), fAspectRatio, 0.1f, 100000.0f );
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	

    soundM = new SoundManager();	

    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR  );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR  );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR  );

	pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 || D3DCLIPPLANE1 || D3DCLIPPLANE2
							  || D3DCLIPPLANE3 || D3DCLIPPLANE4 || D3DCLIPPLANE5);
    
	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	g_DialogResourceManager.OnResetDevice();

	renderer->restoreDeviceObjects();
	newton->restoreDeviceObjects();
	m_pFont->RestoreDeviceObjects();
    occ->restoreDeviceObjects(pd3dDevice);
	   
	D3DXMATRIX matProj, matWorld;

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(60.0f), fAspectRatio, 0.1f, 100000.0f );
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
    pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
    	
	p->RestoreDeviceObjects(pd3dDevice);
    PrintText::onResetDevice(pBackBufferSurfaceDesc);
  
	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    PrintText::frameMove(fElapsedTime);
    renderer->frameMove(fElapsedTime);
	newton->frameMove(pd3dDevice);
	

   	soundM->frameMove();
    missionM->frameMove(pd3dDevice);
    p->FrameMove(fElapsedTime, pd3dDevice, newton->carManager->car->getCarControl()->getMatView());

	//cFrustum *f = p->getFrustum();
	
	/*for(DWORD i = 0; i <= 5; i++){
	  D3DXPLANE p = f->m_Planes[i];
	  float values[4] = {p.a, p.b, p.c, p.d};
      pd3dDevice->SetClipPlane(i, values);
    }*/

        
        
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    	
	// Clear the render target and the zbuffer 
    ///////got rid of clearing stencil buffer
    pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 100, 100,100), 1.0f, 0);
		
    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		
        newton->drawNewtonScene(pd3dDevice, fElapsedTime);
        renderer->render();  		
        
        missionM->render(pd3dDevice);
		PrintText::render(fElapsedTime);
		

		//bool value;
		int offsets[7] = {10, 30, 50, 70, 90, 110, 130};
		/*
		for(int i = 0; i < 7; i++)
		{
		  printString("Block ", 10, offsets[i]);
		  printInt(i+1, 65, offsets[i]);

		  value = renderer->city->isBlockDrawn[i];
		  if(value == true)
		    printString(" is being drawn", 90, offsets[i]); 
		  else
            printString(" is not being drawn", 90, offsets[i]);
		}
		*/
		
		PrintText::printNum(DXUTGetFPS(), 500, 10);
		
        
        
		
	  pd3dDevice->EndScene();
		
    }
	
}

//--------------------------------------------------------------------------------------
// Before handling window messages, the sample framework passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then the sample framework will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );

	SoundManager::h = hWnd;

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = PrintText::ui->MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, the sample framework inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
 
}
void CALLBACK OnLostDevice(void* pUserContext)
{
	renderer->invalidateDeviceObjects();
	newton->invalidateDeviceObjects();
	m_pFont->InvalidateDeviceObjects();
	p->invalidateDeviceObjects();
	g_DialogResourceManager.OnLostDevice();
    text->invalidateDeviceObjects();
    occ->invalidateDeviceObjects();
   	DXUTGetD3DDevice()->Release();
}
void CALLBACK OnDestroyDevice(void* pUserContext)
{
	renderer->destroyDeviceObjects();
	newton->destroyDeviceObjects();
	m_pFont->DeleteDeviceObjects();
	p->destroyDeviceObjects();
	g_DialogResourceManager.OnDestroyDevice();
    text->destroyDeviceObjects();
    occ->destroyDeviceObjects();
	DXUTGetD3DDevice()->Release();
	
}