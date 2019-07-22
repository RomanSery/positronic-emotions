//handles building the players viewing frustum every frame
//defines methods for testing if objects are inside this frustum
//used for frustum culling of objects


#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

class cFrustum
{
  public:
    D3DXPLANE m_Planes[6];

  public:
    BOOL Construct(LPDIRECT3DDEVICE9 device, float ZDistance, D3DXMATRIX matView );
    BOOL CheckPoint(float XPos, float YPos, float ZPos);
    BOOL CheckCube(float XCenter, float YCenter, float ZCenter, float Size);
    BOOL CheckRectangle(float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize);
	BOOL CheckRectangleWhole(float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize);
    BOOL CheckSphere(float XCenter, float YCenter, float ZCenter, float Radius);
};

#endif
