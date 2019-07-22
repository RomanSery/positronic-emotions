#include "dxstdafx.h"
#include ".\IdleNpc.h"

#define WALK_SPEED 0.15f
#define IDLE_SPEED 1.0f

IdleNpc::IdleNpc(EntityType t, bool fp, float x, float y, float z, int i, float rot)
 : Npc(t, i)
{
  state = IDLE;
  type = t;
  facePlayer = fp;
  isAnimStarted = false;
  rotation = rot;
  pos = D3DXVECTOR3(x, y, z);
 
}


void IdleNpc::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *device, D3DXVECTOR3 playerPos)
{
  if(isAnimStarted == false){
    //entity->SetAnimationSet(0);
    setAnimationStatus(entity, 0);
    entity->setAnimationSpeed(1.0f);
    isAnimStarted = true;
  }
   
  D3DXVECTOR3 v = playerPos.operator -(pos);
  distance = max(abs(v.x), abs(v.z));
	
  //set transform matrix
  D3DXMATRIX matTrans, matRot, matWorld;
  D3DXMatrixIdentity(&matWorld);D3DXMatrixIdentity(&matRot);D3DXMatrixIdentity(&matWorld);
  D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

  if(facePlayer){
    calcAngle(playerPos);
    D3DXMatrixRotationY(&matRot, angle);
  }
  else
    D3DXMatrixRotationY(&matRot, rotation);
    
  D3DXMatrixMultiply(&matWorld, &matWorld, &matRot);
  D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
  entity->FrameMove(m_fElapsedTime,matWorld);

}



void IdleNpc::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  entity->Render(device, NPC_CODE, NULL);
 
  /*ID3DXMesh *box;
  D3DXCreateBox(device, boundingBox->width, boundingBox->height, boundingBox->length, &box, NULL);
  
  //render bounding box
  D3DXMATRIX mTrans;D3DXMatrixIdentity(&mTrans);
  D3DXMatrixTranslation(&mTrans, pos.x, pos.y + (boundingBox->height / 2), pos.z);
  device->SetTransform(D3DTS_WORLD, &mTrans);
  box->DrawSubset(0);
  box->Release(); */
}


void IdleNpc::calcAngle(D3DXVECTOR3 playerPos)
{
  D3DXVECTOR2 v1(pos.x, pos.z);
  D3DXVECTOR2 v2(playerPos.x, playerPos.z);
  
  float deltaY, deltaX;
  if(v2.y == v1.y)
   deltaY = (v2.y - v1.y);
  else
   deltaY = (v1.y - v2.y);

  if(v2.x == v2.x)
   deltaX = (v2.x - v1.x);
  else
   deltaX = (v1.x - v2.x);

  angle  = atan2(deltaY ,deltaX);

}