#ifndef NEWTON_H__736495_6495_076__INCLUDED_
#define NEWTON_H__736495_6495_076__INCLUDED_


#ifdef _NEWTON_USE_LIB
	#define NEWTON_API
#else
	#ifdef _NEWTON_BUILD_DLL
		#define NEWTON_API __declspec(dllexport)
	#else
		#define NEWTON_API __declspec(dllimport)
	#endif
#endif


extern "C" {
	typedef struct NewtonBody{} NewtonBody;
	typedef struct NewtonWorld{} NewtonWorld;
	typedef struct NewtonJoint{} NewtonJoint;
	typedef struct NewtonContact{} NewtonContact;
	typedef struct NewtonMaterial{} NewtonMaterial;
	typedef struct NewtonCollision{} NewtonCollision;

	typedef struct NewtonRagDoll{} NewtonRagDoll;
	typedef struct NewtonRagDollBone{} NewtonRagDollBone;
	
	typedef struct NewtonUserMeshCollisionCollideDescTag
	{
		float m_boxP0[4];						// lower bounding box of intersection query in local space
		float m_boxP1[4];						// upper bounding box of intersection query in local space
		void* m_userData;                       // user data passed to the collison geometry at creation time
		int	  m_faceCount;                      // the applycation should set here how many polygons inteset the query
		float* m_vertex;                        // the applycation should the pointer to the vertex array. 
		int m_vertexStrideInBytes;              // the applycation should set here the size of each vertex
	 	int* m_userAttribute;                   // the applycation should set here the pointer to the user data, one for each face
		int* m_faceIndexCount;                  // the applycation should set here the pointer to the vertex count of each face.
		int* m_faceVertexIndex;                 // the applycation should set here the pointer index array for each vertex on a face.
		NewtonBody* m_objBody;                  // pointer to the colliding body
		NewtonBody* m_polySoupBody;             // pointer to the rigid body owner of this collision tree 
	} NewtonUserMeshCollisionCollideDesc;

	typedef struct NewtonUserMeshCollisionRayHitDescTag
	{
		float m_p0[4];							// ray origin in collision local space
		float m_p1[4];                          // ray destination in collision local space   
		float m_normalOut[4];					// copy here the normal at the rat intesection
		int m_userIdOut;                        // copy here a user defined id for further feedback  
		void* m_userData;                       // user data passed to the collison geometry at creation time
	} NewtonUserMeshCollisionRayHitDesc;

	typedef struct NewtonHingeSliderUpdateDescTag
	{
		float m_accel;
		float m_minFriction;
		float m_maxFriction;
		float m_timestep;
	} NewtonHingeSliderUpdateDesc;


	// Newton callback functions
	typedef void* (_cdecl *NewtonAllocMemory) (int sizeInBytes);
	typedef void (_cdecl *NewtonFreeMemory) (void *ptr, int sizeInBytes);

	typedef void (_cdecl *NewtonSerialize) (void* serializeHandle, const void* buffer, size_t size);
	typedef void (_cdecl *NewtonDeserialize) (void* serializeHandle, void* buffer, size_t size);
	

	typedef void (_cdecl *NewtonUserMeshCollisionCollideCallback) (NewtonUserMeshCollisionCollideDesc* collideDescData);
	typedef float (_cdecl *NewtonUserMeshCollisionRayHitCallback) (NewtonUserMeshCollisionRayHitDesc* lineDescData);
	typedef void (_cdecl *NewtonUserMeshCollisionDestroyCallback) (void* descData);
	typedef void (_cdecl *NewtonTreeCollisionCallback) (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body,
		const float* vertex, int vertexstrideInBytes, 
		int indexCount, const int* indexArray); 

	typedef void (_cdecl *NewtonBodyDestructor) (const NewtonBody* body);
	typedef void (_cdecl *NewtonApplyForceAndTorque) (const NewtonBody* body);
	typedef void (_cdecl *NewtonBodyActivationState) (const NewtonBody* body, unsigned state);
	typedef void (_cdecl *NewtonSetTransform) (const NewtonBody* body, const float* matrix);
	typedef void (_cdecl *NewtonSetRagDollTransform) (const NewtonRagDollBone* bone);
	typedef void (_cdecl *NewtonGetBuoyancyPlane) (void *context, const float* globalSpaceMatrix, float* globalSpacePlane);

	typedef void (_cdecl *NewtonVehicleTireUpdate) (const NewtonJoint* vehicle);
	

	typedef float (_cdecl *NewtonWorldRayFilterCallback)(const NewtonBody* body, const float* hitNormal, int collisionID, void* userData, float intersetParam);
	typedef void (_cdecl *NewtonBodyLeaveWorld) (const NewtonBody* body);

	typedef int  (_cdecl *NewtonContactBegin) (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	typedef int  (_cdecl *NewtonContactProcess) (const NewtonMaterial* material, const NewtonContact* contact);
	typedef void (_cdecl *NewtonContactEnd) (const NewtonMaterial* material);


	typedef void (_cdecl *NewtonBodyIterator) (const NewtonBody* body);
	typedef void (_cdecl *NewtonCollisionIterator) (const NewtonBody* body, int vertexCount, const float* FaceArray, int faceId);


	typedef void (_cdecl *NewtonBallCallBack) (const NewtonJoint* ball);
	typedef unsigned (_cdecl *NewtonHingeCallBack) (const NewtonJoint* hinge, NewtonHingeSliderUpdateDesc* desc);
	typedef unsigned (_cdecl *NewtonSliderCallBack) (const NewtonJoint* slider, NewtonHingeSliderUpdateDesc* desc);
	typedef unsigned (_cdecl *NewtonUniversalCallBack) (const NewtonJoint* universal, NewtonHingeSliderUpdateDesc* desc);
	typedef unsigned (_cdecl *NewtonCorkscrewCallBack) (const NewtonJoint* corkscrew, NewtonHingeSliderUpdateDesc* desc);
	typedef unsigned (_cdecl *NewtonUserBilateralCallBack) (const NewtonJoint* userJoint);

	typedef void (_cdecl *NewtonConstraintDestructor) (const NewtonJoint* me);

	// **********************************************************************************************
	//
	// world control functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonWorld* NewtonCreate (NewtonAllocMemory malloc, NewtonFreeMemory mfree);
	NEWTON_API void NewtonDestroy (const NewtonWorld* newtonWorld);
	NEWTON_API void NewtonDestroyAllBodies (const NewtonWorld* newtonWorld);

  	NEWTON_API void NewtonUpdate (const NewtonWorld* newtonWorld, float timestep);
	NEWTON_API float NewtonGetTimeStep (const NewtonWorld* newtonWorld);
	NEWTON_API void NewtonSetMinimumFrameRate (const NewtonWorld* newtonWorld, float frameRate);
	NEWTON_API void NewtonSetBodyLeaveWorldEvent (const NewtonWorld* newtonWorld, NewtonBodyLeaveWorld callback); 
	NEWTON_API void NewtonSetWorldSize (const NewtonWorld* newtonWorld, const float* minPoint, const float* maxPoint); 
	
	NEWTON_API void NewtonWorldFreezeBody (const NewtonWorld* newtonWorld, const NewtonBody* body);
	NEWTON_API void NewtonWorldUnfreezeBody (const NewtonWorld* newtonWorld, const NewtonBody* body);

	NEWTON_API void NewtonWorldForEachBodyDo (const NewtonWorld* newtonWorld, NewtonBodyIterator callback);

	NEWTON_API void NewtonWorldSetUserData (const NewtonWorld* newtonWorld, void* userData);
	NEWTON_API void* NewtonWorldGetUserData (const NewtonWorld* newtonWorld);
	NEWTON_API int NewtonWorldGetVersion (const NewtonWorld* newtonWorld);

	NEWTON_API void NewtonWorldRayCast (const NewtonWorld* newtonWorld, const float* p0, const float* p1, NewtonWorldRayFilterCallback filter, void* userData);

	NEWTON_API int NewtonWorldCollide (const NewtonWorld* newtonWorld, int maxSize,
										const NewtonCollision* collsionA, const float* matrixA,
										const NewtonCollision* collsionB, const float* matrixB,
										float* contacts, float* normals, float* penetration);


	// world utility functions
	// NEWTON_API int NewtonGetBodiesCount();
	// NEWTON_API int NewtonGetActiveBodiesCount();
	// NEWTON_API int NewtonGetActiveConstraintsCount();
	// NEWTON_API float NewtonGetGlobalScale (const NewtonWorld* newtonWorld);

	// **********************************************************************************************
	//
	// Physics Material Section
	//
	// **********************************************************************************************
	NEWTON_API int NewtonMaterialGetDefaultGroupID(const NewtonWorld* newtonWorld);
	NEWTON_API int NewtonMaterialCreateGroupID(const NewtonWorld* newtonWorld);
	NEWTON_API void NewtonMaterialDestroyAllGroupID(const NewtonWorld* newtonWorld);

	NEWTON_API void NewtonMaterialSetDefaultSoftness (const NewtonWorld* newtonWorld, int id0, int id1, float value);
	NEWTON_API void NewtonMaterialSetDefaultElasticity (const NewtonWorld* newtonWorld, int id0, int id1, float elasticCoef);
	NEWTON_API void NewtonMaterialSetDefaultCollidable (const NewtonWorld* newtonWorld, int id0, int id1, int state);
	NEWTON_API void NewtonMaterialSetDefaultFriction (const NewtonWorld* newtonWorld, int id0, int id1, 
		float staticFriction, float kineticFriction);

	NEWTON_API void NewtonMaterialSetCollisionCallback (const NewtonWorld* newtonWorld, int id0, int id1, void* userData,
		NewtonContactBegin begin, NewtonContactProcess process, NewtonContactEnd end);

	NEWTON_API void* NewtonMaterialGetUserData (const NewtonWorld* newtonWorld, int id0, int id1);
		


	// **********************************************************************************************
	//
	// Physics Contact control functions
	//
	// **********************************************************************************************
	NEWTON_API void NewtonMaterialDisableContact (const NewtonMaterial* material);
	NEWTON_API float NewtonMaterialGetCurrentTimestep (const NewtonMaterial* material);
	NEWTON_API void *NewtonMaterialGetMaterialPairUserData (const NewtonMaterial* material);
	NEWTON_API unsigned NewtonMaterialGetContactFaceAttribute (const NewtonMaterial* material);
	NEWTON_API unsigned NewtonMaterialGetBodyCollisionID (const NewtonMaterial* material, const NewtonBody* body);
	NEWTON_API float NewtonMaterialGetContactNormalSpeed (const NewtonMaterial* material, const NewtonContact* contactlHandle);
	NEWTON_API void NewtonMaterialGetContactForce (const NewtonMaterial* material, float* force);
	NEWTON_API void NewtonMaterialGetContactPositionAndNormal (const NewtonMaterial* material, float* posit, float* normal);
	NEWTON_API void NewtonMaterialGetContactTangentDirections (const NewtonMaterial* material, float* dir0, float* dir);
	NEWTON_API float NewtonMaterialGetContactTangentSpeed (const NewtonMaterial* material, const NewtonContact* contactlHandle, int index);
	
	

	NEWTON_API void NewtonMaterialSetContactSoftness (const NewtonMaterial* material, float softness);
	NEWTON_API void NewtonMaterialSetContactElasticity (const NewtonMaterial* material, float restitution);
	NEWTON_API void NewtonMaterialSetContactFrictionState (const NewtonMaterial* material, int state, int index);
	NEWTON_API void NewtonMaterialSetContactStaticFrictionCoef (const NewtonMaterial* material, float coef, int index);
	NEWTON_API void NewtonMaterialSetContactKineticFrictionCoef (const NewtonMaterial* material, float coef, int index);
	NEWTON_API void NewtonMaterialSetContactTangentAcceleration (const NewtonMaterial* material, float accel, int index);

	NEWTON_API void NewtonMaterialContactRotateTangentDirections (const NewtonMaterial* material, const float* directionVector);

	// **********************************************************************************************
	//
	// convex collision primitives creation functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateNull (const NewtonWorld* newtonWorld);
	NEWTON_API NewtonCollision* NewtonCreateSphere (const NewtonWorld* newtonWorld, float radiusX, float radiusY, float radiusZ, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateBox (const NewtonWorld* newtonWorld, float dx, float dy, float dz, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateCone (const NewtonWorld* newtonWorld, float radius, float height, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateCapsule (const NewtonWorld* newtonWorld, float radius, float height, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateCylinder (const NewtonWorld* newtonWorld, float radius, float height, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateChamferCylinder (const NewtonWorld* newtonWorld, float radius, float height, const float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateConvexHull (const NewtonWorld* newtonWorld, int count, float* vertexCloud, int strideInBytes, float *offsetMatrix);
	NEWTON_API NewtonCollision* NewtonCreateConvexHullModifier (const NewtonWorld* newtonWorld, const NewtonCollision* convexHullCollision);
	NEWTON_API void NewtonConvexHullModifierGetMatrix (const NewtonCollision* convexHullCollision, float* matrix);
	NEWTON_API void NewtonConvexHullModifierSetMatrix (const NewtonCollision* convexHullCollision, const float* matrix);
	

	NEWTON_API void NewtonConvexCollisionSetUserID (const NewtonCollision* convexCollision, unsigned id);
	NEWTON_API unsigned  NewtonConvexCollisionGetUserID (const NewtonCollision* convexCollision);

	// **********************************************************************************************
	//
	// complex collision primitives creation functions
	// note: can only be used with static bodies (bodies with infinite mass)
	//
	// **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateCompoundCollision (const NewtonWorld* newtonWorld, int count, 
		NewtonCollision* const collisionPrimitiveArray[]);

	NEWTON_API NewtonCollision* NewtonCreateUserMeshCollision (const NewtonWorld* newtonWorld, const float *minBox, 
		const float *maxBox, void *userData, NewtonUserMeshCollisionCollideCallback collideCallback, 
		NewtonUserMeshCollisionRayHitCallback rayHitCallback, NewtonUserMeshCollisionDestroyCallback destroyCallback);

	// **********************************************************************************************
	//
	// CollisionTree Utility functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonCollision* NewtonCreateTreeCollision (const NewtonWorld* newtonWorld, NewtonTreeCollisionCallback userCallback);
	NEWTON_API void NewtonTreeCollisionBeginBuild (const NewtonCollision* treeCollision);
	NEWTON_API void NewtonTreeCollisionAddFace (const NewtonCollision* treeCollision, int vertexCount, const float* vertexPtr, 
		int strideInBytes, int faceAttribute);
	NEWTON_API void NewtonTreeCollisionEndBuild (const NewtonCollision* treeCollision, int optimize);

	NEWTON_API void NewtonTreeCollisionSerialize (const NewtonCollision* treeCollision, NewtonSerialize serializeFunction, 
		void* serializeHandle);

	NEWTON_API NewtonCollision* NewtonCreateTreeCollisionFromSerialization (const NewtonWorld* newtonWorld, 
		NewtonTreeCollisionCallback userCallback, NewtonDeserialize deserializeFunction, void* serializeHandle);

	NEWTON_API int NewtonTreeCollisionGetFaceAtribute (const NewtonCollision* treeCollision, const int* faceIndexArray); 
	NEWTON_API void NewtonTreeCollisionSetFaceAtribute (const NewtonCollision* treeCollision, const int* faceIndexArray, 
		int attribute); 

	// **********************************************************************************************
	//
	// Collision Miscelaneos function
	//
	// **********************************************************************************************
	NEWTON_API void NewtonReleaseCollision (const NewtonWorld* newtonWorld, const NewtonCollision* collision);
	NEWTON_API void NewtonCollisionCalculateAABB (const NewtonCollision* collision, const float *matrix, float* p0, float* p1);
	NEWTON_API float NewtonCollisionRayCast (const NewtonCollision* collision, const float* p0, const float* p1, float* normals, int* attribute);
	

	// **********************************************************************************************
	//
	// transforms utility functions
	//
	// **********************************************************************************************
	NEWTON_API void NewtonGetEulerAngle (const float* matrix, float* eulersAngles);
	NEWTON_API void NewtonSetEulerAngle (const float* eulersAngles, float* matrix);


	// **********************************************************************************************
	//
	// body manipulation functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonBody* NewtonCreateBody (const NewtonWorld* newtonWorld, const NewtonCollision* collision);
	NEWTON_API void  NewtonDestroyBody(const NewtonWorld* newtonWorld, const NewtonBody* body);

	NEWTON_API void  NewtonBodyAddForce (const NewtonBody* body, const float* force);
	NEWTON_API void  NewtonBodyAddTorque (const NewtonBody* body, const float* torque);

	NEWTON_API void  NewtonBodySetMatrix (const NewtonBody* body, const float* matrix);
	NEWTON_API void  NewtonBodySetMatrixRecursive (const NewtonBody* body, const float* matrix);
	NEWTON_API void  NewtonBodySetMassMatrix (const NewtonBody* body, float mass, float Ixx, float Iyy, float Izz);
	NEWTON_API void  NewtonBodySetMaterialGroupID (const NewtonBody* body, int id);
	NEWTON_API void  NewtonBodySetContinuousCollisionMode (const NewtonBody* body, unsigned state);
	NEWTON_API void  NewtonBodySetJointRecursiveCollision (const NewtonBody* body, unsigned state);
	NEWTON_API void  NewtonBodySetOmega (const NewtonBody* body, const float* omega);
	NEWTON_API void  NewtonBodySetVelocity (const NewtonBody* body, const float* velocity);
	NEWTON_API void  NewtonBodySetForce (const NewtonBody* body, const float* force);
	NEWTON_API void  NewtonBodySetTorque (const NewtonBody* body, const float* torque);
	NEWTON_API void  NewtonBodySetLinearDamping (const NewtonBody* body, float linearDamp);
	NEWTON_API void  NewtonBodySetAngularDamping (const NewtonBody* body, const float* angularDamp);
	NEWTON_API void  NewtonBodySetUserData (const NewtonBody* body, void* userData);
	NEWTON_API void  NewtonBodyCoriolisForcesMode (const NewtonBody* body, int mode);
	NEWTON_API void  NewtonBodySetCollision (const NewtonBody* body, const NewtonCollision* collision);
	NEWTON_API void  NewtonBodySetAutoFreeze (const NewtonBody* body, int state);
	NEWTON_API void  NewtonBodySetFreezeTreshold (const NewtonBody* body, float freezeSpeed2, float freezeOmega2, int framesCount);
	
	NEWTON_API void  NewtonBodySetTransformCallback (const NewtonBody* body, NewtonSetTransform callback);
	NEWTON_API void  NewtonBodySetDestructorCallback (const NewtonBody* body, NewtonBodyDestructor callback);
	NEWTON_API void  NewtonBodySetAutoactiveCallback (const NewtonBody* body, NewtonBodyActivationState callback);
	NEWTON_API void  NewtonBodySetForceAndTorqueCallback (const NewtonBody* body, NewtonApplyForceAndTorque callback);

	NEWTON_API NewtonWorld* NewtonBodyGetWorld (const NewtonBody* body);
	NEWTON_API void* NewtonBodyGetUserData (const NewtonBody* body);
	NEWTON_API NewtonCollision* NewtonBodyGetCollision (const NewtonBody* body);
	NEWTON_API int   NewtonBodyGetMaterialGroupID (const NewtonBody* body);
	NEWTON_API int   NewtonBodyGetContinuousCollisionMode (const NewtonBody* body);
	NEWTON_API int   NewtonBodyGetJointRecursiveCollision (const NewtonBody* body);
	NEWTON_API void  NewtonBodyGetMatrix(const NewtonBody* body, float* matrix);
	NEWTON_API void  NewtonBodyGetMassMatrix (const NewtonBody* body, float* mass, float* Ixx, float* Iyy, float* Izz);
	NEWTON_API void  NewtonBodyGetInvMass(const NewtonBody* body, float* invMass, float* invIxx, float* invIyy, float* invIzz);
	NEWTON_API void  NewtonBodyGetOmega(const NewtonBody* body, float* vector);
	NEWTON_API void  NewtonBodyGetVelocity(const NewtonBody* body, float* vector);
	NEWTON_API void  NewtonBodyGetForce(const NewtonBody* body, float* vector);
	NEWTON_API void  NewtonBodyGetTorque(const NewtonBody* body, float* vector);
	
	NEWTON_API int   NewtonBodyGetSleepingState(const NewtonBody* body);
	NEWTON_API int   NewtonBodyGetAutoFreeze(const NewtonBody* body);
	NEWTON_API float NewtonBodyGetLinearDamping (const NewtonBody* body);
	NEWTON_API void  NewtonBodyGetAngularDamping (const NewtonBody* body, float* vector);
	NEWTON_API void  NewtonBodyGetAABB (const NewtonBody* body, float* p0, float* p1);	
	NEWTON_API void  NewtonBodyGetFreezeTreshold (const NewtonBody* body, float* freezeSpeed2, float* freezeOmega2);

	NEWTON_API float NewtonBodyGetTotalVolume(const NewtonBody* body);
	NEWTON_API void  NewtonBodyAddBuoyancyForce (const NewtonBody* body, float fluidDensity, 
		float fluidLinearViscosity, float fluidAngularViscosity, 
		const float* gravityVector, NewtonGetBuoyancyPlane buoyancyPlane, void *context);

	NEWTON_API void NewtonBodyForEachPolygonDo (const NewtonBody* body, NewtonCollisionIterator callback);
	NEWTON_API void NewtonAddBodyImpulse (const NewtonBody* body, const float* pointDeltaVeloc, const float* pointPosit);
	

	// **********************************************************************************************
	//
	// Common joint funtions
	//
	// **********************************************************************************************
	NEWTON_API void* NewtonJointGetUserData (const NewtonJoint* joint);
	NEWTON_API void NewtonJointSetUserData (const NewtonJoint* joint, void* userData);

	NEWTON_API int NewtonJointGetCollisionState (const NewtonJoint* joint);
	NEWTON_API void NewtonJointSetCollisionState (const NewtonJoint* joint, int state);

	NEWTON_API float NewtonJointGetStiffness (const NewtonJoint* joint);
	NEWTON_API void NewtonJointSetStiffness (const NewtonJoint* joint, float state);

	NEWTON_API void NewtonDestroyJoint(const NewtonWorld* newtonWorld, const NewtonJoint* joint);
	NEWTON_API void NewtonJointSetDestructor (const NewtonJoint* joint, NewtonConstraintDestructor destructor);


	// **********************************************************************************************
	//
	// Ball and Socket joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateBall (const NewtonWorld* newtonWorld, const float* pivotPoint, 
		const NewtonBody* childBody, const NewtonBody* parentBody);
	NEWTON_API void NewtonBallSetUserCallback (const NewtonJoint* ball, NewtonBallCallBack callback);
	NEWTON_API void NewtonBallGetJointAngle (const NewtonJoint* ball, float* angle);
	NEWTON_API void NewtonBallGetJointOmega (const NewtonJoint* ball, float* omega);
	NEWTON_API void NewtonBallGetJointForce (const NewtonJoint* ball, float* force);
	NEWTON_API void NewtonBallSetConeLimits (const NewtonJoint* ball, const float* pin, float maxConeAngle, float maxTwistAngle);

	// **********************************************************************************************
	//
	// Hinge joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateHinge (const NewtonWorld* newtonWorld, 
		const float* pivotPoint, const float* pinDir, 
		const NewtonBody* childBody, const NewtonBody* parentBody);

	NEWTON_API void NewtonHingeSetUserCallback (const NewtonJoint* hinge, NewtonHingeCallBack callback);
	NEWTON_API float NewtonHingeGetJointAngle (const NewtonJoint* hinge);
	NEWTON_API float NewtonHingeGetJointOmega (const NewtonJoint* hinge);
	NEWTON_API void NewtonHingeGetJointForce (const NewtonJoint* hinge, float* force);
	NEWTON_API float NewtonHingeCalculateStopAlpha (const NewtonJoint* hinge, const NewtonHingeSliderUpdateDesc* desc, float angle);

	// **********************************************************************************************
	//
	// Slider joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateSlider (const NewtonWorld* newtonWorld, 
		const float* pivotPoint, const float* pinDir, 
		const NewtonBody* childBody, const NewtonBody* parentBody);
	NEWTON_API void NewtonSliderSetUserCallback (const NewtonJoint* slider, NewtonSliderCallBack callback);
	NEWTON_API float NewtonSliderGetJointPosit (const NewtonJoint* slider);
	NEWTON_API float NewtonSliderGetJointVeloc (const NewtonJoint* slider);
	NEWTON_API void NewtonSliderGetJointForce (const NewtonJoint* slider, float* force);
	NEWTON_API float NewtonSliderCalculateStopAccel (const NewtonJoint* slider, const NewtonHingeSliderUpdateDesc* desc, float position);


	// **********************************************************************************************
	//
	// Corkscrew joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateCorkscrew (const NewtonWorld* newtonWorld, 
		const float* pivotPoint, const float* pinDir, 
		const NewtonBody* childBody, const NewtonBody* parentBody);
	NEWTON_API void NewtonCorkscrewSetUserCallback (const NewtonJoint* corkscrew, NewtonCorkscrewCallBack callback);
	NEWTON_API float NewtonCorkscrewGetJointPosit (const NewtonJoint* corkscrew);
	NEWTON_API float NewtonCorkscrewGetJointAngle (const NewtonJoint* corkscrew);
	NEWTON_API float NewtonCorkscrewGetJointVeloc (const NewtonJoint* corkscrew);
	NEWTON_API float NewtonCorkscrewGetJointOmega (const NewtonJoint* corkscrew);
	NEWTON_API void NewtonCorkscrewGetJointForce (const NewtonJoint* corkscrew, float* force);
	NEWTON_API float NewtonCorkscrewCalculateStopAlpha (const NewtonJoint* corkscrew, const NewtonHingeSliderUpdateDesc* desc, float angle);
	NEWTON_API float NewtonCorkscrewCalculateStopAccel (const NewtonJoint* corkscrew, const NewtonHingeSliderUpdateDesc* desc, float position);


	// **********************************************************************************************
	//
	// Universal joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUniversal (const NewtonWorld* newtonWorld, 
		const float* pivotPoint, const float* pinDir0, const float* pinDir1, 
		const NewtonBody* childBody, const NewtonBody* parentBody);
	NEWTON_API void NewtonUniversalSetUserCallback (const NewtonJoint* universal, NewtonUniversalCallBack callback);
	NEWTON_API float NewtonUniversalGetJointAngle0 (const NewtonJoint* universal);
	NEWTON_API float NewtonUniversalGetJointAngle1 (const NewtonJoint* universal);
	NEWTON_API float NewtonUniversalGetJointOmega0 (const NewtonJoint* universal);
	NEWTON_API float NewtonUniversalGetJointOmega1 (const NewtonJoint* universal);
	NEWTON_API void NewtonUniversalGetJointForce (const NewtonJoint* universal, float* force);
	NEWTON_API float NewtonUniversalCalculateStopAlpha0 (const NewtonJoint* universal, const NewtonHingeSliderUpdateDesc* desc, float angle);
	NEWTON_API float NewtonUniversalCalculateStopAlpha1 (const NewtonJoint* universal, const NewtonHingeSliderUpdateDesc* desc, float angle);


	// **********************************************************************************************
	//
	// SlidingContact joint functions
	//
	// **********************************************************************************************
/*
	NEWTON_API NewtonJoint* NewtonConstraintCreateSlidingContact (const NewtonWorld* newtonWorld, 
		const float* pivotPoint, const float* pinDir0, const float* pinDir1, 
		const NewtonBody* childBody, const NewtonBody* parentBody);
	NEWTON_API void NewtonSlidingContactSetUserCallback (const NewtonJoint* contact, NewtonSlidingContactCallBack callback);
*/


	// **********************************************************************************************
	//
	// Up vector joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUpVector (const NewtonWorld* newtonWorld, const float* pinDir, const NewtonBody* body); 
	NEWTON_API void NewtonUpVectorGetPin (const NewtonJoint* upVector, float *pin);
	NEWTON_API void NewtonUpVectorSetPin (const NewtonJoint* upVector, const float *pin);


	// **********************************************************************************************
	//
	// User defined bilateral Joint
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateUserJoint (const NewtonWorld* newtonWorld, const NewtonBody* childBody, const NewtonBody* parentBody); 
	NEWTON_API void NewtonUserJointSetUserCallback (const NewtonJoint* userJoint, NewtonUserBilateralCallBack callback);



	// **********************************************************************************************
	//
	// Ragdoll joint contatiner funtion
	//
	// **********************************************************************************************
	NEWTON_API NewtonRagDoll* NewtonCreateRagDoll (const NewtonWorld* newtonWorld);
	NEWTON_API void NewtonDestroyRagDoll (const NewtonWorld* newtonWorld, const NewtonRagDoll* ragDoll);

	NEWTON_API void NewtonRagDollBegin (const NewtonRagDoll* ragDoll);
	NEWTON_API void NewtonRagDollEnd (const NewtonRagDoll* ragDoll);


//	NEWTON_API void NewtonRagDollSetFriction (const NewtonRagDoll* ragDoll, float friction);

	NEWTON_API NewtonRagDollBone* NewtonRagDollFindBone (const NewtonRagDoll* ragDoll, int id);
	NEWTON_API NewtonRagDollBone* NewtonRagDollGetRootBone (const NewtonRagDoll* ragDoll);

	NEWTON_API void NewtonRagDollSetForceAndTorqueCallback (const NewtonRagDoll* ragDoll, NewtonApplyForceAndTorque callback);
	NEWTON_API void NewtonRagDollSetTransformCallback (const NewtonRagDoll* ragDoll, NewtonSetRagDollTransform callback);
	NEWTON_API NewtonRagDollBone* NewtonRagDollAddBone (const NewtonRagDoll* ragDoll, const NewtonRagDollBone* parent, 
		                                                void *userData, float mass, const float* matrix, 
														const NewtonCollision* boneCollision, const float* size);

	NEWTON_API void* NewtonRagDollBoneGetUserData (const NewtonRagDollBone* bone);
	NEWTON_API NewtonBody* NewtonRagDollBoneGetBody (const NewtonRagDollBone* bone);
	NEWTON_API void NewtonRagDollBoneSetID (const NewtonRagDollBone* bone, int id);


	NEWTON_API void NewtonRagDollBoneSetLimits (const NewtonRagDollBone* bone, 
		                                        const float* coneDir, float minConeAngle, float maxConeAngle, float maxTwistAngle,
												const float* bilateralConeDir, float negativeBilateralConeAngle, float positiveBilateralConeAngle);
	
//	NEWTON_API NewtonRagDollBone* NewtonRagDollBoneGetChild (const NewtonRagDollBone* bone);
//	NEWTON_API NewtonRagDollBone* NewtonRagDollBoneGetSibling (const NewtonRagDollBone* bone);
//	NEWTON_API NewtonRagDollBone* NewtonRagDollBoneGetParent (const NewtonRagDollBone* bone);
//	NEWTON_API void NewtonRagDollBoneSetLocalMatrix (const NewtonRagDollBone* bone, float* matrix);
//	NEWTON_API void NewtonRagDollBoneSetGlobalMatrix (const NewtonRagDollBone* bone, float* matrix);

	NEWTON_API void NewtonRagDollBoneGetLocalMatrix (const NewtonRagDollBone* bone, float* matrix);
	NEWTON_API void NewtonRagDollBoneGetGlobalMatrix (const NewtonRagDollBone* bone, float* matrix);



	// **********************************************************************************************
	//
	// Vehicle joint functions
	//
	// **********************************************************************************************
	NEWTON_API NewtonJoint* NewtonConstraintCreateVehicle (const NewtonWorld* newtonWorld, const float* upDir, const NewtonBody* body); 
	NEWTON_API void NewtonVehicleReset (const NewtonJoint* vehicle); 
	NEWTON_API void NewtonVehicleSetTireCallback (const NewtonJoint* vehicle, NewtonVehicleTireUpdate update);
	NEWTON_API int NewtonVehicleAddTire (const NewtonJoint* vehicle, const float* localMatrix, const float* pin, float mass, float width, float radius, 
		       float suspesionShock, float suspesionSpring, float suspesionLength, void* userData, int collisionID);
	NEWTON_API void NewtonVehicleRemoveTire (const NewtonJoint* vehicle, int tireIndex);
	NEWTON_API void NewtonVehicleBalanceTires (const NewtonJoint* vehicle, float gravityMag);

	NEWTON_API int NewtonVehicleGetFirstTireID (const NewtonJoint* vehicle);
	NEWTON_API int NewtonVehicleGetNextTireID (const NewtonJoint* vehicle, int tireId);

	NEWTON_API int NewtonVehicleTireIsAirBorne (const NewtonJoint* vehicle, int tireId);
	NEWTON_API int NewtonVehicleTireLostSideGrip (const NewtonJoint* vehicle, int tireId);
	NEWTON_API int NewtonVehicleTireLostTraction (const NewtonJoint* vehicle, int tireId);

	NEWTON_API void* NewtonVehicleGetTireUserData (const NewtonJoint* vehicle, int tireId);
	NEWTON_API float NewtonVehicleGetTireOmega (const NewtonJoint* vehicle, int tireId);
	NEWTON_API float NewtonVehicleGetTireNormalLoad (const NewtonJoint* vehicle, int tireId);
	NEWTON_API float NewtonVehicleGetTireSteerAngle (const NewtonJoint* vehicle, int tireId);
	NEWTON_API float NewtonVehicleGetTireLateralSpeed (const NewtonJoint* vehicle, int tireId);
	NEWTON_API float NewtonVehicleGetTireLongitudinalSpeed (const NewtonJoint* vehicle, int tireId);
	NEWTON_API void NewtonVehicleGetTireMatrix (const NewtonJoint* vehicle, int tireId, float* matrix);


	NEWTON_API void NewtonVehicleSetTireTorque (const NewtonJoint* vehicle, int tireId, float torque);
	NEWTON_API void NewtonVehicleSetTireSteerAngle (const NewtonJoint* vehicle, int tireId, float angle);
	
	NEWTON_API void NewtonVehicleSetTireMaxSideSleepSpeed (const NewtonJoint* vehicle, int tireId, float speed);
	NEWTON_API void NewtonVehicleSetTireSideSleepCoeficient (const NewtonJoint* vehicle, int tireId, float coeficient);
	NEWTON_API void NewtonVehicleSetTireMaxLongitudinalSlideSpeed (const NewtonJoint* vehicle, int tireId, float speed);
	NEWTON_API void NewtonVehicleSetTireLongitudinalSlideCoeficient (const NewtonJoint* vehicle, int tireId, float coeficient);

	NEWTON_API float NewtonVehicleTireCalculateMaxBrakeAcceleration (const NewtonJoint* vehicle, int tireId);
	NEWTON_API void NewtonVehicleTireSetBrakeAcceleration (const NewtonJoint* vehicle, int tireId, float accelaration, float torqueLimit);
	
};


#endif


