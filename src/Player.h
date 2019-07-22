//represents the player

#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "Frustum.h"
#include "Newton/newton.h"
#include "structs.h"
#include "PlayerControl.h"

#define HEIGHTADJUST   -6


class Player {
 public:
  static playerInfo		*sPlayerInfo;  
   
  static void   transformToHuman(); 
  static void   setInputControls(bool m, bool e, bool moveM = true);	

 private:
  
  PlayerControl		*pControl;
  cFrustum			*frustum;

  
 public:
  Player();
  
  //application called functions
  HRESULT	  InitDeviceObjects(LPDIRECT3DDEVICE9 device);
  HRESULT	  Render(LPDIRECT3DDEVICE9 m_pd3dDevice, bool useTextures,IDirect3DCubeTexture9 *ct);
  void		  renderText();
  HRESULT	  FrameMove(float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice, D3DXMATRIX carView);	
  HRESULT	  RestoreDeviceObjects(LPDIRECT3DDEVICE9 m_pd3dDevice);
  void		  invalidateDeviceObjects();
  void		  destroyDeviceObjects();
    
  
  cFrustum*   getFrustum(){ return frustum;}
  playerInfo* getPlayerInfo() { return sPlayerInfo; }
  PlayerControl* getPlayerCntrl() { return pControl; }
       
  
};

#endif