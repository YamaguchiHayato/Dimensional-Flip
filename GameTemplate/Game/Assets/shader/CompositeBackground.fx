Texture2D<float4> bgTexture : register(t0);
Texture2D<float4> depthTexture : register(t1);
SamplerState Sampler : register(s0);

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
    float4 screenParam; // z=画面幅, w=画面高さ
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

static const float kSkyBottomY = 0.78f; // 画面上部〜中央下まで（足場直上は深度マスクで除外）

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

    // 空（ZPrepass未描画）だけに表示
    float depth = depthTexture.Sample(Sampler, screenUv).z;
    if (abs(depth - 1.0f) > 0.03f)
        discard;

    // 画面上部の空だけ（下半分は捨てる）
    if (screenUv.y > kSkyBottomY)
        discard;

    float4 c = bgTexture.Sample(Sampler, input.uv);

    float lum = dot(c.rgb, float3(0.299, 0.587, 0.114));
    float formulaAlpha = c.a;
    if (formulaAlpha > 0.90f)
    {
        if (lum > 0.85f)
            discard;
        formulaAlpha = saturate(1.0f - lum);
    }

    if (formulaAlpha <= 0.02f)
        discard;

    // SPM風: 淡い白青のウォーターマーク
    c.rgb = lerp(c.rgb, float3(0.95, 0.97, 1.0), 0.82);
    c.rgb *= float3(0.82, 0.90, 1.0);

    float horizonFade = saturate((kSkyBottomY - screenUv.y) / kSkyBottomY);
    horizonFade = 0.62f + 0.38f * horizonFade;

    float alpha = formulaAlpha * mulColor.a * horizonFade;
    return float4(c.rgb * mulColor.rgb, alpha);
}
