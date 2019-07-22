string XFile = "meshes/idle.x";                // model

// light directions (view space)
float3 DirFromLight = {0.5, 0.5, 0.0};

// light intensity
float4 LightAmbientIntensity    = { 0.8f, 0.8f, 0.8f, 1.0f };    // ambient
float4 SpecularColor = { 0.8f, 0.8f, 0.0f, 1.0f };


// material reflectivity
float4 MaterialAmbientIntensity = { 0.5f, 0.5f, 0.5f, 1.0f };    // ambient


// transformations
float4x3 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float3 CameraPos : CAMERAPOSITION;  


texture EnvironmentMap 
< 
    string type = "CUBE"; 
    string name = "media/cubeMap.dds";
>;

texture BaseMap 
< 
    string type = "2D"; 
    string name = "media/glass43.jpg";
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
    Texture = (BaseMap);
    MipFilter = Linear;
    MinFilter = Linear;
    MagFilter = Linear;
    addressu = wrap;
    addressv = wrap;
};



float4 CalcSpecular(float3 Normal, float3 DirFromLight, float3 EyeToVertex)
{
    float3 R = normalize(reflect(DirFromLight, Normal));
    return pow(max(0, dot(R, -EyeToVertex)), 128) * SpecularColor;
}




// vertex shader output structure
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Spec : COLOR1;
    float3 CubeTexcoord : TEXCOORD0;
    float3 BaseTexcoord : TEXCOORD1;    
};

VS_OUTPUT VS(
    float3 InPos  : POSITION,   // Vertex position in model space
    float3 InNormal : NORMAL,   // Vertex normal in model space
    float3 InTex    : TEXCOORD0)    
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 Pos = mul(float4(InPos, 1), (float4x3)World);         // position (view space)
    float3 Normal = normalize(mul(InNormal, (float3x3)World));   // normal (view space)
    float3 EyeToVertex = normalize(Pos - CameraPos);             // vector from vertex towards eye

    float4x4 ViewProjection = mul(View, Projection);
    Out.Pos  = mul(float4(Pos, 1), ViewProjection);              // position (projected)
    

    DirFromLight = InPos - CameraPos;
    Out.Spec += CalcSpecular(Normal, DirFromLight, EyeToVertex);
    Out.CubeTexcoord = reflect(EyeToVertex, Normal);
    Out.BaseTexcoord = InTex;
    
    return Out;
}


float4 PS ( VS_OUTPUT In) : COLOR
{
    float4 OutColor;
    
    float4 Environment = texCUBE(EnvironmentSampler, In.CubeTexcoord);
    float4 Base = tex2D(BaseSampler, In.BaseTexcoord) / 2;

    Environment = lerp(0, Environment, 0.90);
    OutColor = Base + Environment;

    OutColor += In.Spec;
    OutColor.rgb -= 0.2;
    return OutColor;
}



technique RenderWindow
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
                

    }
}