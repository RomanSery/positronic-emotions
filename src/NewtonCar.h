//defines classes for the car, normal tires, and tires that steer

#ifndef NEWTON_CAR
#define NEWTON_CAR

#include "dxstdafx.h"
#include "Newton.h"
#include "camera.h"
#include "CarControl.h"
#include "carStruct.h"
#include "d3dFont.h"

#define TIRE_COLLITION_ID	0x100
#define CHASIS_COLLITION_ID	0x200

class NewtonVehicle;

class NewtonTire{
 private:
  int     id;
  float   torque;
  float   brake;
  float   radius;
  float   width;
  D3DXMATRIX posMatrix;

 public: 
  void create(NewtonVehicle *vehicle, D3DXMATRIX mat, float radius, float width);
  virtual void setTirePhysics(const NewtonJoint *joint, int tireID);
  void render(IDirect3DDevice9 *device, CDXUTMesh *wheel, ID3DXEffect *effect,
			  D3DXMATRIX matView, D3DXMATRIX proj, D3DXVECTOR3 eyePt);
  
 friend class NewtonVehicle;
 friend class NewtonSteeringTire;
};   


class NewtonSteeringTire : NewtonTire
{
 private:
  float steerAngle;
 public:
  NewtonSteeringTire();
  void setTirePhysics(const NewtonJoint *joint, int tireID);

 friend class NewtonVehicle;
};



class NewtonVehicle {

 private:
  NewtonSteeringTire *flTire;
  NewtonSteeringTire *frTire;
  NewtonTire         *rlTire;
  NewtonTire         *rrTire;
	 
  NewtonJoint		 *joint;
  NewtonBody		 *vehicleBody;
  
  carInfo			 *sCarInfo;
  CarControl		 *cControl;

  ID3DXEffect           *effect;
  IDirect3DCubeTexture9 *cubeTex;
  
  
  bool		isUnderPlayerControl;

 public:
  NewtonVehicle();
  void setSteeringAngle(float angle);
  void setTorque(float torque);    
  void setBrake(float brake);
  void render(CDXUTMesh *car, CDXUTMesh *wheel, IDirect3DDevice9 *device,
			  D3DXMATRIX matView, D3DXVECTOR3 eyePt);
  void processInput();

  NewtonBody*  GetRigidBody()     { return vehicleBody;}
  NewtonJoint* GetJoint()         { return joint;}
  D3DXMATRIX   getPos()			  { return sCarInfo->pos;}
  D3DXVECTOR3  getVecPos()        { return D3DXVECTOR3(sCarInfo->pos._41,sCarInfo->pos._42,sCarInfo->pos._43); }
  D3DXVECTOR3  getVel()			  {	return sCarInfo->velocity; }	
  carInfo*	   getCarInfo()		  { return sCarInfo; }
  CarControl*  getCarControl()    { return cControl; }
  void		   setCarInfo(D3DXMATRIX p, D3DXVECTOR3 vel)
			   {sCarInfo->pos = p; sCarInfo->velocity = vel;}
			   void		   setPlayerControl(bool b) {isUnderPlayerControl = b; }
  
  void		   frameMove(IDirect3DDevice9 *device);

  //callbacks
  static void _cdecl SetTransform (const NewtonBody* body, const float* matrixPtr);
  static void _cdecl TireUpdate (const NewtonJoint* vehicle);
  static void _cdecl PhysicsApplyGravityForce(const NewtonBody *body);
  
  //init functions
  void createCar();
  void create(NewtonWorld *nWorld, D3DXMATRIX mat);  

  void initDeviceObjects(IDirect3DDevice9* device);
  void restoreDeviceObjects(IDirect3DDevice9 *device);
  void invalidateDeviceObjects();
  void destroyDeviceObjects();
  
  
  
  friend class CarManager;
};

#endif
