#include "dxstdafx.h"
#include "playerFuncs.h"
#include "Player.h"


void renderPlayerRobotForm(ID3DXEffect *effect, D3DXMESHCONTAINER_DERIVED *pMeshContainer,
						   D3DXFRAME_DERIVED *pFrame, IDirect3DCubeTexture9 *cubeTexture,
						   D3DXHANDLE technique)
{
  //effect matrices
  D3DXMATRIX proj, viewProjection;
  D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );
  D3DXMatrixMultiply(&viewProjection, &Player::sPlayerInfo->matView, &proj);

  UINT passes = 1, iMaterial; 
  effect->SetTechnique(technique);
  effect->Begin(&passes, 0);
  effect->BeginPass(0);
  effect->SetMatrix("World", &pFrame->CombinedTransformationMatrix);
  effect->SetMatrix("ViewProjection", &viewProjection);
  effect->SetVector("CameraPos", &D3DXVECTOR4(Player::sPlayerInfo->eyePos, 1));
  effect->SetVector("lightPos", &D3DXVECTOR4(Player::sPlayerInfo->lightPos, 1));
  effect->SetTexture("EnvironmentMap", cubeTexture);
  effect->SetFloat("Time",(float)DXUTGetTime());
  effect->CommitChanges();
  for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
    effect->SetTexture("baseTex", pMeshContainer->ppTextures[iMaterial]);
    effect->CommitChanges();
    pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
  }  
  effect->EndPass();
  effect->End();  

}

void renderPlayerHumanForm(ID3DXEffect *effect, D3DXMESHCONTAINER_DERIVED *pMeshContainer,
							D3DXFRAME_DERIVED *pFrame)
{
  //effect matrices
  D3DXMATRIX proj, viewProjection;
  D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60.0f), GET_ASPECTRATIO(), 0.1f, 100000.0f );
  D3DXMatrixMultiply(&viewProjection, &Player::sPlayerInfo->matView, &proj);

  UINT passes = 1, iMaterial; 
  effect->SetTechnique("renderHuman");
  effect->Begin(&passes, 0);
  effect->BeginPass(0);
  effect->SetMatrix("World", &pFrame->CombinedTransformationMatrix);
  effect->SetMatrix("ViewProjection", &viewProjection);
  effect->SetVector("lightPos", &D3DXVECTOR4(Player::sPlayerInfo->lightPos, 1));
  
  for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
    effect->SetTexture("baseTex", pMeshContainer->ppTextures[iMaterial]);
    effect->CommitChanges();
    pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
  }
  effect->EndPass();
  effect->End();  

}