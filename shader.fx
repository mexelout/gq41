float4x4 g_world_view_projection;
float4 g_light_direction;
float4x4 g_world;
float4 g_ambient;

sampler g_tex_sampler;

void vertexShaderNotLight(in float4 inPosition: POSITION, in float4 inDiff : COLOR, out float4 outPosition: POSITION, out float4 outDiff : COLOR) {
	outPosition = mul(inPosition, g_world_view_projection);
	outDiff = inDiff;
}

void vertexShader3D(in float4 inPosition: POSITION, in float4 inNormal: NORMAL, out float4 outPosition: POSITION, out float4 outDiffuse: COLOR0) {
	outPosition = mul(inPosition, g_world_view_projection);
	inNormal.w = 0;
	float4 normal = mul(inNormal, g_world);
	// îÕàÕ 0Å`1
	outDiffuse = min(max(-dot(g_light_direction, normal) + g_ambient, 0), 1);
	outDiffuse.a = 1.0f;
}

void vertexShaderTexture(in float4 inPosition: POSITION, in float4 inNormal: NORMAL, in float4 inDiff : COLOR, out float4 outPosition: POSITION, out float4 outDiffuse: COLOR0, in float2 inTex : TEXCOORD0, out float2 outTex : TEXCOORD0) {
	outPosition = mul(inPosition, g_world_view_projection);
	inNormal.w = 0;
	float4 normal = mul(inNormal, g_world);
	// îÕàÕ 0Å`1
	outDiffuse = min(max(-dot(g_light_direction, normal) + g_ambient, 0), 1) * inDiff;
	outDiffuse.a = 1.0f;
	outTex = inTex;
}

void pixelShader3D(in float4 inDiff : COLOR, out float4 outDiff : COLOR) {
	outDiff = inDiff;
}

void pixelShaderTexture(in float2 inTex : TEXCOORD0, in float4 inDiff : COLOR, out float4 outDiff : COLOR) {
	outDiff = tex2D(g_tex_sampler, inTex) * inDiff;
};
