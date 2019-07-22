#include "dxstdafx.h"
#include "Frustum.h"
#include ".\NodeTree.h"


cNodeTreeMesh::cNodeTreeMesh(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
  m_TreeType    = OCTREE;

  m_Graphics    = NULL;

  m_ParentNode  = NULL;

  m_NumGroups   = 0;
  m_Groups      = NULL;

  m_Polygons    = NULL;
  m_NumPolygons = 0;

  m_Min = min;
  m_Max = max;
}

cNodeTreeMesh::~cNodeTreeMesh()
{
  Free();
}

BOOL cNodeTreeMesh::Create(IDirect3DDevice9 *Graphics, CDXUTMesh *Mesh, 
                           int TreeType, float MaxSize, long MaxPolygons)
{
  ID3DXMesh      *LoadMesh;
  unsigned short *IndexPtr;
  unsigned long  *Attributes;
  float           MaxX, MaxY, MaxZ;
  unsigned long   i;
  

  // Free a prior mesh
  Free();
  
  cdMesh = Mesh;
  // Get mesh info
  m_Mesh        = Mesh->GetSysMemMesh();
  LoadMesh      = Mesh->GetSysMemMesh();
  m_VertexFVF   = LoadMesh->GetFVF();
  m_VertexSize  = D3DXGetFVFVertexSize(m_VertexFVF);
  m_NumPolygons = LoadMesh->GetNumFaces();
  m_MaxPolygons = MaxPolygons;

  // Create the polygon list and texture groups
  m_Polygons    = new sPolygon[m_NumPolygons]();
  m_NumGroups   = Mesh->m_dwNumMaterials;
  m_Groups      = new sGroup[m_NumGroups]();

  // Lock the index and attribute buffers
  LoadMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&IndexPtr);
  LoadMesh->LockAttributeBuffer(D3DLOCK_READONLY, &Attributes);

  // Load polygon information into structures
  for(i=0;i<m_NumPolygons;i++) {
    m_Polygons[i].Vertex0 = *IndexPtr++;
    m_Polygons[i].Vertex1 = *IndexPtr++;
    m_Polygons[i].Vertex2 = *IndexPtr++;

    m_Polygons[i].Group = Attributes[i];
    m_Polygons[i].Timer = 0;

    m_Groups[Attributes[i]].NumPolygons++;
  }

  // Unlock buffers
  LoadMesh->UnlockAttributeBuffer();
  LoadMesh->UnlockIndexBuffer();

  // Build the group vertex index buffers
  for(i=0;i<m_NumGroups;i++) {
    if(m_Groups[i].NumPolygons != 0)
      Graphics->CreateIndexBuffer(m_Groups[i].NumPolygons * 3 * sizeof(unsigned short),
          D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_Groups[i].IndexBuffer, NULL);
  }

  // Get the size of the bounding cube
  MaxX = (float)max(fabs(m_Min.x), fabs(m_Max.x));
  MaxY = (float)max(fabs(m_Min.y), fabs(m_Max.y));
  MaxZ = (float)max(fabs(m_Min.z), fabs(m_Max.z));
  m_Size    = max(MaxX, max(MaxY, MaxZ)) * 2.0f;
  m_MaxSize = MaxSize;

  // Create the parent node
  m_ParentNode = new sNode();

  // Sort polygons into nodes
  LoadMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&m_VertexPtr);
  SortNode(m_ParentNode, 0.0f, 0.0f, 0.0f, m_Size);
  LoadMesh->UnlockVertexBuffer();

  m_Timer = 0; // Reset timer

  return TRUE;
}

BOOL cNodeTreeMesh::Free()
{
  delete m_ParentNode;
  m_ParentNode = NULL;

  m_NumPolygons = 0;
  delete [] m_Polygons;
  m_Polygons = NULL;

  m_NumGroups = 0;
  delete [] m_Groups;
  m_Groups = NULL;

  m_Graphics = NULL;

  return TRUE;
}

void cNodeTreeMesh::SortNode(sNode *Node, 
                             float XPos, float YPos, float ZPos, 
                             float Size)
{
  unsigned long i, Num;
  float         XOff, YOff, ZOff;

  // Error checking
  if(Node == NULL)
    return;

  // Store node coordinates and size
  Node->XPos = XPos;
  Node->YPos = (m_TreeType==QUADTREE)?0.0f:YPos;
  Node->ZPos = ZPos;
  Node->Size = Size;

  // See if there are any polygons in the node
  if(!(Num = CountPolygons(XPos, YPos, ZPos, Size)))
    return;

  // Split node if size > maximum and too many polygons
  if(Size > m_MaxSize && Num > m_MaxPolygons) {
    for(i=0;i<(unsigned long)((m_TreeType==QUADTREE)?4:8);i++) {
      XOff = (((i % 2) < 1) ? -1.0f : 1.0f) * (Size / 4.0f);
      ZOff = (((i % 4) < 2) ? -1.0f : 1.0f) * (Size / 4.0f);
      YOff = (((i % 8) < 4) ? -1.0f : 1.0f) * (Size / 4.0f);

      // See if any polygons in new node bounding box
      if(CountPolygons(XPos+XOff,YPos+YOff,ZPos+ZOff,Size/2.0f)) {
        
        Node->Nodes[i] = new sNode();  // Create new child node

        // Sort the polygons with the new child node
        SortNode(Node->Nodes[i],XPos+XOff,YPos+YOff,ZPos+ZOff,Size/2.0f);
      }
    }

    return;
  }

  // Allocate space for vertex indices
  Node->NumPolygons = Num;
  Node->PolygonList = new unsigned long[Num];

  // Scan through polygon list, storing pointers and assigning them
  Num = 0;
  for(i=0;i<m_NumPolygons;i++) {
    // Add polygon to node list if contained in 3-D space.
    if(IsPolygonContained(&m_Polygons[i],
                          XPos, YPos, ZPos, Size) == TRUE)
      // Store polygon #
      Node->PolygonList[Num++] = i;
  }
}

BOOL cNodeTreeMesh::IsPolygonContained(
                         sPolygon *Polygon,
                         float XPos, float YPos, float ZPos,
                         float Size)
{
  float    XMin, XMax, YMin, YMax, ZMin, ZMax;
  sVertex *Vertex[3];

  // Get the polygon's vertices
  Vertex[0] = (sVertex*)&m_VertexPtr[m_VertexSize * Polygon->Vertex0];
  Vertex[1] = (sVertex*)&m_VertexPtr[m_VertexSize * Polygon->Vertex1];
  Vertex[2] = (sVertex*)&m_VertexPtr[m_VertexSize * Polygon->Vertex2];

  // Check against X axis of specified 3-D space
  XMin = min(Vertex[0]->x, min(Vertex[1]->x, Vertex[2]->x));
  XMax = max(Vertex[0]->x, max(Vertex[1]->x, Vertex[2]->x));
  if(XMax < (XPos - Size / 2.0f))
    return FALSE;
  if(XMin > (XPos + Size / 2.0f))
    return FALSE;

  // Check against Y axis of specified 3-D space (only if octree tree type)
  if(m_TreeType == OCTREE) {
    YMin = min(Vertex[0]->y, min(Vertex[1]->y, Vertex[2]->y));
    YMax = max(Vertex[0]->y, max(Vertex[1]->y, Vertex[2]->y));
    if(YMax < (YPos - Size / 2.0f))
      return FALSE;
    if(YMin > (YPos + Size / 2.0f))
      return FALSE;
  }
  	
  // Check against Z axis of specified 3-D space
  ZMin = min(Vertex[0]->z, min(Vertex[1]->z, Vertex[2]->z));
  ZMax = max(Vertex[0]->z, max(Vertex[1]->z, Vertex[2]->z));
  if(ZMax < (ZPos - Size / 2.0f))
    return FALSE;
  if(ZMin > (ZPos + Size / 2.0f))
    return FALSE;

  return TRUE;
}

unsigned long cNodeTreeMesh::CountPolygons(
                  float XPos, float YPos, float ZPos, float Size)
{
  unsigned long i, Num;
 
   // Return if no polygons to process
  if(!m_NumPolygons)
    return 0;

  // Go through every polygon and keep count of those 
  // contained in the specified 3-D space.
  Num = 0;
  for(i=0;i<m_NumPolygons;i++) {
    if(IsPolygonContained(&m_Polygons[i],
                          XPos,YPos,ZPos,Size) == TRUE)
      Num++;
  }

  return Num;
}

BOOL cNodeTreeMesh::Render(IDirect3DDevice9 *m_Graphics, cFrustum *Frustum, float ZDistance)
{
  D3DXMATRIX Matrix;     // Matrix used for calculations
  cFrustum ViewFrustum;  // Local viewing frustum
  IDirect3DVertexBuffer9 *pVB = NULL;
  unsigned long i;

  // Error checking
  if(m_Graphics == NULL || m_ParentNode == NULL || !m_NumPolygons)
    return FALSE;

  // Construct the viewing frustum (if none passed)
  if((m_Frustum = Frustum) == NULL) {
//    ViewFrustum.Construct(m_Graphics, ZDistance);
    m_Frustum = &ViewFrustum;
  }

  // Set the world transformation matrix to identity, so that
  // level mesh is rendered around the origin it was designed.
  D3DXMatrixIdentity(&Matrix);
  m_Graphics->SetTransform(D3DTS_WORLD, &Matrix);

  // Lock group index buffer
  for(i=0;i<m_NumGroups;i++) {
    if(m_Groups[i].NumPolygons) {
      m_Groups[i].IndexBuffer->Lock(
          0, m_Groups[i].NumPolygons * 3 * sizeof(unsigned short),
          (VOID**)&m_Groups[i].IndexPtr,0);
    }
    m_Groups[i].NumPolygonsToDraw = 0;
  }

  // Increase frame timer
  m_Timer++;

  // Add polygons to be drawn into group lists
  AddNode(m_ParentNode);
  
  // Get vertex buffer pointer
  //cdMesh->GetSysMemMesh()->GetVertexBuffer(&pVB);
  m_Mesh->GetVertexBuffer(&pVB);

  // Set vertex shader and source
  m_Graphics->SetStreamSource(0, pVB, m_VertexSize, m_VertexSize);
  //m_Graphics->SetVertexShader(m_VertexFVF);
  m_Graphics->SetFVF(m_VertexFVF);

  // Unlock vertex buffers and draw
  for(i=0;i<m_NumGroups;i++) {
    if(m_Groups[i].NumPolygons)
      m_Groups[i].IndexBuffer->Unlock();

    if(m_Groups[i].NumPolygonsToDraw) {
	  m_Graphics->SetMaterial(&cdMesh->m_pMaterials[i]);
	  m_Graphics->SetTexture(0, cdMesh->m_pTextures[i]);

      m_Graphics->SetIndices(m_Groups[i].IndexBuffer);

      m_Graphics->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 1,
								m_Mesh->GetNumVertices(),0,
                                m_Groups[i].NumPolygonsToDraw);
	  
    }
  }
  
  // Release vertex buffer
  if(pVB != NULL)
    pVB->Release();

  return TRUE;
}

void cNodeTreeMesh::AddNode(sNode *Node)
{
  unsigned long i, Group;
  sPolygon     *Polygon;
  short         Num;

  if(Node == NULL)
    return;

  // Perform frustum check based on tree type
  if(m_TreeType == QUADTREE) {
    if(m_Frustum->CheckRectangle(
           Node->XPos,        0.0f,          Node->ZPos, 
           Node->Size / 2.0f, m_Size / 2.0f, Node->Size / 2.0f) == FALSE)
      return;
  } else {
    if(m_Frustum->CheckRectangle(
           Node->XPos,        Node->YPos,        Node->ZPos, 
           Node->Size / 2.0f, Node->Size / 2.0f, Node->Size / 2.0f) == FALSE)
      return;
  }

  // Scan other nodes
  Num = 0;
  for(i=0;i<(unsigned long)((m_TreeType==QUADTREE)?4:8);i++) {
    if(Node->Nodes[i] != NULL) {
      Num++;
      AddNode(Node->Nodes[i]);
    }
  }

  // Don't need to go on if there was other nodes
  if(Num)
    return;

  // Add contained polygons (if any)
  if(Node->NumPolygons != 0) {

    for(i=0;i<Node->NumPolygons;i++) {

      // Get pointer to polygon
      Polygon = &m_Polygons[Node->PolygonList[i]];

      // Only draw if not done already this frame
      if(Polygon->Timer != m_Timer) {
      
        // Set polygon as processed this frame
        Polygon->Timer = m_Timer;

        // Get texture group of polygon
        Group = Polygon->Group;

        // Make sure group is okay and material is not transparent
		if(Group < m_NumGroups){ //&& m_Mesh->m_pMaterials[Group]].Diffuse.a != 0.0f) {

          // Copy indices into index buffer
          *m_Groups[Group].IndexPtr++ = Polygon->Vertex0;
          *m_Groups[Group].IndexPtr++ = Polygon->Vertex1;
          *m_Groups[Group].IndexPtr++ = Polygon->Vertex2;
      
          // Increase count of polygons to draw in group
          m_Groups[Group].NumPolygonsToDraw++;
        }
      }
    }
  }
}