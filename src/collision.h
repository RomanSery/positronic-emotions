//collision and height detection functions
#include "vector.h"

// Get height below and above point
float GetHeightBelow( LPD3DXBASEMESH m_Mesh, float XPos, float YPos, float ZPos);
float GetHeightAbove( LPD3DXBASEMESH m_Mesh, float XPos, float YPos, float ZPos);

// Check if a polygon blocks path from start to end
BOOL CheckIntersect(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, LPD3DXBASEMESH m_Mesh, float *Length);

// Returns the angle, in degrees, between "vec1" and "vec2"
float AngleBetweenVecs(const CVector &vec1, const CVector &vec2); 