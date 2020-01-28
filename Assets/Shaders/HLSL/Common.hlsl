/*
	Common.hlsl
	Shaders to render UI, blit textures and draw simple meshes.
*/

#include "Utils.hlsl"
#include "Declarations.hlsl"

//////////////////////////////////////
// Resources
//////////////////////////////////////

Texture2D MainTex : register(t0);
SamplerState LinearWrapSampler : register(s0);

//////////////////////////////////////
// Vertex inputs and outputs definitions
//////////////////////////////////////

struct FullscreenVSIn
{
	float3 Position : POSITION;
};

struct FullscrenVSOut
{
	float4 ClipPos  : SV_Position;
	float2 TexCoord : TEXCOORD;
};

struct UIVSIn
{
	float2 Position 	: POSITION;
	float2 TexCoord 	: TEXCOORD;
	float4 VertexColor 	: COLOR;
};

struct UIVSOut
{
	float4 ClipPos 		: SV_Position;
	float4 VertexColor 	: COLOR;
	float2 TexCoord    	: TEXCOORD;
};

struct SimpleVSIn
{
	float3 Position : POSITION;
	float3 Normal 	: NORMAL;
	float3 Tangent 	: TANGENT;
	float2 Texcoord : TEXCOORD;
};

struct SimpleVSOut
{
	float4 ClipPos  : SV_Position;
	float4 WPos		: WPOS;
	float3 PNormal  : NORMAL;
	//float2 PTexcoord: TEXCOORD;
	//float3x3 TBN	: TBNMATRIX;
};

//////////////////////////////////////
// Blit texture fullscreen
//////////////////////////////////////
FullscrenVSOut VSFullScreen(FullscreenVSIn i)
{
	FullscrenVSOut o;
	o.ClipPos = float4(i.Position.xy,1.0,1.0);
	o.TexCoord = 0.5 * (i.Position.xy + 1.0);
	o.TexCoord.y = 1.0 - o.TexCoord.y;
	return o;
}

float4 PSFullScreen(FullscrenVSOut i): SV_Target0
{
	return MainTex.Sample(LinearWrapSampler, i.TexCoord);
}

//////////////////////////////////////
// Blit texture and apply gamma correction + tone mapping
//////////////////////////////////////
float4 PSToneGamma(FullscrenVSOut i): SV_Target0
{
	float4 base = MainTex.Sample(LinearWrapSampler, i.TexCoord);
	base *= 8.0;
	base.xyz = Uncharted2Tonemap(base.xyz);
	base.w = 1.0;
	base = ToSRGB(base,2.2);
	return base;
}

//////////////////////////////////////
// UI (ImGui) shaders
//////////////////////////////////////
UIVSOut VSUI(UIVSIn i)
{
	UIVSOut o;
	o.ClipPos = mul(ProjectionUI,float4(i.Position,0.0,1.0));
	o.VertexColor = i.VertexColor;
	o.TexCoord = i.TexCoord;
	return o;
}

float4 PSUI(UIVSOut i): SV_Target0
{
	return MainTex.Sample(LinearWrapSampler,(i.TexCoord * 1.0)) * i.VertexColor;
}

//////////////////////////////////////
// Simple shaders
//////////////////////////////////////
SimpleVSOut VSSimple(SimpleVSIn i)
{
	SimpleVSOut o;
	o.WPos = mul(World, float4(i.Position,1.0));
	o.ClipPos = mul(InvViewProj, o.WPos);
	o.PNormal = mul((float3x3)World,i.Normal);
	//o.PTexcoord = i.Texcoord;

	//float3 T 	= normalize(mul(Model,float4(i.Tangent,0.0f))).xyz;
	//float3 N 	= normalize(mul(Model,float4(i.Normal,0.0f))).xyz;
	//T 			= normalize(T - dot(T, N) * N);
	//float3 B 	= cross(T,N);
	//o.TBN 		= float3x3(T,B,N);
	
	return o;
}

float4 PSSimple(SimpleVSOut i): SV_Target0
{	
	/*
	float4 c = AlbedoTexture.Sample(LinearWrapSampler,i.PTexcoord);
	float3 n = BumpTexture.Sample(LinearWrapSampler,i.PTexcoord).xyz;;
	n = normalize(n * 2.0f - 1.0f);
	n = normalize(mul(i.TBN, n));

	float3 tl = normalize(float3(0.0f,15.0f,8.0f) - i.WPos.xyz);
	float ndl = max(dot(n,tl),0.0f);

	return (c * ndl);
	*/

	float lightDir = normalize(float3(1.9,0.0,0.0));
	float ndl = max(dot(normalize(i.PNormal), lightDir),0.0);
	float3 albedo = float3(0.7,0.6,0.6);
	return float4(albedo * ndl, 1.0);
}