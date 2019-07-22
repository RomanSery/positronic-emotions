string XFile = "meshes/car.x";   // model


float4 lightPos = float4(0,1,0,1);
float4 windowTint = float4(0.4, 0.0, 0.0, 1.0);
bool   areLightsOn;                 


texture baseTex;
texture cubeTexture
<
  string type = "CUBE";
  string name = "media/cubeMap.dds";
>;

// Transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float3 CameraPos    : CAMERAPOSITION;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Spec : COLOR1;
    float2 Tex  : TEXCOORD0;
    float3 CubeTex : TEXCOORD1;
    
};

float4 DoPointLight(float4 vPosition, float3 N)
{
  lightPos = normalize(lightPos);
  float4x4 WorldView = mul(World, View);
  float3 L = mul((float3x3)WorldView, lightPos);

  return max(0.1, dot(N, L));

}


VS_OUTPUT VS(
    float3 Pos     : POSITION, 
    float3 Norm    : NORMAL, 
    float2 Tex     : TEXCOORD0,
    float4 Col     : COLOR0)
    
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    float3 L = mul((float3x3)View, lightPos);
    float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);  // position (view space)
    float3 N = normalize(mul(Norm, (float3x3)WorldView)); // normal (view space)
    float3 EyeToVertex = normalize(Pos - CameraPos);             
    float3 R = normalize(2 * dot(N, L) * N - L);          // reflection vector (view space)
    float3 V = -normalize(P);                             // view direction (view space)
    

    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Diff = DoPointLight(lightPos, N); 
    Out.Diff.a = Col.a;
    Out.Spec = pow(max(0, dot(R, V)), 24);   // specular
    Out.Tex  = Tex; 
    Out.CubeTex = reflect(EyeToVertex, N);                                      
    return Out;
}



sampler2D Sampler = sampler_state
{
    Texture   = (baseTex);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

samplerCUBE cubeSampler = sampler_state
{
    Texture   = (cubeTexture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


float4 PScarBody(VS_OUTPUT IN) : COLOR
{
    //base color
    float4 color = tex2D(Sampler, IN.Tex);
    
    //enviornment color
    float4 color2 = texCUBE(cubeSampler, IN.CubeTex) * 2; 

    
    if(color.r == 0){   //rendering window, add window tint color
      float4 result = color2 * IN.Diff + IN.Spec;
      result += windowTint;
      return result;
    }
    //rendering car body
    else {            
      return color2 * IN.Diff + IN.Spec;
    }
}



//lights, lines alpha value of 0
float4 PScarLightsLines(VS_OUTPUT IN) : COLOR
{
    float4 color = tex2D(Sampler, IN.Tex);
    if(areLightsOn == true)
     return color * 2;
    else
     return color / 2;
    
  
}



float4 PScarWheels(VS_OUTPUT IN) : COLOR
{
    float4 color = tex2D(Sampler, IN.Tex);
    float4 wheelColor = (1, 1, 0.0, 1.0);
    
    //wheel rim-green color in 3dmax
    if(color.g == 1){  
     return wheelColor * IN.Diff + IN.Spec;
    }
    else
     return color;
}



technique RenderCarBody
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PScarBody();
        
    }
    pass P1
    {
        
        PixelShader  = compile ps_1_1 PScarLightsLines();
    }
}

technique RenderCarWheels
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PScarWheels();

    }
}



