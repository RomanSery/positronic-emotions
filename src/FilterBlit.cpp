
#define STRICT
#include "dxstdafx.h"
#include <d3dx9math.h>
#include <vector>
#include <assert.h>

#include "FilterBlit.h"

HINSTANCE g_hInstance = NULL;


FilterBlit::FilterBlit()
{
    m_pEffect = NULL;
	isOn = true;
	technique = "Sharpen";
	intensity = 6;
	renderToTex = new RenderToTexture();
	   
}


HRESULT FilterBlit::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
    int i;
    D3DXCreateEffectFromFile(pd3dDevice, L"FilterBlit.fx", NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, NULL);

    // create vertex buffer 
    pd3dDevice->CreateVertexBuffer( 4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0);
    
    QuadVertex      *pBuff;
    if (m_pVertexBuffer){
        m_pVertexBuffer->Lock(0, 4 * sizeof(QuadVertex),(void**)&pBuff, 0);
        for (i = 0; i < 4; ++i){
            pBuff->Position = D3DXVECTOR3((i==0 || i==3) ? -1.0f : 1.0f,
                                          (i<2)          ? -1.0f : 1.0f,
                                          0.0f);
            pBuff->Tex = D3DXVECTOR2(((i==0 || i==3) ? 0.0f : 1.0f), 
                                      ((i<2)          ? 1.0f : 0.0f));
            pBuff++;
        }
        m_pVertexBuffer->Unlock();
    }
    CreateAndWriteUVOffsets(1024,768);//ddsd.Width, ddsd.Height);
    pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	renderToTex->restoreDeviceObjects(pd3dDevice);
	return S_OK;
}

void FilterBlit::initDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	renderToTex->initDeviceObjects(pd3dDevice);
	
}

HRESULT FilterBlit::InvalidateDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	renderToTex->invalidateDeviceObjects();
	SAFE_RELEASE(m_pEffect);
	return S_OK;
}


// displace texture-uvs so that the sample points on the 
// texture describe 
// i)   a square around the texel to sample.
// ii) a square around the texel to sample, so as to include sixteen texels:
// iii) A quad-sample filter:
void FilterBlit::CreateAndWriteUVOffsets(int width, int height)
{
    
    // first the easy one: no offsets
    float const     noOffsetX[4] = { 0.0f, 0.0f, 0.0f, 0.0f}; 
    float const     noOffsetY[4] = { 0.0f, 0.0f, 0.0f, 0.0f};

    float const     kPerTexelWidth  = 1.0f/static_cast<float>(width);
    float const     kPerTexelHeight = 1.0f/static_cast<float>(height);
    float           s               = 0.5f;
    float const     eps             = 10.0e-4f;
    float const     rotAngle1       = D3DXToRadian( 0.0f );
    float const     rotAngle2       = rotAngle1 + D3DXToRadian(120.0f); 
    float const     rotAngle3       = rotAngle1 + D3DXToRadian(240.0f); 

    // Change filter kernel for 9-sample box filtering, but for edge-detection we are 
    // going to use interpolated texels.  Why?  Because we detect diagonal edges only
    // and the vertical and horizontal filtering seems to help.
        
    float const type1OffsetX[4] = { -s * kPerTexelWidth, 
                                    -s * kPerTexelWidth,  
                                     s * kPerTexelWidth,   
                                     s * kPerTexelWidth  };
    float const type1OffsetY[4] = { -s * kPerTexelHeight, 
                                     s * kPerTexelHeight, 
                                     s * kPerTexelHeight, 
                                    -s * kPerTexelHeight };

    // we have to bring the 16 texel-sample-filter a bit closer to the center to avoid 
    // separation due to floating point inaccuracies.
    float const type2OffsetX[4] = { -1 * kPerTexelWidth + eps,  
                                    -1 * kPerTexelWidth + eps, 
                                    1.0f * kPerTexelWidth - eps, 
                                    1.0f * kPerTexelWidth - eps };
    float const type2OffsetY[4] = { -1 * kPerTexelHeight+ eps, 
                                    1.0f * kPerTexelHeight- eps, 
                                    1.0f * kPerTexelHeight- eps, 
                                    -1 * kPerTexelHeight+ eps };

    float const type3OffsetX[4] = {0.0f,  sinf(rotAngle1)*kPerTexelWidth,  
                                          sinf(rotAngle2)*kPerTexelWidth,  
                                          sinf(rotAngle3)*kPerTexelWidth  };
    float const type3OffsetY[4] = {0.0f, -cosf(rotAngle1)*kPerTexelHeight, 
                                         -cosf(rotAngle2)*kPerTexelHeight, 
                                         -cosf(rotAngle3)*kPerTexelHeight };

    s = 2.0f/3.0f;      // same as type 1, except s is different
    float const type4OffsetX[4] = { -s * kPerTexelWidth, 
                                    -s * kPerTexelWidth,  
                                     s * kPerTexelWidth,   
                                     s * kPerTexelWidth  };
    float const type4OffsetY[4] = { -s * kPerTexelHeight, 
                                     s * kPerTexelHeight, 
                                     s * kPerTexelHeight, 
                                    -s * kPerTexelHeight };

    // write all these offsets to constant memory
    for (int i = 0; i < 4; ++i)
    {
        D3DXVECTOR4  noOffset(      noOffsetX[i],    noOffsetY[i], 0.0f, 0.0f);
        D3DXVECTOR4  type1Offset(type1OffsetX[i], type1OffsetY[i], 0.0f, 0.0f);
        D3DXVECTOR4  type2Offset(type2OffsetX[i], type2OffsetY[i], 0.0f, 0.0f);
        D3DXVECTOR4  type3Offset(type3OffsetX[i], type3OffsetY[i], 0.0f, 0.0f);
        D3DXVECTOR4  type4Offset(type4OffsetX[i], type4OffsetY[i], 0.0f, 0.0f);

        // helpful comment:
        // the first 4 UvBase vectors are the 4 texture stage u/v's for "no-offset" sampling.
        // the next 4 UvBase vectors are the 4 texture stage u/v's for 9-sample box filter sampling,
        // and so on.

        char str[64];
        sprintf(str, "UvBase[%d]", i     ); 
        m_pEffect->SetVector(str, &noOffset);
        sprintf(str, "UvBase[%d]", i +  4); 
        m_pEffect->SetVector(str, &type1Offset);
        sprintf(str, "UvBase[%d]", i +  8); 
        m_pEffect->SetVector(str, &type2Offset);
        sprintf(str, "UvBase[%d]", i + 12); 
        m_pEffect->SetVector(str, &type3Offset);
        sprintf(str, "UvBase[%d]", i + 16); 
        m_pEffect->SetVector(str, &type4Offset);
    }
}



HRESULT FilterBlit::Render(LPDIRECT3DDEVICE9 pd3dDevice, IDirect3DTexture9 *tex)
{
        D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matWorld, 1.0f, 1.0f, 1.0f);
				
		if(isOn == true)
		{
		  pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
          // write to constant memory which uv-offsets to use
          float offset = 4;
          m_pEffect->SetValue("UvOffsetToUse", &offset, sizeof(float));

          // set render state 
		  pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(QuadVertex));
				        
		  m_pEffect->SetTechnique(technique);
		
          // draw the screen-aligned polygon
          UINT uPasses;
          m_pEffect->Begin(&uPasses, 0);   
            m_pEffect->BeginPass(0);                 
				m_pEffect->SetMatrix("WorldViewProj", &matWorld);
		        m_pEffect->SetTexture("colorTexture", tex);
				m_pEffect->SetInt("intensity", intensity);
				m_pEffect->CommitChanges();
				pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		    m_pEffect->EndPass();
          m_pEffect->End();
		}	
	    return S_OK;
}

HRESULT FilterBlit::RenderGlow(LPDIRECT3DDEVICE9 pd3dDevice, IDirect3DTexture9 *tex, char* tech, int i)
{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matWorld, 1.0f, 1.0f, 1.0f);
				
		  pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
          // write to constant memory which uv-offsets to use
          float offset = 4;
          m_pEffect->SetValue("UvOffsetToUse", &offset, sizeof(float));

          // set render state 
		  pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(QuadVertex));
				        
		  m_pEffect->SetTechnique(tech);
		  
          // draw the screen-aligned polygon
          UINT uPasses;
          m_pEffect->Begin(&uPasses, 0);   
            m_pEffect->BeginPass(0);                 
				m_pEffect->SetMatrix("WorldViewProj", &matWorld);
		        m_pEffect->SetTexture("colorTexture", tex);
				m_pEffect->SetInt("intensity", i);
				m_pEffect->CommitChanges();
				pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
			m_pEffect->EndPass();
          m_pEffect->End();
			
	    return S_OK;


}



