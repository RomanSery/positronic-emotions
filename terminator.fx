string XFile = "meshes/robot.x";                // model

// position of closest light
float3 lightPos;


// transformations
float4x3 World          : WORLD;
float4x4 ViewProjection : VIEWPROJECTION;
float3   CameraPos      : CAMERAPOSITION;  
float    Time           : TIME;

texture baseTex;
texture EnvironmentMap 
< 
    string type = "CUBE"; 
    string name = "media/cubeMap.dds";
>;

samplerCUBE EnvironmentSampler = sampler_state
{ 
    Texture = (EnvironmentMap);
    MipFilter = Linear;
    MinFilter = Linear;
    MagFilter = Linear;
    addressu = wrap;
    addressv = wrap;
};

sampler2D BaseSampler = sampler_state
{ 
    Texture = (baseTex);
    MipFilter = Linear;
    MinFilter = Linear;
    MagFilter = Linear;
    addressu = wrap;
    addressv = wrap;
};



float4 CalcDiffuse(float3 Normal, float3 DirToLight)
{
    return max(0, dot(Normal, DirToLight));
}

float3 CalcVertexAnimation(float3 Offset)
{
    return sin(Time * Offset) * 0.1;        
}


float4 CalcSpecular(float3 Normal, float3 DirFromLight, float3 EyeToVertex)
{
    float3 R = normalize(reflect(DirFromLight, Normal));
    return max(0, dot(R, -EyeToVertex)) / 1;
}



// vertex shader output structure
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Spec : COLOR1;
    float3 CubeTexcoord : TEXCOORD0;    
    float3 TexBase : TEXCOORD1;    

};


///////////////// Normal rendering vs and ps shaders /////////////


VS_OUTPUT VS(
    float3 InPos  : POSITION,   // Vertex position in model space
    float3 InNormal : NORMAL,   // Vertex normal in model space
    float3 InTex   : TEXCOORD0)    
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 Pos = mul(float4(InPos, 1), (float4x3)World);         // position (view space)
    float3 Normal = normalize(mul(InNormal, (float3x3)World));   // normal (view space)
    float3 EyeToVertex = normalize(Pos - CameraPos);             // vector from vertex towards eye

    // calculate the lighting terms and output Position
    float3 DirFromLight = lightPos - Pos;
    Out.Pos  = mul(float4(Pos, 1), ViewProjection);     // position (projected)
    Out.Diff += CalcDiffuse(Normal, DirFromLight); 
    Out.CubeTexcoord = reflect(EyeToVertex, Normal);
    Out.TexBase = InTex;
    
    return Out;
}

float4 PS ( VS_OUTPUT In) : COLOR
{
    float4 OutColor;
    
    float4 Base = tex2D(BaseSampler, In.TexBase);
    float4 Environment = texCUBE(EnvironmentSampler, In.CubeTexcoord);

    Environment = lerp(0, Environment, 2.00);
    OutColor = (Base * Environment) * In.Diff;
  
    return OutColor;
}


//////////////////////// vs and ps shaders for transforming effect //////////////


VS_OUTPUT VS_transform(
    float3 InPos  : POSITION,   // Vertex position in model space
    float3 InNormal : NORMAL)   // Vertex normal in model space
        
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 Pos = mul(float4(InPos, 1), (float4x3)World);         // position (view space)
    float3 Normal = normalize(mul(InNormal, (float3x3)World));   // normal (view space)
    float3 EyeToVertex = normalize(Pos - CameraPos);             // vector from vertex towards eye
    
    

    Pos += CalcVertexAnimation(InPos);

    Out.Pos  = mul(float4(Pos, 1), ViewProjection);              // position (projected)
        

    // calculate the selected lighting terms
    float3 DirFromLight = lightPos - Pos;
    Out.Diff += CalcDiffuse(Normal, -DirFromLight); 
    Out.Diff.r = tan(Time) * 10;
    Out.Diff.g = sin(Time) * 0.1;
    Out.Diff.b = cos(Time) * 2;

    Out.Spec += CalcSpecular(Normal, DirFromLight, EyeToVertex);
    Out.CubeTexcoord = reflect(EyeToVertex, Normal);
    
    return Out;
}


float4 PS_transform ( VS_OUTPUT In) : COLOR
{
    float4 OutColor;
    
    float4 Environment = texCUBE(EnvironmentSampler, In.CubeTexcoord);
    OutColor = lerp(In.Diff, Environment, 2.00);
    
    OutColor += In.Spec;
      
    return OutColor;
}

//////////////// rendering human form ////////////////
VS_OUTPUT VS_human(
    float3 InPos  : POSITION,   // Vertex position in model space
    float3 InNormal : NORMAL,   // Vertex normal in model space
    float3 InTex    : TEXCOORD0)   
        
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 Pos = mul(float4(InPos, 1), (float4x3)World);         // position (view space)
    float3 Normal = normalize(mul(InNormal, (float3x3)World));   // normal (view space)

    // calculate the lighting terms and output Position
    float3 DirFromLight = lightPos - Pos;
    Out.Pos  = mul(float4(Pos, 1), ViewProjection);     // position (projected)
    Out.Diff += CalcDiffuse(Normal, DirFromLight); 
    Out.CubeTexcoord = InTex;    

    return Out;
}


float4 PS_human ( VS_OUTPUT In) : COLOR
{
    return tex2D(BaseSampler, In.CubeTexcoord) * In.Diff;
}




technique render
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
    }
}

technique renderHuman
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS_human();
        PixelShader  = compile ps_1_1 PS_human();
    }
}

technique transform
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS_transform();
        PixelShader  = compile ps_1_1 PS_transform();
    }
}

