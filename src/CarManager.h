//manages all the cars in the world. For now there is only one car so this class is
//just framework code.


#ifndef CAR_MANAGER
#define CAR_MANAGER

#include "NewtonCar.h"
#include "Frustum.h"
#include "structs.h"


class CarManager {
  private:
	
    	
	//collection of car meshes
	CDXUTMesh		*mercCar;
    CDXUTMesh		*mercWheel;
  public:
	CarManager(void);
	
	//application called functions
	void InitDeviceObjects(IDirect3DDevice9 *device);
	void RestoreDeviceObjects(IDirect3DDevice9 *device);
	void invalidateDeviceObjects();
	void destroyDeviceObjects();
	void frameMove(IDirect3DDevice9 *device);
	void render(IDirect3DDevice9 *device, cFrustum *f);

	void addCars(NewtonWorld *nWorld, int matID);
   	
    NewtonVehicle *car;

    friend class NewtonObjects;
};

#endif