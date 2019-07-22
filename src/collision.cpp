#include "dxstdafx.h"
#include ".\collision.h"
#include <d3dx9mesh.h>
#include "vector.h"


float GetHeightBelow( LPD3DXBASEMESH m_Mesh, float XPos, float YPos, float ZPos)
{
  BOOL  Hit;
  float u, v, Dist;
  DWORD FaceIndex;

  D3DXIntersect(m_Mesh,
                &D3DXVECTOR3(XPos,YPos,ZPos),
                &D3DXVECTOR3(0.0f, -1.0f, 0.0f),
                &Hit, &FaceIndex, &u, &v, &Dist, NULL, NULL);
  if(Hit == TRUE)
    return YPos-Dist;
  return YPos;
}

float GetHeightAbove( LPD3DXBASEMESH m_Mesh, float XPos, float YPos, float ZPos)
{ 
  BOOL  Hit;
  float u, v, Dist;
  DWORD FaceIndex;

  D3DXIntersect(m_Mesh,
                &D3DXVECTOR3(XPos,YPos,ZPos),
                &D3DXVECTOR3(0.0f, 1.0f, 0.0f),
                &Hit, &FaceIndex, &u, &v, &Dist, NULL, NULL);
  if(Hit == TRUE)
    return YPos+Dist;
  return YPos;
}

BOOL CheckIntersect(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, LPD3DXBASEMESH m_Mesh,  float *Length)
{
  BOOL  Hit;
  float u, v, Dist;
  //float XDiff, YDiff, ZDiff, Size;
  DWORD FaceIndex;
  //D3DXVec3Normalize(&vecDir, &D3DXVECTOR3(XDiff, YDiff, ZDiff));
  
  D3DXIntersect(m_Mesh, &pos, &vecDir, &Hit, &FaceIndex, &u, &v, &Dist, NULL, NULL);
  
  //Size = (float)sqrt(XDiff*XDiff+YDiff*YDiff+ZDiff*ZDiff);
  if(Hit == TRUE) {
    
    if(Dist > 80.0f)
      Hit = FALSE;
    else {
      if(Length != NULL)
        *Length = Dist;
    }
  }

  return Hit;
}

// **NOTE** assumes "vec1" and "vec2" are normalized, thus the division by
//			the magnitude of the vectors is unneeded
float AngleBetweenVecs(const CVector &vec1, const CVector &vec2)
{
	//assert(TEqual(vec1.magnitude(), 1.0f));
	//assert(TEqual(vec2.magnitude(), 1.0f));
		
	float angle = acos(vec1 * vec2);
		return D3DXToDegree(angle);
}
