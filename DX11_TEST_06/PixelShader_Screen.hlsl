Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer BrightnessBuffer : register(b0)
{
    float brightness;
    float3 padding;
}

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
    float4 screen = tex.Sample(samp, uv);
    screen.rgb *= brightness;
    return screen;
}