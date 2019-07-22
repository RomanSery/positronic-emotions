//**************************************************************************************************
//
// CXfileEntity.cpp
//
// Description: Code for loading and rendering .x files with animation
//
// Created by: Keith Ditchburn 18/12/2003
//
//**************************************************************************************************


#include "dxstdafx.h"
#include "XfileEntity.h"
#include "MeshHeirarchy.h"
#include <assert.h>
#include "Player.h"
#include "playerFuncs.h"

// The time to change from one animation set to another
// To see how the merging works - increase this time value to slow it down
const float kMoveTransitionTime=0.25f;

D3DXHANDLE CXfileEntity::technique = "render";

/**************************************************************************************************
Desc: Constructor: Set all member variables to defaults.
Date: 20/12/2003
**************************************************************************************************/
CXfileEntity::CXfileEntity()
{
    m_frameRoot=NULL;
    m_animController=NULL;
	m_pd3dDevice=NULL;
	m_currentAnimationSet=0;
	m_currentTime=0.0f;
	m_currentTrack=0;
}

/**************************************************************************************************
**************************************************************************************************/
CXfileEntity::~CXfileEntity()
{
	DeleteDeviceObjects();
}

/**************************************************************************************************
Desc: Load the .x file with the passed filename. Additionally we make a local copy of the d3d device
Note: Returns D3D result code
Date: 15/11/2003
**************************************************************************************************/
HRESULT CXfileEntity::LoadXFile(LPDIRECT3DDEVICE9 device, LPCWSTR filename)
{
	m_pd3dDevice=device;
	HRESULT hr;
	// Create a mesh heirarchy class to control the allocation of memory for the frame heirarchy
    CAllocateHierarchy Alloc;
	      
	hr = D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, m_pd3dDevice, &Alloc, NULL, &m_frameRoot, &m_animController);
	
	return hr;
}

/**************************************************************************************************
Desc: Called each frame update with the time and the current world matrix
Date: 15/11/2003
**************************************************************************************************/
void CXfileEntity::FrameMove(float elapsedTime, D3DXMATRIX matWorld)
{
	// Adjust animation speed:
	elapsedTime/=m_speedAdjust;

	// Advance the time
    if (m_animController != NULL)
        m_animController->AdvanceTime(elapsedTime, NULL);


	m_currentTime+=elapsedTime;

	// Update the model matrices
    UpdateFrameMatrices(m_frameRoot, &matWorld);
}

//--------------------------------------------------------------------------------------
// update the frame matrices
//--------------------------------------------------------------------------------------
void CXfileEntity::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    if (pParentMatrix != NULL)
        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
    else
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    if (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

//--------------------------------------------------------------------------------------
// Called to render a frame in the hierarchy
//--------------------------------------------------------------------------------------
void CXfileEntity::DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame, int renderCode, IDirect3DCubeTexture9 *cubeTexture)
{
    LPD3DXMESHCONTAINER pMeshContainer;

    pMeshContainer = pFrame->pMeshContainer;
	
    while (pMeshContainer != NULL)
    {
        DrawMeshContainer( pd3dDevice, pMeshContainer, pFrame, renderCode ,cubeTexture);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameSibling, renderCode, cubeTexture);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameFirstChild, renderCode , cubeTexture);
    }
}
void CXfileEntity::initDeviceObjects(IDirect3DDevice9 *device)
{
	WCHAR str[100];
    DXUTFindDXSDKMediaFileCch( str, 100, L"terminator.fx" );
    D3DXCreateEffectFromFile(device, str, NULL, NULL, D3DXFX_NOT_CLONEABLE,NULL,&effect, NULL);

}

void CXfileEntity::restoreDeviceObjects(IDirect3DDevice9 *device)
{
	effect->OnResetDevice();
}

//--------------------------------------------------------------------------------------
// Called to render a mesh in the hierarchy
// renderCode = {0:robot form, 1:human form, -1:other}
void CXfileEntity::DrawMeshContainer( IDirect3DDevice9 *pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase,
									  int renderCode, IDirect3DCubeTexture9 *cubeTexture)
{
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    
	if(renderCode == ROBOT_FORM_CODE){ //used for rendering the player character
        renderPlayerRobotForm(effect, pMeshContainer, pFrame, cubeTexture, technique);
	}
    else if(renderCode == HUMAN_FORM_CODE){//human form
	    renderPlayerHumanForm(effect, pMeshContainer, pFrame);
    }
	else if(renderCode == NPC_CODE || renderCode == SPACESHIP_CODE){  //used for rendering the NPC characters
	  // standard mesh, just draw it after setting material properties
	  for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
        pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
	    pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
        pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
		pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);		
	  }
	}
}

/**************************************************************************************************
Desc: Called when the d3d device is about to be deleted
Date: 15/11/2003
**************************************************************************************************/
HRESULT CXfileEntity::DeleteDeviceObjects()
{
	m_pd3dDevice=NULL;

	// // Create a mesh heirarchy class to control the removal of memory for the frame heirarchy	
	if (m_frameRoot)
	{
		CAllocateHierarchy Alloc;
		D3DXFrameDestroy(m_frameRoot, &Alloc);

		m_frameRoot=NULL;
	}

    SAFE_RELEASE(m_animController);
	SAFE_RELEASE(effect);

	return S_OK;
}

/**************************************************************************************************
Desc: Call the DrawFrame recursive fn on render with the root frame (see notes diagram)
Date: 15/11/2003
**************************************************************************************************/
void CXfileEntity::Render(IDirect3DDevice9 *pd3dDevice, int renderCode, IDirect3DCubeTexture9 *cubeTexture)
{
    DrawFrame(pd3dDevice, m_frameRoot, renderCode, cubeTexture);
    
}

/**************************************************************************************************
Desc: Slow down animation
Date: 15/11/2003
**************************************************************************************************/
void CXfileEntity::AnimateSlower()
{
	m_speedAdjust+=0.1f;
}

/**************************************************************************************************
Desc: Speed up animation
Date: 15/11/2003
**************************************************************************************************/
void CXfileEntity::AnimateFaster()
{
	m_speedAdjust-=0.1f;

	if (m_speedAdjust<=0.0f)
		m_speedAdjust=0.1f;
}

/**************************************************************************************************
Desc: Take the animation sets in passed Xfile and adds it to this instance
Note: Must refer to same mesh heirarchy
Date: 19/12/2003
**************************************************************************************************/
bool CXfileEntity::AddAnimationSet(CXfileEntity *entity)
{
	assert(entity);

	UINT numToCopy=entity->GetNumberOfAnimationSets();

	if (numToCopy==0)
	{
		//OutputDebugString("File has no animation sets\n");
		return TRUE;
	}

	// Check the controller has room
	UINT numPossible=m_animController->GetMaxNumAnimationSets();
	UINT numAlready=m_animController->GetNumAnimationSets();

	if (numPossible-numAlready<numToCopy)
	{
		// There is not enough room in the controller for more animation sets
		// Since we cannot dynamically increase the room we need to create a new
		// Controller with enough room - we use CloneAnimationController to do this
	
		LPD3DXANIMATIONCONTROLLER newController=NULL;
		HRESULT hr=m_animController->CloneAnimationController(
			m_animController->GetMaxNumAnimationOutputs(),
			numAlready+numToCopy,
			2,	// number of required tracks
			m_animController->GetMaxNumEvents(),
			&newController);

		if (FAILED(hr))
			return FALSE;

		// Release existing controller
		SAFE_RELEASE(m_animController);

		// Now set this to new one just created
		m_animController=newController;
	}

	// Copy all the new animation sets - note: often just one
	for (UINT i=0;i<numToCopy;i++)
	{
		LPD3DXANIMATIONSET animSet=entity->GetAnimationSet(i);
		if (animSet==NULL)
			return FALSE;


		// Add it to our controller
		HRESULT hr=m_animController->RegisterAnimationSet(animSet);
		if (FAILED(hr))
			return FALSE;
	}

	return TRUE;
}

/**************************************************************************************************
Desc: Returns number of animation sets loaded for this model
Date: 19/12/2003
**************************************************************************************************/
UINT CXfileEntity::GetNumberOfAnimationSets() const
{
	if (!m_animController)
		return 0;

	return m_animController->GetNumAnimationSets();
}

/**************************************************************************************************
Desc: Retrieve an animation set by index
Date: 19/12/2003
**************************************************************************************************/
const LPD3DXANIMATIONSET CXfileEntity::GetAnimationSet(UINT index) const
{
	assert(index<GetNumberOfAnimationSets());
	assert(m_animController);

	LPD3DXANIMATIONSET ppAnimSet;

	HRESULT hr=m_animController->GetAnimationSet(index,&ppAnimSet);
	if (FAILED(hr))
		return NULL;

	return ppAnimSet;
}

/**************************************************************************************************
Desc: Change to use the next animation set numerically
Date: 19/12/2003
**************************************************************************************************/
void CXfileEntity::SelectNextAnimationSet()
{
	m_currentAnimationSet++;

	if (m_currentAnimationSet>=GetNumberOfAnimationSets())
		m_currentAnimationSet=0;

	SetAnimationSet(m_currentAnimationSet);
}

/**************************************************************************************************
Desc: Change to a different animation set
Note: Handles transitions between animations to make it smooth and not a sudden jerk to a new position
Date: 19/12/2003
**************************************************************************************************/
void CXfileEntity::SetAnimationSet(UINT index)
{
	if (index>=GetNumberOfAnimationSets())
		return;

	m_currentAnimationSet=index;

	// Get the animation set
	LPD3DXANIMATIONSET set;
	m_animController->GetAnimationSet(m_currentAnimationSet, &set );

	DWORD newTrack = ( m_currentTrack == 0 ? 1 : 0 );

	// Assign to our track
	m_animController->SetTrackAnimationSet( newTrack, set );
    set->Release();		

	// Note: for a smooth transition between animation sets we can use two tracks and assign the new set to the track
	// not currently playing then insert Keys into the KeyTrack to do the transition between the tracks
	// tracks can be mixed together so we can gradually change into the new animation

	// Clear any track events currently assigned to our two tracks
	m_animController->UnkeyAllTrackEvents( m_currentTrack );
    m_animController->UnkeyAllTrackEvents( newTrack );

	// Add an event key to disable the currently playing track kMoveTransitionTime seconds in the future
    m_animController->KeyTrackEnable( m_currentTrack, FALSE, m_currentTime + kMoveTransitionTime );
	// Add an event key to change the speed right away so the animation completes in kMoveTransitionTime seconds
    m_animController->KeyTrackSpeed( m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the secon track)
    m_animController->KeyTrackWeight( m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR );

	// Enable the new track
    m_animController->SetTrackEnable( newTrack, TRUE );
	// Add an event key to set the speed of the track
    m_animController->KeyTrackSpeed( newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR );
	// Add an event to change the weighting of the current track (the effect it has blended with the first track)
	// As you can see this will go from 0 effect to total effect(1.0f) in kMoveTransitionTime seconds and the first track goes from 
	// total to 0.0f in the same time.
    m_animController->KeyTrackWeight( newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR );

    m_currentTrack = newTrack;

}

/**************************************************************************************************
Desc: Save this x file to filename
Note: If it has animation sets combined into it we save them all as well so after loading a number
of .x files with the same model but different animation sets and adding their sets to this x file
we can reduce to just having to use one .x file. You can also use the DirectX MeshView program to do this
manually.
[note: unfortunately the example .x files I use have an adjacency error that causes problems with this]
Date: 20/12/2003
**************************************************************************************************/
bool CXfileEntity::Save(LPCWSTR filename)
{
	assert(filename);

	HRESULT hr=D3DXSaveMeshHierarchyToFile(filename,0,m_frameRoot,m_animController,NULL);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}
