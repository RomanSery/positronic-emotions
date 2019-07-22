
#include "dxstdafx.h"
#include "NewtonCar.h"
#include "carFuncs.h"
#include "Renderer.h"

void NewtonTire::create(NewtonVehicle *vehicle, D3DXMATRIX mat, float r, float w)
{
  float tireMass = 2.0f;
  float tireSuspensionShock = 0.8f;
  float tireSuspensionSpring = 0.8f;
  float tireSuspensionLength = 0.2f;
  D3DXVECTOR3 tirePin(0,0,1);

  radius = r;
  width = w;

  // Now add the tire to the vehicle joint and retrive it's internal ID (for later use)
  id = NewtonVehicleAddTire(vehicle->GetJoint(), (FLOAT *)mat, (FLOAT *)tirePin, tireMass, w,
							r, tireSuspensionShock, tireSuspensionSpring, tireSuspensionLength, this, TIRE_COLLITION_ID);

  // Set the maximum rate (angular) at which the wheel can rotate before loosing traction
  NewtonVehicleSetTireMaxLongitudinalSlideSpeed(vehicle->GetJoint(), id, 10000);

  torque = 0;
  brake = 0;
}

void NewtonTire::setTirePhysics(const NewtonJoint *joint, int tireID)
{
  float accel;
  float omega;
  float speed;

  // Get tire's angular velocity (angular velocity = omega)
  omega = NewtonVehicleGetTireOmega(joint, tireID);
 omega *= 10000;
  // Add some viscuos damp to the tire torque (this prevents out of control spin)
  NewtonVehicleSetTireTorque(joint, tireID, torque);// - 0.1f * omega);
  
  // calculate the tire speed at the contact
  // set the max side slip speed as a fraction of the tire spm
  speed = radius * omega;
  NewtonVehicleSetTireMaxSideSleepSpeed (joint, tireID, speed * 0.1f);

  // The side slip is usually propostinal to the tire longitudilnal speed, and tire load
  NewtonVehicleSetTireSideSleepCoeficient (joint, tireID, speed * 0.5f);

 
  // Braking
  if(brake > 0){
    // Get acceleration that's needed to stop the wheel
    accel = NewtonVehicleTireCalculateMaxBrakeAcceleration(joint, tireID);
   
    // Brake the wheel
    NewtonVehicleTireSetBrakeAcceleration(joint, tireID, accel, 8000.0f * brake);

	// set some side slipe as funtion of the linear speed 
	speed = NewtonVehicleGetTireLongitudinalSpeed (joint, tireID);
	NewtonVehicleSetTireMaxSideSleepSpeed (joint, tireID, speed * 0.1f);
  }

  // Reset brake
  brake = 0;

  // Update tire matrix
  NewtonVehicleGetTireMatrix(joint, id, (FLOAT *)posMatrix);
}

void NewtonTire::render(IDirect3DDevice9 *device, CDXUTMesh *wheel, ID3DXEffect *effect,
						D3DXMATRIX matView, D3DXMATRIX proj, D3DXVECTOR3 eyePt)
{
  
  if( Renderer::SUPPORTS_SHADERS_1_1)
    renderWheelsWithShader(device, effect, wheel, posMatrix, matView, proj, eyePt);
  else
    renderWithoutShader(device, wheel, posMatrix);
}

// *****************************************************************************
//  NewtonSteeringTire
// *****************************************************************************
NewtonSteeringTire::NewtonSteeringTire()
{
  steerAngle = 0.0f;

}

void NewtonSteeringTire::setTirePhysics(const NewtonJoint *joint, int tireID)
{
  float currSteerAngle;

  //Update steering angle
  currSteerAngle = NewtonVehicleGetTireSteerAngle(joint, tireID);
  NewtonVehicleSetTireSteerAngle(joint, tireID, currSteerAngle+(steerAngle-currSteerAngle) * 0.25f);
  
  //Call inherited SetTirePhysics for the rest of the work
  NewtonTire::setTirePhysics(joint, tireID);

}



// *****************************************************************************
//  NewtonVehicle
// *****************************************************************************
NewtonVehicle::NewtonVehicle()
{
  sCarInfo = new carInfo();
  cControl = new CarControl();
  sCarInfo->state = PARK;

  isUnderPlayerControl = false;
  
}

void NewtonVehicle::frameMove(IDirect3DDevice9 *device)
{
	if(isUnderPlayerControl)
	 cControl->frameMove(sCarInfo, device);
}

void NewtonVehicle::create(NewtonWorld *nWorld, D3DXMATRIX mat)
{
  float mass, Ixx, Iyy, Izz;
  NewtonCollision *collider;
  D3DXVECTOR3 upDir(0,1,0);
  D3DXVECTOR3 boxSize(100, 5, 40);
   
  //first we make a normal rigid body
  mass = 100.0f;
  Ixx = mass * (boxSize.y * boxSize.y + boxSize.z * boxSize.z) / 12;
  Iyy = mass * (boxSize.x * boxSize.x + boxSize.z * boxSize.z) / 12;
  Izz = mass * (boxSize.x * boxSize.x + boxSize.y * boxSize.y) / 12;

  collider = NewtonCreateBox(nWorld, boxSize.x,boxSize.y,boxSize.z, NULL);
  vehicleBody = NewtonCreateBody(nWorld, collider);

  //Release the Collider
  NewtonReleaseCollision(nWorld, collider);
  // Set user data (so that we can directly access the class in callbacks)
  NewtonBodySetUserData(vehicleBody, this);
  // Set transform callback
  NewtonBodySetTransformCallback(vehicleBody, SetTransform);
  // Set the force and torque call back funtion
  NewtonBodySetForceAndTorqueCallback(vehicleBody, PhysicsApplyGravityForce);
  // Set mass matrix
  NewtonBodySetMassMatrix(vehicleBody, mass, Ixx, Iyy, Izz);
  // Set the matrix for the rigid body
  NewtonBodySetMatrix(vehicleBody, (FLOAT *)mat);

  //second we need to add a vehicle joint to the body
  joint = NewtonConstraintCreateVehicle(nWorld, (FLOAT *)upDir, vehicleBody);
  // Set the callback for the tires
  NewtonVehicleSetTireCallback(joint, TireUpdate);
}

void NewtonVehicle::createCar()
{
  D3DXMATRIX tireMatrix;
  float z = -13.0f;	//height
  float bx = 27.0f;	//back wheels spacing from bumper to bumper
  float fx = 34.0f;	//front wheels spacing from bumper to bumper

  float y = 16.0f;	//wheel spacing from side mirror to side mirror
  
  float r = 8.5f;
  float w = 8.0f;

 	
  //First we create two static wheels (back)
  D3DXMatrixIdentity(&tireMatrix);D3DXMatrixTranslation(&tireMatrix, bx, z,  y);
  rrTire = new NewtonTire();
  rrTire->create(this, tireMatrix, r, w); 
  
  D3DXMatrixIdentity(&tireMatrix);D3DXMatrixTranslation(&tireMatrix, bx, z, -y);
  rlTire = new NewtonTire();
  rlTire->create(this, tireMatrix, r, w); 
   
  //And now two steering tires (front)
  D3DXMatrixIdentity(&tireMatrix);D3DXMatrixTranslation(&tireMatrix, -fx, z, -y);
  frTire = new NewtonSteeringTire();
  frTire->create(this, tireMatrix, r, w); 
  
  D3DXMatrixIdentity(&tireMatrix);D3DXMatrixTranslation(&tireMatrix, -fx, z,  y);
  flTire = new NewtonSteeringTire();
  flTire->create(this, tireMatrix, r, w); 
 
  // When we have attached all wheels, we MUST balance the tires
  NewtonVehicleBalanceTires(joint, -10);
}

void NewtonVehicle::setSteeringAngle(float a)
{
	frTire->steerAngle = a;
	flTire->steerAngle = a;
		
}

void NewtonVehicle::setTorque(float torque)
{
	frTire->torque = torque;
	flTire->torque = torque;
	rrTire->torque = torque;
	rlTire->torque = torque;
	
}

void NewtonVehicle::setBrake(float brake)
{
	frTire->brake = brake;
	flTire->brake = brake;
	rrTire->brake = brake;
	rlTire->brake = brake;
}

void NewtonVehicle::processInput()
{
  float PI = 3.14f;
  
  //car steering control
  if (DXUTIsKeyDown('K') == TRUE && isUnderPlayerControl == true)
	setSteeringAngle(30 * PI/180);
  else if (DXUTIsKeyDown('H') == TRUE && isUnderPlayerControl == true)
	setSteeringAngle(-30 * PI/180);
  else
	setSteeringAngle(0.0f);
  
  //forware, backward movement
  if(sCarInfo->state != PARK && isUnderPlayerControl == true){
	if(sCarInfo->state == DRIVE){
	  if(DXUTIsKeyDown('U') == TRUE)
	    setTorque(3000.0f);
	  else
	    setTorque(800.0f);
	}
	
	if(sCarInfo->state == REVERSE){
	  if(DXUTIsKeyDown('U') == TRUE)
	    setTorque(-3000.0f);
	  else
	    setTorque(-800.0f);
	}

	if(DXUTIsKeyDown('J') == TRUE){
	  setBrake(5.0f);
	  setTorque(0.0f);
	  sCarInfo->holdingDownBrake = true;
	}
	else{ 
	  setBrake(0.0f);
	  sCarInfo->holdingDownBrake = false;
	}
  }
  else{
	setBrake(1.0f);
	setTorque(0.0f);
  }

  //other controls
  if(isUnderPlayerControl)
   cControl->processInput(sCarInfo);
   
}

void NewtonVehicle::render(CDXUTMesh *car, CDXUTMesh *wheel, IDirect3DDevice9 *device,
						   D3DXMATRIX matView, D3DXVECTOR3 eyePt)
{
  D3DXMATRIX proj;
  D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );
  
  effect->SetBool("areLightsOn", sCarInfo->areLightsOn);
  if( Renderer::SUPPORTS_SHADERS_1_1)
    renderCarWithShader(device, effect, car, sCarInfo->pos, matView, proj, eyePt, cubeTex);
  else
    renderWithoutShader(device, car, sCarInfo->pos);
  

  //render car wheels
  flTire->render(device, wheel, effect, matView, proj, eyePt);
  frTire->render(device, wheel, effect, matView, proj, eyePt);
  rlTire->render(device, wheel, effect, matView, proj, eyePt);
  rrTire->render(device, wheel, effect, matView, proj, eyePt);

  if(isUnderPlayerControl){
    char *state = "DRIVE";
    if(sCarInfo->state == PARK)
	  state = "PARK";
    else if(sCarInfo->state == REVERSE)
	  state = "REVERSE";
//    printString(font, state, 100, 10);
  }
  
}

void NewtonVehicle::TireUpdate(const NewtonJoint *joint)
{
  int tyreId;
  NewtonTire* tire = NULL;

  // iterate trough each tire applying the tire dynamics
  for (tyreId = NewtonVehicleGetFirstTireID (joint); tyreId; tyreId = NewtonVehicleGetNextTireID (joint, tyreId)) {
		// get the graphic object and set the transformation matrix 
		tire = (NewtonTire*) NewtonVehicleGetTireUserData(joint, tyreId);
		tire->setTirePhysics(joint, tyreId);
  }
}

void NewtonVehicle::SetTransform(const NewtonBody *body, const float *matrixPtr)
{
    D3DXVECTOR3 vel; int tyreId;
	NewtonTire* tire;
	NewtonVehicle* vehicle;

	// get the graphic object form the rigid body
	vehicle = (NewtonVehicle*) NewtonBodyGetUserData (body);
	NewtonBodyGetVelocity(body, (FLOAT *)vel);

	// set the transformation matrix for this rigid body
	D3DXMATRIX matrix(matrixPtr);
	vehicle->setCarInfo(matrix, vel);

    //Set the global matrix for each tire
	for (tyreId = NewtonVehicleGetFirstTireID (vehicle->joint); tyreId; tyreId = NewtonVehicleGetNextTireID (vehicle->joint, tyreId)) {

		// get the graphic object and set the transformation matrix 
		tire = (NewtonTire*) NewtonVehicleGetTireUserData (vehicle->joint, tyreId);
		NewtonVehicleGetTireMatrix (vehicle->joint, tyreId, (FLOAT *)tire->posMatrix);
	}
}

void NewtonVehicle::PhysicsApplyGravityForce(const NewtonBody *body)
{
  float mass, Ixx, Iyy, Izz;
  float speed = -20.0f;
  NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
  D3DXVECTOR3 force(0.0f, mass * speed, 0.0f);
  NewtonBodySetForce (body, (FLOAT *)force);

}

void NewtonVehicle::initDeviceObjects(IDirect3DDevice9 *device)
{
	WCHAR str[100];
    DXUTFindDXSDKMediaFileCch( str, 100, L"car.fx");
    D3DXCreateEffectFromFile(device, str, NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL,&effect, NULL);

	D3DXCreateCubeTextureFromFile(device, L"media/cubeMap.dds", &cubeTex);
	
}

void NewtonVehicle::restoreDeviceObjects(IDirect3DDevice9 *device)
{
	if(effect) effect->OnResetDevice();
}

void NewtonVehicle::invalidateDeviceObjects()
{
  effect->OnLostDevice();
  
  SAFE_RELEASE( cubeTex);
}

void NewtonVehicle::destroyDeviceObjects()
{
  SAFE_RELEASE( effect);
}
