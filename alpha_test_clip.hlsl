sampler g_tex_sampler;


float4 psAlphaTestClip(in float2 tex: TEXCOORD) : COLOR {
	float4 color = tex2D(g_tex_sampler, tex);

	// AlphaTest
	if(color.a <= 0.5f) {
		clip(-1);
	}

	return color;
}