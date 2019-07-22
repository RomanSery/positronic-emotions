//**************************************************************************************************
// Created by: Keith Ditchburn 18/12/2003
//**************************************************************************************************

#if !defined(AFX_XFILEENTITY_H__8A0008F8_D5E7_4797_B75E_F98E1EE865E0__INCLUDED_)
#define AFX_XFILEENTITY_H__8A0008F8_D5E7_4797_B75E_F98E1EE865E0__INCLUDED_

#include "dxstdafx.h"


class CXfileEntity  
{
private:
    LPD3DXFRAME                 m_frameRoot;
    
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	float						m_speedAdjust;
	UINT						m_currentAnimationSet;
	float						m_currentTime;
	DWORD						m_currentTrack;
	

	//effect stuff
	ID3DXEffect         *effect;
		

	void  DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame, int renderCode, IDirect3DCubeTexture9 *cubeTexture );
	void  DrawMeshContainer( IDirect3DDevice9 *pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, int renderCode, IDirect3DCubeTexture9 *cubeTexture );
	void  UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	const LPD3DXANIMATIONSET GetAnimationSet(UINT index) const;



public:
	//effect functions
	static D3DXHANDLE technique;
	void initDeviceObjects(IDirect3DDevice9 *device);
	void restoreDeviceObjects(IDirect3DDevice9 *device);
		
	
	bool Save(LPCWSTR filename);
	bool AddAnimationSet(CXfileEntity *entity);
	void SetAnimationSet(UINT index);
	void SelectNextAnimationSet();
	
	UINT GetNumberOfAnimationSets() const;
    UINT getCurrAnim() { return m_currentAnimationSet; }

	void AnimateFaster();
	void AnimateSlower();
	void Render(IDirect3DDevice9 *pd3dDevice, int renderCode, IDirect3DCubeTexture9 *cubeTexture);

	HRESULT DeleteDeviceObjects();
	void FrameMove(float elapsedTime, D3DXMATRIX matWorld);

	HRESULT LoadXFile(LPDIRECT3DDEVICE9 device, LPCWSTR filename);

	CXfileEntity();
	virtual ~CXfileEntity();

	inline float GetAnimationAdjuster() const {return m_speedAdjust;};
	inline void  setAnimationSpeed(float f) {m_speedAdjust = f;}
    
    LPD3DXANIMATIONCONTROLLER   m_animController;
};

#endif
