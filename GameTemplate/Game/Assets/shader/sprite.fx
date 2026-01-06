////////////////////////////////
/// スプライトシェーダー
////////////////////////////////

struct VSInput
{
    float4 pos : POSITION; //位置
    float2 uv : TEXCOORD0; //UV
};

struct PSInput
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD0; //UV
};

struct LinearWipe
{
    float2 direction;
    float size;
};

//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp; //MVP行列
    float4 mulColor; //乗算カラー
};

//スプライトレンダーの定数バッファ
cbuffer SpriteRenderCb : register(b1)
{
    LinearWipe linearWipe; //リニアワイプ
    int linearWipeDrawingMode; //リニアワイプの描画モード
    float drawingRate; //画像加工用イージング割合
    int screenDrawingMode; //画像加工

    // UVScroll用。
    float2 uvOffset;
    
};

Texture2D<float4> colorTexture : register(t0); //カラーテクスチャ
sampler Sampler : register(s0); //サンプラー

void CalcLinearWipeFromNormal(PSInput In);
void CalcLinearWipeFromDirection(PSInput In);
void CalcLinearWipeFromRound(PSInput In);
void CalcLinearWipeFromVertical(PSInput In);
void CalcLinearWipeFromHorizontal(PSInput In);
void CalcLinearWipeFromCheckerBoard(PSInput In);
float4 CalcMonochrome(float4 color);
float4 CalcSepia(float4 color);
float4 CalcNega(float4 color);
float4 CalcNoise(PSInput In, float4 color);

//頂点シェーダー
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

//ピクセルシェーダー
float4 PSMain(PSInput In) : SV_Target0
{
    float2 scrollUV = In.uv + uvOffset;
    
    float4 color = colorTexture.Sample(Sampler, scrollUV) * mulColor;
    
    //リニアワイプの描画モード
    switch (linearWipeDrawingMode)
    {
        case 0: //通常ワイプ
            CalcLinearWipeFromNormal(In);
            break;
        case 1: //方向ワイプ
            CalcLinearWipeFromDirection(In);
            break;
        case 2: //円形ワイプ
            CalcLinearWipeFromRound(In);
            break;
        case 3: //縦じまワイプ
            CalcLinearWipeFromVertical(In);
            break;
        case 4: //横じまワイプ
            CalcLinearWipeFromHorizontal(In);
            break;
        case 5: //チェッカーボードワイプ
            CalcLinearWipeFromCheckerBoard(In);
            break;
        default:
            break;
    }
    
    //画像加工
    switch (screenDrawingMode)
    {
        case 0: //モノクロ
            color = CalcMonochrome(color);
            break;
        case 1: //セピア
            color = CalcSepia(color);
            break;
        case 2: //ネガポジ反転
            color = CalcNega(color);
            break;
        case 3: //ノイズ
            color = CalcNoise(In, color);
            break;
        default:
            break;
    }

    return color;
}

//通常ワイプ
void CalcLinearWipeFromNormal(PSInput In)
{
    clip(In.pos.x - linearWipe.size);
}

//方向ワイプ
void CalcLinearWipeFromDirection(PSInput In)
{
    float t = dot(linearWipe.direction, In.pos.xy);
    clip(t - linearWipe.size);
}

//円形ワイプ
void CalcLinearWipeFromRound(PSInput In)
{
    float2 posFromCenter = In.pos.xy - float2(960.0f, 540.0f);
    clip(length(posFromCenter) - linearWipe.size);
}

//縦じまワイプ
void CalcLinearWipeFromVertical(PSInput In)
{
    float t = (int) fmod(In.pos.x, 64.0f);
    clip(t - linearWipe.size);
}

//横じまワイプ
void CalcLinearWipeFromHorizontal(PSInput In)
{
    float t = (int) fmod(In.pos.y, 64.0f);
    clip(t - linearWipe.size);
}

//チェッカーボードワイプ
void CalcLinearWipeFromCheckerBoard(PSInput In)
{
    float t = floor(In.pos.y / 128.0f);
    t = fmod(t, 2.0f);
    t = (int) fmod(In.pos.x + 64.0f * t, 128.0f);
    clip(t - linearWipe.size);
}

//モノクロ
float4 CalcMonochrome(float4 color)
{
    float y = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    
    float3 monochromeColor = float3(y, y, y);
    color.xyz = lerp(color, monochromeColor, drawingRate);
    return color;
}

//セピア
float4 CalcSepia(float4 color)
{
    float y = 0.299f * color.r + 0.587f * color.g + 0.144f * color.b;
    
    float3 sepiaColor;
    sepiaColor.r = y * 0.9f;
    sepiaColor.g = y * 0.7f;
    sepiaColor.b = y * 0.4f;
    
    color.xyz = lerp(color, sepiaColor, drawingRate);
    return color;
}

//ネガポジ反転
float4 CalcNega(float4 color)
{
    float3 negaColor;
    negaColor.x = 1.0f - color.x;
    negaColor.y = 1.0f - color.y;
    negaColor.z = 1.0f - color.z;
    
    color.xyz = lerp(color, negaColor, drawingRate);
    return color;
}

//ハッシュ関数
float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

//3次元ベクトルからシンプレックスノイズを生成する関数
float SimplexNoise(float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);
    
    f = f * f * (3.0f - 2.0f * f);
    float n = p.x + p.y * 57.0f + 113.0f * p.z;
    
    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                     lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
                lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                     lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

//ノイズ
float4 CalcNoise(PSInput In, float4 color)
{
    float t = SimplexNoise(In.pos.xyz);
    
    t = (t - 0.5f) * 2.0f;
    
    float2 uv = In.uv + t * 0.01f;
    color = colorTexture.Sample(Sampler, uv);
    return color;
}
