//handels input control for the player including movement and camera movement
//also sets the animation depending on what the player is doing

#pragma once
#include "MeshHeirarchy.h"
#include "camera.h"
#include "vector.h"
#include <dinput.h>
#include "Frustum.h"
#include "structs.h"


#define HEIGHTADJUST   -6

class PlayerControl {
 public:
  static bool isMouseDown;
  
 private:
  //animation variables
  CXfileEntity			      *m_xFileEntity[2];
  static int				  IDLE_ID, WALK_ID, currSet;
    
  //camera objects
  CCamera					  *cam;
  CPos					      pos;

  //input
  LPDIRECTINPUTDEVICE8		  mouse;	 
  LPDIRECTINPUT8			  g_pDI;	
  DIMOUSESTATE				  state;	
  static bool				  canMove;
  static bool			      canMoveMouse;
  static bool				  canEnterLeave;
  
  //character status
  float						  facing;
  int						  m_currentAnimation;
  
  //variables for CNTRL+mouse camera movement
  CPos						  tempEye;
  CPos						  tempTarget;
  bool						  isY;

  //input
  void	readKeyboardInput(playerInfo *sInfo);
  void	readMouseInput(float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice, playerInfo *sInfo);

  //animation functions
  void	setAnimationStatus(int n);
  int	getAnimationStatus(){return m_currentAnimation;}
  void	setViewMatrix(LPDIRECT3DDEVICE9 device);
  static void  transformHuman(){  WALK_ID = 1; IDLE_ID = 0; }
  
  void updatePlayerInfo(playerInfo *sPlayerInfo);
  
public:
	PlayerControl(void);

	//player called functions
	void InitDeviceObjects(LPDIRECT3DDEVICE9 device);
	void RestoreDeviceObjects(LPDIRECT3DDEVICE9 m_pd3dDevice);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
	void FrameMove(playerInfo *sInfo, float m_fElapsedTime, LPDIRECT3DDEVICE9 m_pd3dDevice);	
	
	CXfileEntity* getXFileEntity() { return m_xFileEntity[currSet];}
	float		  getFacing() {return facing;}
	CCamera*	  getCamera() {return cam;}	
	void		  readOtherControls(playerInfo *sInfo);

 friend class Player;
};
