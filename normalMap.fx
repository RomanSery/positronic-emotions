String XFile = "meshes/box.x";
float4x4 WorldI : WORLDINVERSE;
float4x4 World  : WORLD;
float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float3   camPos  : CAMERAPOSITION;

#define NUM_LIGHTS 2
float4 LightPos[NUM_LIGHTS];

//float4 LightColor = { 1.0f, 1.0f, 1.0f, 0.0f };
//float4 AmbiColor = {0.2f, 0.2f, 0.2f, 1.0f};


texture diffuseTexture
<
  string name = "media/STWL_T04.jpg";
>;

texture normalMap : NormalMap
<
  string name = "bedia/STWL_T04.jpg";
>;

sampler2D cmap = sampler_state
{
    Texture = <diffuseTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


sampler2D normalSampler = sampler_state 
{
    Texture = <normalMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


/************* STRUCTS **************/

// vertex input
struct VertexIn {
    float4 Position  : POSITION; //in object space
    float2 UV        : TEXCOORD0; //in object space
    float4 Normal    : NORMAL; //in object space
    float3 T         : TANGENT; //in object space
    float3 B         : BINORMAL; //in object space
};

//vertex output
struct VertexOut {
    float4 HPosition    : POSITION;
    float2 texCoord0    : TEXCOORD0;
    float2 texCoord1    : TEXCOORD1;
    float3 LightVector[NUM_LIGHTS]  : TEXCOORD2;


};



//vertex shader*************************************
VertexOut DiffuseBumpVS(VertexIn IN)
{
    //create the vertex out struct
    VertexOut OUT;
    
    OUT.texCoord0 = IN.UV; //pass coords for diffuse map
    OUT.texCoord1 = IN.UV; //pass coords for normal map
    
    // compute the 3x3 tranform from tangent space to object space
    float3x3 objToTangentSpace;
    objToTangentSpace[0] = IN.B;
    objToTangentSpace[1] = -IN.T;
    objToTangentSpace[2] = IN.Normal;
    
    //put the vert position in world space
    float4 worldSpacePos = mul(IN.Position, World);
    
    //calculate the lighting vector for each light
    for(int i = 0; i < NUM_LIGHTS; i++){
      //cast a ray to the light
      float4 normLightVec = LightPos[i] - worldSpacePos;

      //transform the light vector to object space
      float3 objnormLightVec = normalize(mul(normLightVec, World));
    
      // transform light vector from object space to tangent space and pass it as a color 
      OUT.LightVector[i] = mul(objToTangentSpace, objnormLightVec);

    }    
        

    // transform position to projection space
    OUT.HPosition = mul(IN.Position, WorldViewProj);

    return OUT;
}


//pixel shader*************************************
float4 myps(VertexOut IN) : COLOR0
{
    float b = 0.0f;
    
    //get the color from the diffuse texture
    float4 texColor = tex2D(cmap, IN.texCoord0);
    
    //get the color from the normal map and convert to normal
    float4 bumpNormal = 2 * (tex2D(normalSampler, IN.texCoord1)-0.5);
    
    //sum the contribution of each light
    for(int i = 0; i < NUM_LIGHTS; i++){
      //unpack the light vector to [-1,1]
      float3 lightVector = 2 * (IN.LightVector[i] - 0.5);
          
      //compute the angle to the light and clamp at zero
      float bump = max(dot(bumpNormal, lightVector), 0.0);
      b += bump;
    }
        
    //compute final color (diffuse + ambient)
    return texColor * b;

}




//MAIN*************************************
technique diffuseBump
{
    pass p0 
    {    
        VertexShader = compile vs_1_1 DiffuseBumpVS();
        PixelShader = compile ps_1_1 myps();        
       
    }    

    pass p1
    {    
        VertexShader = compile vs_1_1 DiffuseBumpVS();
        PixelShader = compile ps_1_1 myps();        
        
        DestBlend = DESTCOLOR;
        SrcBlend = SRCCOLOR;
        BlendOp = MAX;
        AlphaBlendEnable = True;
    }
    
}
