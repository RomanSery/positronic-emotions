//manages all objects that use the Newton physics engine
//loads world collision geometry, handls player collision

#include "dxstdafx.h"
#include "Newton/newton.h"
#include "Player.h"
#include "CarManager.h"

class NewtonObjects
{
 public:
   static CDXUTMesh	    *collisionMesh; //so NPCs can access it easily
   static NewtonWorld   *nWorld;
   static CarManager	*carManager;
   static bool          isWorldCollidable;
 private:
   
   NewtonBody        *levelBody;
   NewtonCollision   *collision;

   NewtonBody		 *playerBody;
   
   //CDXUTMesh	     *collisionMesh;
      
   
   Player			 *p;
   D3DXMATRIX	     matWorld;
   IDirect3DDevice9* pd3dDevice;


   void			     readInLevel();
   void				 initNewton();
   void				 addPlayer();

   //callbacks
   static void _cdecl  PhysicsApplyForceAndTorque (const NewtonBody* body);
   static void _cdecl  PhysicsSetTransform (const NewtonBody* body, const float* matrix);
   static void _cdecl  PhysicsBodyDestructor (const NewtonBody* body);
   static void _cdecl  CharacterApplyForceAndTorque(const NewtonBody *body);
   static void _cdecl  LevelCollisionCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body,
								const float* vertex, int vertexstrideInBytes, int indexCount, const int* indexArray); 
  
   //memory allocation
   static void* _cdecl PhysicsAlloc (int sizeInBytes) {	return malloc (sizeInBytes);}
   static void _cdecl  PhysicsFree (void *ptr, int sizeInBytes) { free (ptr); }

   	
public:
	NewtonObjects(Player *player);
		
    void	drawNewtonScene(IDirect3DDevice9* pd3dDevice, float fElapsedTime);
	void	frameMove(IDirect3DDevice9* pd3dDevice);

	void	initDeviceObjects(IDirect3DDevice9* device);
	void	restoreDeviceObjects();
	void	invalidateDeviceObjects();
	void	destroyDeviceObjects();

	
	float m_AccumlativeTimeSlice;
   
};
