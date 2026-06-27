Texture2D<float4> bgTexture : register(t0);
Texture2D<float4> depthTexture : register(t1);
SamplerState Sampler : register(s0);

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
    float4 screenParam;
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
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
    float2 screenSize = max(screenParam.zw, float2(1.0f, 1.0f));
    float2 screenUv = input.pos.xy / screenSize;

    // [核心] モデル（ZPrepass 済み）より手前に描かない
    float depth = depthTexture.Sample(Sampler, screenUv).z;
    if (abs(depth - 1.0f) > 0.03f)
        discard;

    float4 c = bgTexture.Sample(Sampler, input.uv) * mulColor;
    if (c.a <= 0.02f)
        discard;

    return c;
}
