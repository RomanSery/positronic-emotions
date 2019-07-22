//control class for the car that is used when player is driving car
//handls input, setting the proper viewing paramaters, and the movement of the camera

#ifndef CAR_CONTROL
#define CAR_CONTROL

#include "dxstdafx.h"
#include "camera.h"
#include "carStruct.h"


class CarControl
{
 private:
  CCamera	  *cam;
  D3DXVECTOR3 eyePt;
  D3DXMATRIX  matView;
   
 public:
	CarControl();
    
	void   frameMove(carInfo *info, IDirect3DDevice9 *device);
    void   processInput(carInfo *info);

	D3DXMATRIX  getMatView() {return matView; }
	D3DXVECTOR3 getEyePt()  {return eyePt; }

	
};

#endif