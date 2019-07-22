//provides occlusion culling for objects that are defining with bounding boxes
//use an occlusion mesh for information about where the occluders are

#include "NPCStructs.h"

struct sOccluder {
  D3DXVECTOR3 centerPt;
  D3DXVECTOR3 size; 
};


class OcclusionCuller{
  private:
    static CDXUTMesh *occluders;
    static sOccluder occInfo[5];
    static int numOccluders;
		
    static sOccluder getClosestOccluderInfo(D3DXVECTOR3 eyePt);
    static bool		 isEyePtInside(D3DXVECTOR3 eyePt);
  public:
	OcclusionCuller(void);
	
	void initDeviceObjects(IDirect3DDevice9* device) {occluders->Create(device, L"meshes/occluders.x");}
	void restoreDeviceObjects(IDirect3DDevice9* device) {occluders->RestoreDeviceObjects(device);}
	void invalidateDeviceObjects() { occluders->InvalidateDeviceObjects(); }
	void destroyDeviceObjects() { occluders->Destroy(); }

	static bool isObjectOccluded(D3DXVECTOR3 objectPos, sBoundingBox *box);
    static bool isObjectOccludedEx(D3DXVECTOR3 objectPos, sBoundingBox *box);

	
};
