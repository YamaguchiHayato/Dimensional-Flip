Texture2D<float4> bgTexture : register(t0);
Texture2D<float4> depthTexture : register(t1);
SamplerState Sampler : register(s0);

cbuffer cb : register(b0)
{
    float4x4 mvp;      //! 
    float4 mulColor;   //! 
};

struct VSInput
{
    float4 pos : POSITION; //! 
    float2 uv : TEXCOORD0; //! 
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PSInput VSMain(VSInput In)
{
    PSInput o;
    o.pos = mul(mvp, In.pos);
    o.uv = In.uv;
    return o;
}


float4 PSMain(PSInput input) : SV_Target
{
    float depth = depthTexture.Sample(Sampler, input.uv).z;
    if (abs(depth - 1.0f) > 0.001f)
        discard;

    if (input.uv.y > 0.55)   // 0.50〜0.65 で調整
        discard;

    float4 c = bgTexture.Sample(Sampler, input.uv);
    if (c.a <= 0.0f)
        discard;
    c.rgb /= max(c.a, 0.0001f);

    c.rgb = lerp(c.rgb, float3(1, 1, 1), 0.85);
    c.rgb *= float3(0.75, 0.85, 1.0);

    float horizonFade = saturate((0.55 - input.uv.y) / 0.45);
    horizonFade *= horizonFade;

    float alpha = c.a * mulColor.a * horizonFade;
    return float4(c.rgb * mulColor.rgb, alpha);
}
