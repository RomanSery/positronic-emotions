string XFile = "meshes/idle.x";

float4x4 WorldViewProj : WORLDVIEWPROJECTION;
float    UvOffsetToUse;
float4   UvBase[20];      // index = 4*effect[0..4] + texstage[0..3]
float    intensity = 6;


texture colorTexture
<
   string name = "media/brkrun.jpg";
    
>;


sampler2D cmap = sampler_state
{
    Texture = <colorTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


/////////////////////////////////////

struct VS_INPUT {
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT {
    float4 Position  : POSITION;
    float2 TexCoord0 : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
    float2 TexCoord2 : TEXCOORD2;
    float2 TexCoord3 : TEXCOORD3;

};

/////////////////////////////////////

VS_OUTPUT SimpleVS(VS_INPUT IN)
{
    VS_OUTPUT OUT;
        
    // Transform vertex-position to clip-space
    OUT.Position = mul(IN.Position, WorldViewProj);
    
    // Read which set of offsets to use
    int offset = (int)UvOffsetToUse * 4;
    
    // copy uv-coordinates to all four texture stages
    // and offset them according to a0
    OUT.TexCoord0 = IN.TexCoord + UvBase[offset    ].xy;
    //OUT.TexCoord0 = IN.TexCoord;
    OUT.TexCoord1 = IN.TexCoord + UvBase[offset + 1].xy * intensity;
    OUT.TexCoord2 = IN.TexCoord + UvBase[offset + 2].xy * intensity;
    OUT.TexCoord3 = IN.TexCoord + UvBase[offset + 3].xy * intensity;
    
    return OUT;    
}        


float4 PostProcessPS( VS_OUTPUT OUT ) : COLOR
{

    float4 Color = 0;
   
    float4 t0 = tex2D( cmap, OUT.TexCoord0);
    float4 t1 = tex2D( cmap, OUT.TexCoord1) * 0.25;
    float4 t2 = tex2D( cmap, OUT.TexCoord2) * 0.25;
    float4 t3 = tex2D( cmap, OUT.TexCoord3) * 0.25;
    
    Color = t0 + t1 + t2 + t3;
    return Color * 2;
    
    
}

float4 PostProcessPSWindow( VS_OUTPUT OUT ) : COLOR
{

    float4 Color = 0;
   
    float4 t0 = tex2D( cmap, OUT.TexCoord0);
    float4 t1 = tex2D( cmap, OUT.TexCoord1) * 0.25;
    float4 t2 = tex2D( cmap, OUT.TexCoord2) * 0.25;
    float4 t3 = tex2D( cmap, OUT.TexCoord3) * 0.25;

    Color = t0 + t1 + t2 + t3;
    Color.b -= 0.8;
    return Color;
    
}

//////////////////////////////////////////////////

technique Glow {
    
    pass p0 
    
    {
        VertexShader = compile vs_1_1 SimpleVS();
        PixelShader = compile ps_1_1 PostProcessPS();

        AlphaBlendEnable = True;
        ZWRITEENABLE = False;
        CullMode = None;
    }
}

technique Window {
    
    pass p0 
    
    {
        VertexShader = compile vs_1_1 SimpleVS();
        PixelShader = compile ps_1_1 PostProcessPSWindow();

        AlphaBlendEnable = True;
        ZWRITEENABLE = False;
        CullMode = None;
    }
}


technique Blur {
    
    pass p0 
    
    {
        VertexShader = compile vs_1_1 SimpleVS();
        
        PixelShader  = 
        asm
        {
            ; ************************************************************************
            ; Simple pixel shader that samples all 4 texture stages, averages these
            ; samples and outputs the result.
            ; ***********************************************************************
            
            ; Declare pixel shader version
            ps.1.1
            
            ; define constants
            def c0, 0.25f, 0.25f, 0.25f, 0.25f    
            
            ; get colors from all 4 texture stages
            tex t0
            tex t1
            tex t2
            tex t3
            
            ; and average them according to weights in constant mem:
            ; r0 = 0.25*t0 + 0.25*t1 + 0.25*t2 + 0.25*t3
            mul r0, c0, t0
            mad r0, c0, t1, r0
            mad r0, c0, t2, r0
            mad r0, c0, t3, r0
            
        };
        Sampler[0] = <cmap>;
        Sampler[1] = <cmap>;
        Sampler[2] = <cmap>;
        Sampler[3] = <cmap>;
        
        
        CullMode = CW;
    }
}

Technique Sharpen
{
    Pass P0
    {
        VertexShader = compile vs_1_1 SimpleVS();
        PixelShader  = 
        asm
        {
           
            
            ; Declare pixel shader version
            ps.1.1
                
            ; sample all 4 texture stages
            tex t0          // this is the center sample
            tex t1
            tex t2
            tex t3
            
            ; compute r0 = t0 - t1 + t0 - t2 + t0 - t3 + t0
            mov     r0, t0
            sub     r0, r0,   t1
            add     r0, r0,   t0
            sub     r0, r0,   t2
            add     r0, r0,   t0
            sub     r0, r0,   t3
            add_sat r0, r0,   t0
        };
        
        Sampler[0] = <cmap>;
        Sampler[1] = <cmap>;
        Sampler[2] = <cmap>;
        Sampler[3] = <cmap>;
        
        CullMode = None;
    }
}

Technique Luminance
{
    Pass P0
    {
        VertexShader = compile vs_1_1 SimpleVS();
        PixelShader  = 
        asm
        {
            ; ************************************************************************
            ; This pixel shader samples the first texture stage only and converts 
            ; the color-value obtained into a luminance-value.
            ; ************************************************************************
            
            ; Declare pixel shader version
            ps.1.1
            
            ; declare constants:
            def c1, 0.3f, 0.59f, 0.11f, 0.0f    ; luminance conversion constant
            
            ; get color from first texture stage only
            tex t0
            
            ; convert color to luminance and output
            dp3    r0.rgba, t0, c1        
        };
        
        Sampler[0] = <cmap>;
        CullMode = CW;
    }
}

Technique LuminanceDiagEdge
{
    Pass P0
    {
        VertexShader = compile vs_1_1 SimpleVS();
        PixelShader  = 
        asm
        {
            ; Declare pixel shader version
            ps.1.1
            
            ; declare constants:
            def c1, 0.3f, 0.59f, 0.11f, 0.0f    ; luminance conversion constant
            
            ; get colors from all 4 texture stages
            tex t0
            tex t1
            tex t2
            tex t3
            
            dp3    r0.rgba, t0, c1        // convert t0 color to luminance, store in r0.a
            dp3    r1.rgba, t2, c1        // convert t2 color to luminance, store in r1.a
            
            dp3    r0.rgb, t1, c1        // convert t1 color to luminance, store in r0.rgb
            dp3    r1.rgb, t3, c1        // convert t3 color to luminance, store in r1.rgb
            
            ; Both .rgb and .a pipes are used in the following
            sub_x4 r0, r0, r1        // take both differences   (and keep oversaturating the colors)
            mul_x4 r0, r0, r0        // square both differences (instead of abs())
            
            ; Recombine .rgb and .a values
            sub_sat    r0.rgb, 1-r0, r0.a    // invert and add the 2 components:
        };
        
        Sampler[0] = <cmap>;
        //Sampler[1] = <cmap>;
        //Sampler[2] = <cmap>;
        //Sampler[3] = <cmap>;
        CullMode = None;
    }
}
