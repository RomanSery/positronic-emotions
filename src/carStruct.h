#ifndef CAR_INFO
#define CAR_INFO

#include "NPCStructs.h"
enum Transmission
{
  DRIVE,
  PARK,
  REVERSE
};

struct carInfo{
 D3DXMATRIX     pos;		//updated by Newton
 D3DXVECTOR3    velocity; //updated by Newton
 Transmission	state;
 
 //bounding box
 sBoundingBox   *boundingBox;
 
 bool		    areLightsOn;
 bool			holdingDownBrake;

 carInfo(){
   boundingBox = new sBoundingBox(DOCTOR);
   boundingBox->height = 28;
   boundingBox->length = 40;
   boundingBox->width = 100;   
 }

};

#endif