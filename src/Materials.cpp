// Materials.cpp: implementation of the Materials class.
//
//////////////////////////////////////////////////////////////////////

#include "dxstdafx.h"
//#include "Newton.h"
#include "Materials.h"


int woodID; 
int metalID; 
int levelID;
int worldInteriorID; 
int vehicleID;
int characterID; 



// strutures use to hold diffrents materials interation 
SpecialEffectStruct wood_wood;
SpecialEffectStruct wood_metal;
SpecialEffectStruct wood_level;
SpecialEffectStruct metal_metal;
SpecialEffectStruct metal_level;
SpecialEffectStruct vehicle_level;


static SpecialEffectStruct* g_currectEffect;

// this callback is called when the two aabb boxes of the collisiong object overlap
int _cdecl GenericContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{

	// get the pointer to the special effect struture
	g_currectEffect = (SpecialEffectStruct *)NewtonMaterialGetMaterialPairUserData (material);

	// save the collisiong bodies
	g_currectEffect->m_body0 = (NewtonBody*) body0;
	g_currectEffect->m_body1 = (NewtonBody*) body1;

	// clear the contact normal speed 
	g_currectEffect->m_contactMaxNormalSpeed = 0.0f;

	// clear the contact sliding speed 
	g_currectEffect->m_contactMaxTangentSpeed = 0.0f;
	
	D3DXVECTOR3 zero(0,0,0);
	//NewtonBodySetVelocity(g_currectEffect->m_body0, (FLOAT *)zero);
	//NewtonBodySetVelocity(g_currectEffect->m_body1, (FLOAT *)zero);
	//NewtonBodySetOmega(g_currectEffect->m_body1, (FLOAT *)zero);
	//NewtonBodySetOmega(g_currectEffect->m_body0, (FLOAT *)zero);
	// return one the tell Newton the application wants to proccess this contact
	return 1;
}


// this callback is called for every contact between the two bodies
int _cdecl GenericContactProcess (const NewtonMaterial* material, const NewtonContact* contact)
{
	float speed0;
	float speed1;
	D3DXVECTOR3 normal;

	// Get the maximun normal speed of this impact. this can be used for particels of playing collision sound
	speed0 = NewtonMaterialGetContactNormalSpeed (material, contact);
	if (speed0 > g_currectEffect->m_contactMaxNormalSpeed) {
		// save the position of the contact (for 3d sound of particles effects)
		g_currectEffect->m_contactMaxNormalSpeed = speed0;
		NewtonMaterialGetContactPositionAndNormal (material, &g_currectEffect->m_position.x, &normal.x);
	}

	// get the maximun of the two sliding contact speed
	speed0 = NewtonMaterialGetContactTangentSpeed (material, contact, 0);
	speed1 = NewtonMaterialGetContactTangentSpeed (material, contact, 1);
	if (speed1 > speed0) {
		speed0 = speed1;
	}

	// Get the maximun tangent speed of this contact. this can be used for particles(sparks) of playing scratch sounds 
	if (speed0 > g_currectEffect->m_contactMaxTangentSpeed) {
		// save the position of the contact (for 3d sound of particles effects)
		g_currectEffect->m_contactMaxTangentSpeed = speed0;
		NewtonMaterialGetContactPositionAndNormal (material, &g_currectEffect->m_position.x, &normal.x);
	}
	D3DXVECTOR3 zero(0,0,0);
	//NewtonBodySetVelocity(g_currectEffect->m_body0, (FLOAT *)zero);
	//NewtonBodySetVelocity(g_currectEffect->m_body1, (FLOAT *)zero);
	//NewtonBodySetOmega(g_currectEffect->m_body1, (FLOAT *)zero);
	//NewtonBodySetOmega(g_currectEffect->m_body0, (FLOAT *)zero);
	// return one to tell Newton we want to accept this contact
	return 1;
}

// this function is call affter all contacts for this pairs is proccesed
void _cdecl GenericContactEnd (const NewtonMaterial* material)
{
	#define MIN_CONTACT_SPEED 15
	#define MIN_SCRATCH_SPEED 5

	// if the max contact speed is larger than some minumum value. play a sound
	if (g_currectEffect->m_contactMaxNormalSpeed > MIN_CONTACT_SPEED) {
		g_currectEffect->PlayImpactSound (g_currectEffect->m_contactMaxNormalSpeed - MIN_CONTACT_SPEED);
	}

	// if the max contact speed is larger than some minumum value. play a sound
	if (g_currectEffect->m_contactMaxNormalSpeed > MIN_SCRATCH_SPEED) {
		g_currectEffect->PlayScratchSound (g_currectEffect->m_contactMaxNormalSpeed - MIN_SCRATCH_SPEED);
	}

	// implement here any other effects
}





// this is use to constomized the chatarer contril contact vectors
int _cdecl CharacterContactProcess (const NewtonMaterial* material, const NewtonContact* contact)
{
	float mass;
	float Ixx;
	float Iyy;
	float Izz;
	NewtonBody* sphere;
	
	// apply the default behaviuor
	GenericContactProcess (material, contact);

	// get the spherical body, it is the body with non zero mass
	// this way to determine the body is quit and dirty but it no safe, it only work in this
	// case because one of the two bodies is the terrain which we now have infinite mass.
	// a better way is by getting the user data an finding some object identifier stored with the user data.
	sphere = g_currectEffect->m_body0;
	NewtonBodyGetMassMatrix (g_currectEffect->m_body0, &mass, &Ixx, &Iyy, &Izz);
	if (mass == 0.0f) {
		sphere = g_currectEffect->m_body1;
		NewtonBodyGetMassMatrix (g_currectEffect->m_body1, &mass, &Ixx, &Iyy, &Izz);
	}

	// align the tangent contact direction with the velocity vector of the ball
	D3DXVECTOR3 posit;
	D3DXVECTOR3 normal;
	
	D3DXVECTOR3 velocity;
	NewtonBodyGetVelocity(sphere, &velocity.x);
	NewtonMaterialGetContactPositionAndNormal (material, &posit.x, &normal.x);

	// calculate ball velocity perpendicular to the contact normal
	D3DXVECTOR3 tangentVelocity ((velocity - normal));//.Scale (normal % velocity));

	// align the tangent at the contact point with the tangent velocity vector of the ball
	NewtonMaterialContactRotateTangentDirections (material, &tangentVelocity.x);
	


	// return one to tell Newton we want to accept this contact
	return 1;
}

// this is use to constomized the chatarer control contact vectors
int _cdecl VehicleContactProcess (const NewtonMaterial* material, const NewtonContact* contact)
{
	//int collisionID;
	int faceMaterialID;
	float mass;
	float Ixx;
	float Iyy;
	float Izz;
	NewtonBody* car;
	
	// apply the default behaviuor
    //GenericContactProcess (material, contact);

	// get the car, it is the body with non zero mass
	// this way to determine the body is quit and dirty but it no safe, it only works in this
	// case because one of the two bodies is the terrain which we now have infinite mass.
	// a better way is by getting the user data an finding some object identifier stored with the user data.

	car = g_currectEffect->m_body0;
	NewtonBodyGetMassMatrix (g_currectEffect->m_body0, &mass, &Ixx, &Iyy, &Izz);
	if (mass == 0.0f) {
		car = g_currectEffect->m_body1;
	}
	
	// get the contact user data assigned to the terrain face generating the contact
	// get the contact user data assigned to the terrain face generating the contact
	faceMaterialID = NewtonMaterialGetContactFaceAttribute (material);

	// used the max allowable friction coeficenst 2.0f in Newton
	// set the friction values depending on the face material
	// first axis is the longitudilal tire friction (in general lateral and longitudinal friction should be equal)
	// but the apliication can play with this values to generate specials effects
	// first axis is the longitudilal tire friction
	NewtonMaterialSetContactStaticFrictionCoef (material, 2.0f, 0);
	NewtonMaterialSetContactKineticFrictionCoef (material, 1.5f, 0);

	// second axis is the lateral tire friction (in general lateral and longitudinal friction should be equal)
	// but the aplication can play with this values to generate specials effects
	NewtonMaterialSetContactStaticFrictionCoef (material, 2.0f, 1);
	NewtonMaterialSetContactKineticFrictionCoef (material, 1.5f, 1);


	// return one to tell Newton we want to accept this contact
	return 1;
}


// initialize all material and material interations
void SetupMaterials (NewtonWorld* nWorld)
{
	int defaultID;

	// initialize the material intaration here
	wood_wood.Init ("wood_wood");
	wood_metal.Init ("wood_metal");
	wood_level.Init ("wood_level");
	metal_metal.Init ("metal_metal");
	metal_level.Init ("metal_level");
	
	
	// get the defuult material ID
	defaultID = NewtonMaterialGetDefaultGroupID (nWorld);

	// set default material properties
	NewtonMaterialSetDefaultSoftness (nWorld, defaultID, defaultID, 0.5f);
	NewtonMaterialSetDefaultElasticity (nWorld, defaultID, defaultID, 0.4f);
	NewtonMaterialSetDefaultCollidable (nWorld, defaultID, defaultID, 1);
	NewtonMaterialSetDefaultFriction (nWorld, defaultID, defaultID, 1.0f, 0.5f);
	NewtonMaterialSetCollisionCallback (nWorld, woodID, woodID, &wood_wood, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	
	// create all materials ID
	woodID = NewtonMaterialCreateGroupID(nWorld);
	metalID = NewtonMaterialCreateGroupID(nWorld);
	levelID = NewtonMaterialCreateGroupID(nWorld);
    worldInteriorID = NewtonMaterialCreateGroupID(nWorld);
	characterID = NewtonMaterialCreateGroupID(nWorld);

	// set the material properties for wood on wood
	NewtonMaterialSetDefaultElasticity (nWorld, woodID, woodID, 0.3f);
	NewtonMaterialSetDefaultFriction (nWorld, woodID, woodID, 1.1f, 0.7f);
	NewtonMaterialSetCollisionCallback (nWorld, woodID, woodID, &wood_wood, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for wood on metal
	NewtonMaterialSetDefaultElasticity (nWorld, woodID, metalID, 0.5f);
	NewtonMaterialSetDefaultFriction (nWorld, woodID, metalID, 0.8f, 0.6f);
	NewtonMaterialSetCollisionCallback (nWorld, woodID, metalID, &wood_metal, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for wood on levell
	NewtonMaterialSetDefaultElasticity (nWorld, woodID, levelID, 0.3f);
	NewtonMaterialSetDefaultFriction (nWorld, woodID, levelID, 0.9f, 0.3f);
	NewtonMaterialSetCollisionCallback (nWorld, woodID, levelID, &wood_level, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for metal on metal
	NewtonMaterialSetDefaultElasticity (nWorld, metalID, metalID, 0.7f);
	NewtonMaterialSetDefaultFriction (nWorld, metalID, metalID, 0.5f, 0.2f);
	NewtonMaterialSetCollisionCallback (nWorld, metalID, metalID, &metal_metal, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for metal on level
	NewtonMaterialSetDefaultElasticity (nWorld, metalID, levelID, 0.4f);
	NewtonMaterialSetDefaultFriction (nWorld, metalID, levelID, 0.6f, 0.4f);
	NewtonMaterialSetCollisionCallback (nWorld, metalID, levelID, &metal_level, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// level on level is will never hapens


	// create the charater material interactions
	NewtonMaterialSetDefaultElasticity (nWorld, woodID, characterID, 0.3f);
	NewtonMaterialSetDefaultFriction (nWorld, woodID, characterID, 1.1f, 0.7f);
	NewtonMaterialSetCollisionCallback (nWorld, woodID, characterID, &wood_wood, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for metal on metal
	NewtonMaterialSetDefaultElasticity (nWorld, metalID, characterID, 0.7f);
	NewtonMaterialSetDefaultFriction (nWorld, metalID, characterID, 0.5f, 0.2f);
	NewtonMaterialSetCollisionCallback (nWorld, metalID, characterID, &metal_metal, GenericContactBegin, GenericContactProcess, GenericContactEnd); 

	// set the material properties for character on level 
	NewtonMaterialSetDefaultElasticity (nWorld, levelID, characterID, 0.1f);
	NewtonMaterialSetDefaultFriction (nWorld, levelID, characterID, 0.6f, 0.6f);
	NewtonMaterialSetCollisionCallback (nWorld, levelID, characterID, &metal_level, GenericContactBegin, CharacterContactProcess, GenericContactEnd); 
 
    // set the material properties for character on interior 
	NewtonMaterialSetDefaultElasticity (nWorld, worldInteriorID, characterID, 0.1f);
	NewtonMaterialSetDefaultFriction (nWorld, worldInteriorID, characterID, 0.6f, 0.6f);
	NewtonMaterialSetCollisionCallback (nWorld, worldInteriorID, characterID, &metal_level, GenericContactBegin, CharacterContactProcess, GenericContactEnd); 

	// set the material properties for vehicle on level
	NewtonMaterialSetDefaultElasticity (nWorld, levelID, vehicleID, 0.1f);
	//NewtonMaterialSetDefaultFriction (nWorld, levelID, vehicleID, 2.0f, 1.8f);
	NewtonMaterialSetCollisionCallback (nWorld, levelID, vehicleID, &vehicle_level, GenericContactBegin, VehicleContactProcess, GenericContactEnd); 
}


// destroy all material resources
void CleanUpMaterials (NewtonWorld* nWorld)
{
	wood_wood.Detroy();
	wood_metal.Detroy();
	wood_level.Detroy();
	metal_metal.Detroy();
	metal_level.Detroy();
}
