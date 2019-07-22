//base class for more specialized NPCs.
//This base class is used only by simple patroling NPCs

#ifndef NPC_CLASS
#define NPC_CLASS

#include "MeshHeirarchy.h"
#include "NPCStructs.h"
#include "SoundManager.h"
#include "Frustum.h"

class Npc {
 private:
   //NPC status variables
   int			m_currentAnimation;
   D3DXVECTOR3	pos;				//current position of NPC
   D3DXVECTOR3  destination;		//current destination of NPC
   D3DXVECTOR3  facing;				//direction NPC is facing
   Status		state;				//current state of NPC
   float        angle;				//angle of rotation
   BOOL         seePlayer;			//does the NPC see player
   BOOL         isIntersectingMesh; //is NPC facing collision geometry
   BOOL         isBlocked;			//is blocked by player
   bool			repressBlockingSound;//whether or not to play a sound when blocked by player

   //navigation variables
   sPatrolNode  nodes[10];			//collection of patrol nodes
   int		    numNodes;			//number of nodes
   int			currNode;			//index of node NPC is traveling to
   float	    distance;			//distance from player
   float        distanceFromMesh;   //distance from city geometry if intersecting
   float	    length;			    //distance to next patrol point
   float	    XLength, ZLength, ratio, ZValue;  //movement delta values
   bool		    loopPatrolNodes;	//wether or not to stop at the last patrol node	   
   float		followDistance;		//if following player, distance to follow at
   
   //bounding box
   sBoundingBox *boundingBox;


   //for reading in dialogue information, used only by missionNPCs
   FILE    *inFile;
   char*   text[10];
   void    readInNpcText();
   
   
   //entity information
   EntityType   type;		 //character model ID	
   int			index;		 //index of entity

   
   //animation functions
   void	 setAnimationStatus(CXfileEntity *entity, int n);
   int	 getAnimationStatus(){return m_currentAnimation;}
   void  updateAnimationStatus(CXfileEntity *entity);
   
   //navigation functions
   void  calcAngle(D3DXVECTOR3 playerPos);
   void  addNode(float x, float y, float z);
   void  finishAddingNodes();
   void  doFollow(D3DXVECTOR3 playerPos);
   void  calcXZValues();
   void  setLoopNodes(bool b) { loopPatrolNodes = b; }
   
 public:
	Npc(EntityType t, int i = 0, Status s = WALKING);

	BOOL          canSeePlayer();	
	EntityType    getEntityType() { return type; }
    int			  getIndex()      { return index;}
	bool		  reachedEnd()    { return (currNode == numNodes - 1); }
    void		  setRepressBlockingSound(bool b) { repressBlockingSound = b; }
    void		  setFollowDistance(float d) { followDistance = d; }

    //overridable functions
    virtual void  FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *m_pd3dDevice, D3DXVECTOR3 playerPos);
	virtual void  render(IDirect3DDevice9 *device, CXfileEntity *entity);
    virtual void  talk();
    virtual BOOL  isInVeiwingFrustum(cFrustum* frustum);
    virtual float getDistanceFromPlayer() { return distance; }   
    
    virtual void initDeviceObjects(IDirect3DDevice9* device);
    virtual void invalidateDeviceObjects();

	
    friend class NPCManager;
    friend class IdleNpc;
    friend class SuicidalNpc;
    friend class MissionNPC;
    friend class JaneMissionNPC;
    friend class StoreClerkMissionNPC;
    friend class InspectorMissionNPC;
    friend class WitnessMissionNPC;
};

#endif
