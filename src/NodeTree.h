#ifndef _NODETREE_H_
#define _NODETREE_H_
#include "Frustum.h"

// Enumerate the two types of tree structures
enum TreeTypes { QUADTREE = 0, OCTREE = 1 };

class cNodeTreeMesh
{
  private:
    // The sVertex structure is a custom vertex structure that
    // contains only the 3-D coordinates. This is used to
    // retrieve coordinate info from a mesh's vertex buffer.
    typedef struct sVertex { float x, y, z; } sVertex;

    // Polygon information
    typedef struct sPolygon {
      unsigned short Vertex0;        // Vertex index #1
      unsigned short Vertex1;        // Vertex index #2
      unsigned short Vertex2;        // Vertex index #3
      unsigned long  Group;          // Texture group
      unsigned long  Timer;          // Frame last drawn
    } sPolygon;

    // The node structure keeps count of the # of polygons in
    // its 3-D space, an array of sPolygon structures, the
    // 3-D coordinates of the node (as well as the size, which
    // is the distance from the center to one edge making the node
    // a perfect cube), and pointers to the child nodes.
    typedef struct sNode {
      float           XPos, YPos, ZPos; // Center coordinate of node
      float           Size;             // Size of node

      unsigned long   NumPolygons;      // # of polygons in node
      unsigned long  *PolygonList;      // Polygon list

      sNode          *Nodes[8];         // Child nodes 4=quad, 8=oct

      // Constructor used to clear out variables
      sNode()
      {
        XPos = YPos = ZPos = Size = 0.0f; // Position and size
        NumPolygons = 0;        // Set to no polygons in node
        PolygonList = NULL;     // Clear polygon list
        for(short i=0;i<8;i++)  // Quadtrees only uses first 4
          Nodes[i] = NULL;      // Clear out child node pointers
      }

      // Destructor to clear child nodes and variables
      ~sNode()
      {
        delete [] PolygonList;  // Delete list of polygons
        for(short i=0;i<8;i++)
          delete Nodes[i];      // Delete child nodes
      }
    } sNode;

    // The texture group structure uses a cVertexBuffer class
    // from the Graphics Core to store polygons that need to
    // be rendered in a single frame, also it maintains the
    // number of polygons in a texture group and how many
    // polygons to draw each frame.
    typedef struct sGroup {
      unsigned long  NumPolygons;       // # polygons in group
      unsigned long  NumPolygonsToDraw; // # polygon to draw

      IDirect3DIndexBuffer9 *IndexBuffer;
      unsigned short        *IndexPtr;

      // Clear # polygons
      sGroup() { NumPolygons = 0; IndexBuffer = NULL; }  

      // Free index buffer
      ~sGroup() { if(IndexBuffer != NULL) IndexBuffer->Release(); }
    } sGroup;

    int            m_TreeType;     // Type of nodetree
                                   // QUADTREE or OCTREE

    IDirect3DDevice9 *m_Graphics;     // Parent cGraphics object
    cFrustum      *m_Frustum;      // Viewing frustum

    float          m_Size;         // Size of world cube
    float          m_MaxSize;      // Maximum node size

    sNode         *m_ParentNode;   // Node linked list parent

    unsigned long  m_NumGroups;    // # of texture groups
    sGroup        *m_Groups;       // Texture groups

    unsigned long  m_MaxPolygons;  // Maximum # of polygons per node

    unsigned long  m_NumPolygons;  // # polygons in scene
    sPolygon      *m_Polygons;     // List of polygons

    unsigned long  m_Timer;        // Current draw timer

    LPD3DXMESH    m_Mesh;         // Parent mesh
	CDXUTMesh	  *cdMesh;
    char          *m_VertexPtr;    // Pointer to mesh vertices
    unsigned long  m_VertexFVF;    // Mesh vertex FVF
    unsigned long  m_VertexSize;   // Size of mesh vertex

	D3DXVECTOR3 m_Min, m_Max;

    // SortNode groups the polygons into nodes and splits
    // the nodes into child nodes as needed.
    void           SortNode(sNode *Node,                      \
                         float XPos, float YPos, float ZPos,  \
                         float Size);

    // AddNode adds a node into the list of nodes to draw.
    void           AddNode(sNode *Node);

    // IsPolygonContained return TRUE if a polygon's bounding
    // box intersects with the specified cube space.
    BOOL           IsPolygonContained(
                         sPolygon *Polygon,
                         float XPos, float YPos, float ZPos,  \
                         float Size);

    // CountPolygons returns the # of polygons in a 3-D cube.
    unsigned long  CountPolygons(                             \
                         float XPos, float YPos, float ZPos,  \
                         float Size);

  public:
    cNodeTreeMesh(D3DXVECTOR3 min, D3DXVECTOR3 max);   // Constructor
    ~cNodeTreeMesh();  // Destructor

    // Functions to create and free a node/tree mesh from
    // a source cMesh object, specifying the maximum # of
    // polygons in an area that is larger than a specific
    // size (forcing node splits).
    BOOL Create(IDirect3DDevice9 *Graphics, CDXUTMesh *Mesh,             \
              int TreeType = OCTREE,                          \
              float MaxSize = 256.0f, long MaxPolygons = 512);
    BOOL Free();

    // Render the current view using the current view
    // transformation and overloaded distance of view.
    // Also specify to use a pre-calculate frustum or
    // force a calculation of own frustum.
    BOOL Render(IDirect3DDevice9 *Graphics, cFrustum *Frustum = NULL, float ZDistance = 0.0f);
    
};

#endif
