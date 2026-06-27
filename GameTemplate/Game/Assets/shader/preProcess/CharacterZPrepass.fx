///////////////////////////////////////
// CharacterZPrepass（Player 用・アルファ付き ZPrepass）
///////////////////////////////////////

struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 depth : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

#include "../ModelVSCommon.h"

Texture2D<float4> g_albedo : register(t0);
sampler g_sampler : register(s0);

SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.pos = mul(mView, psIn.pos);
    psIn.depth.z = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.depth.x = psIn.pos.z / psIn.pos.w;
    psIn.depth.y = saturate(psIn.pos.w / 1000.0f);
    psIn.uv = vsIn.uv;
    return psIn;
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedo = g_albedo.Sample(g_sampler, psIn.uv);
    clip(albedo.a - 0.2f); // GBuffer と同じ閾値

    return float4(psIn.pos.z, psIn.depth.y, psIn.depth.z, 1.0f);
}
