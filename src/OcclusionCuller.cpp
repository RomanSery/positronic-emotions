#include "dxstdafx.h"
#include ".\occlusionculler.h"
#include "Player.h"
#include "PrintText.h"

CDXUTMesh* OcclusionCuller::occluders;
sOccluder  OcclusionCuller::occInfo[5];
int		   OcclusionCuller::numOccluders = 2;

OcclusionCuller::OcclusionCuller(void)
{
  occluders = new CDXUTMesh();

  occInfo[0].centerPt = D3DXVECTOR3(187, 64, -69);
  occInfo[0].size = D3DXVECTOR3(285, 128, 231);
  occInfo[1].centerPt = D3DXVECTOR3(-327, 58, -70);
  occInfo[1].size = D3DXVECTOR3(188, 116, 236);
  //occInfo[2].centerPt = D3DXVECTOR3(-112, 97, 523);
  //occInfo[2].size = D3DXVECTOR3(641, 194, 41);

  
  
}

sOccluder OcclusionCuller::getClosestOccluderInfo(D3DXVECTOR3 eyePt)
{
   int index = 0;
   D3DXVECTOR3 v = occInfo[0].centerPt.operator -(eyePt);  v.y = 0;
   float closestDist = D3DXVec3Length(&v);

   for(int i = 1; i < numOccluders; i++){
     v = occInfo[i].centerPt.operator -(eyePt);  v.y = 0;
     float dist = D3DXVec3Length(&v);
     if(dist < closestDist){
       closestDist = dist;
       index = i;
     }
   }

   return occInfo[index];
}

bool OcclusionCuller::isEyePtInside(D3DXVECTOR3 eyePt)
{
   sOccluder occ = getClosestOccluderInfo(eyePt);
   D3DXVECTOR3 diff = occ.centerPt.operator -(eyePt);
   diff.x = abs(diff.x); diff.z = abs(diff.z);
      
   if(diff.x > (occ.size.x/2) || diff.z > (occ.size.z/2))
     return false;
   else
     return true;   
}

//returns true if object is occluded by at least one occluder npc
bool OcclusionCuller::isObjectOccluded(D3DXVECTOR3 center, sBoundingBox *bb)
{
  BOOL isHit = FALSE;
  DWORD faceIndex; 
  D3DXVECTOR3 targetPt, vDir, vDirNorm, playerPos;
  float u, v, distance;
  
  if(isEyePtInside(Player::sPlayerInfo->eyePos)){
    playerPos = Player::sPlayerInfo->vecPos;
    //PrintText::printString("inside", 10, 70);
  }
  else{
    playerPos = Player::sPlayerInfo->eyePos;
    playerPos.y = 20;
    //PrintText::printString("outside", 10, 70);
  }
  
  
       
  targetPt = D3DXVECTOR3(center.x + (bb->width/2), center.y + (bb->height), center.z + (bb->length/2));
  //targetPt = D3DXVECTOR3(center.x, center.y, center.z);
  vDir = targetPt.operator -(playerPos);
  D3DXVec3Normalize(&vDirNorm, &vDir);
        
  D3DXIntersect(occluders->GetLocalMesh(), &playerPos, &vDirNorm, &isHit,
				&faceIndex, &u, &v, &distance, NULL, NULL);   
   
  if(isHit == FALSE)
    return false;

  targetPt = D3DXVECTOR3(center.x - (bb->width/2), center.y - (bb->height - 2), center.z - (bb->length/2));  
  vDir = targetPt.operator -(playerPos);
  D3DXVec3Normalize(&vDirNorm, &vDir);
        
  D3DXIntersect(occluders->GetLocalMesh(), &playerPos, &vDirNorm, &isHit,
				&faceIndex, &u, &v, &distance, NULL, NULL);   
  
    
  if(isHit == FALSE)
    return false;
  else{
    float distFromPlayer = D3DXVec3Length(&vDir);
    if(distFromPlayer > distance)
      return true;
    else
      return false;
  } 
}

bool OcclusionCuller::isObjectOccludedEx(D3DXVECTOR3 center, sBoundingBox *bb)
{
  BOOL isHit = FALSE;
  DWORD faceIndex; 
  D3DXVECTOR3 targetPt, vDir, vDirNorm, playerPos;
  float u, v, distance;
  
  if(isEyePtInside(Player::sPlayerInfo->eyePos)){
    playerPos = Player::sPlayerInfo->vecPos;    
  }
  else{
    playerPos = Player::sPlayerInfo->eyePos;    
  }
       
  targetPt = D3DXVECTOR3(center.x + (bb->width/2), center.y + (bb->height/2), center.z + (bb->length/2));
  vDir = targetPt.operator -(playerPos);
  D3DXVec3Normalize(&vDirNorm, &vDir);
        
  D3DXIntersect(occluders->GetLocalMesh(), &playerPos, &vDirNorm, &isHit,
				&faceIndex, &u, &v, &distance, NULL, NULL);   
  
  if(isHit == FALSE)
    return false;

  targetPt = D3DXVECTOR3(center.x - (bb->width/2), center.y - (bb->height/2), center.z - (bb->length/2));  
  vDir = targetPt.operator -(playerPos);
  D3DXVec3Normalize(&vDirNorm, &vDir);
        
  D3DXIntersect(occluders->GetLocalMesh(), &playerPos, &vDirNorm, &isHit,
				&faceIndex, &u, &v, &distance, NULL, NULL);   
  
  
  if(isHit == FALSE)
    return false;
  else{
    float distFromPlayer = D3DXVec3Length(&vDir);
    if(distFromPlayer > distance)
      return true;
    else
      return false;
  }
}


/*bool OcclusionCuller::isObjectOccluded(D3DXVECTOR3 objectPos, sBoundingBox *sbox)
{
  BOOL isHit = FALSE;
  DWORD faceIndex; D3DXVECTOR3 playerPos;
  float u, v, distance;
  
  if(isEyePtInside(Player::sPlayerInfo->eyePos))
    playerPos = Player::sPlayerInfo->vecPos;
  else
    playerPos = Player::sPlayerInfo->eyePos;
  
  D3DXVECTOR3 vDir = objectPos.operator -(playerPos);
  D3DXVECTOR3 vDirNorm;
  D3DXVec3Normalize(&vDirNorm, &vDir);
    
    
  D3DXIntersect(occluders->GetLocalMesh(), &playerPos, &vDirNorm, &isHit,
				&faceIndex, &u, &v, &distance, NULL, NULL); 

    

  if(isHit == FALSE)
    return false;
  else{
    float distFromPlayer = D3DXVec3Length(&vDir);  
	if(distFromPlayer > distance)
	  return true;
	else
	  return false;    
  }
}*/