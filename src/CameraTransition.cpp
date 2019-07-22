#include "dxstdafx.h"
#include ".\cameratransition.h"
#include "camera.h"
#include "Player.h"

CameraTransition::CameraTransition(void)
{
  currPoint = -1;
  time = 0.0f;
  numPoints = 0;
  isFinished = false;
}

void CameraTransition::addTransitionPoint(D3DXVECTOR3 eye, D3DXVECTOR3 at, float l)
{
   transitionPoints[numPoints] = new sTransitionPoint(eye, at, l);
   numPoints++;
}


void CameraTransition::switchToNextTransitionPoint()
{
  currPoint++;
    
  //set new camera view params
  CPos eye, target;
  eye.set(transitionPoints[currPoint]->cameraPos.x,transitionPoints[currPoint]->cameraPos.y, transitionPoints[currPoint]->cameraPos.z);
  CCamera::setEye(eye);
  target.set(transitionPoints[currPoint]->lookAtPt.x, transitionPoints[currPoint]->lookAtPt.y, transitionPoints[currPoint]->lookAtPt.z);
  CCamera::setTarget(target);
  
  //build view matrix
  D3DXMATRIX mView;
  D3DXVECTOR3 lookAtDir = transitionPoints[currPoint]->lookAtPt.operator -(transitionPoints[currPoint]->cameraPos);
  D3DXMatrixLookAtLH(&mView, &transitionPoints[currPoint]->cameraPos, &lookAtDir, &D3DXVECTOR3(0, 1, 0));
  
}

//returns true if finished proccesing all transition points
bool CameraTransition::frameMove()
{
  //init view to first transition point
  static bool init = false;
  if(init == false){
    switchToNextTransitionPoint();
    init = true;
  }

  if(!isFinished){
	//switch to next tansition point if time is up
	if(time >= transitionPoints[currPoint]->length){
		if(currPoint < numPoints-1){
		switchToNextTransitionPoint();
		time = 0;
		}
		else{
		isFinished = true;
		return isFinished;
		}
	}

	time = time + DXUTGetElapsedTime();
  }

  return isFinished;
}






