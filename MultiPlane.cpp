#include "MultiPlane.h"
#include "ShaderDevise.h"
#include "Camera.h"

MultiPlane::MultiPlane(void) {
}


MultiPlane::~MultiPlane(void) {
}

MultiPlane* MultiPlane::init() {
	plane1 = (new Plane())->init(ShaderDevise::device());
	plane2 = (new Plane())->init(ShaderDevise::device());
	plane3 = (new Plane())->init(ShaderDevise::device());
	plane4 = (new Plane())->init(ShaderDevise::device());
	plane5 = (new Plane())->init(ShaderDevise::device());

	{
		CUSTOMVERTEX vtxs[][4] =
		{
			{
				CUSTOMVERTEX(D3DXVECTOR3(-4, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
				CUSTOMVERTEX(D3DXVECTOR3(-3, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
				CUSTOMVERTEX(D3DXVECTOR3(-4, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)),
				CUSTOMVERTEX(D3DXVECTOR3(-3, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1))
			},
			{
				CUSTOMVERTEX(D3DXVECTOR3(-3, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
				CUSTOMVERTEX(D3DXVECTOR3(-1, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
				CUSTOMVERTEX(D3DXVECTOR3(-3, 0, -3), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)),
				CUSTOMVERTEX(D3DXVECTOR3(-1, 0, -3), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1))
			},
			{
				CUSTOMVERTEX(D3DXVECTOR3(-1, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
				CUSTOMVERTEX(D3DXVECTOR3( 2, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
				CUSTOMVERTEX(D3DXVECTOR3(-1, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)),
				CUSTOMVERTEX(D3DXVECTOR3( 2, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1))
			},
			{
				CUSTOMVERTEX(D3DXVECTOR3( 1, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
				CUSTOMVERTEX(D3DXVECTOR3( 2, 0,  0), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
				CUSTOMVERTEX(D3DXVECTOR3( 1, 0, -2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)),
				CUSTOMVERTEX(D3DXVECTOR3( 2, 0, -2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1))
			},
			{
				CUSTOMVERTEX(D3DXVECTOR3( 0, 0,  3), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 0)),
				CUSTOMVERTEX(D3DXVECTOR3( 1, 0,  3), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 0)),
				CUSTOMVERTEX(D3DXVECTOR3( 0, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(0, 1)),
				CUSTOMVERTEX(D3DXVECTOR3( 1, 0,  2), D3DXVECTOR3(0, 1, 0), 0xffffffff, D3DXVECTOR2(1, 1))
			}
		};
		
		plane1->setVertices(vtxs[0], 4);
		plane2->setVertices(vtxs[1], 4);
		plane3->setVertices(vtxs[2], 4);
		plane4->setVertices(vtxs[3], 4);
		plane5->setVertices(vtxs[4], 4);
	}
	LPD3DXBUFFER code;
	LPD3DXBUFFER error;

	D3DXCompileShaderFromFile("shader.fx", NULL, NULL,
								"vertexShader3D", "vs_2_0", 0,
								&code, &error, &vs_constant_table);

	if(error) {
		OutputDebugString((LPCSTR)error->GetBufferPointer());
		MessageBox(NULL, "shader error.\ncheck output string message.", "error", MB_OK);
		error->Release();
		delete this;
		return NULL;
	}

	ShaderDevise::device()->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vertex_shader);
	code->Release();

	return this;
}

void MultiPlane::update() {
	static float parcent = 0;
	static int idx = 0;
	D3DXVECTOR3 eyes[] = {
		D3DXVECTOR3(5, 5, 5),
		D3DXVECTOR3(3, 3, 3), 
		D3DXVECTOR3(2, 2, 2),
		D3DXVECTOR3(-2, 3, 3),
		D3DXVECTOR3(-2, 3, -2),
		D3DXVECTOR3(0, 4, -4),
		D3DXVECTOR3(1, 5, -3),
		D3DXVECTOR3(2, 4, 0),
	};

	static int idx_max = sizeof(eyes) / sizeof(D3DXVECTOR3);

	plane1->update();
	plane2->update();
	plane3->update();
	plane4->update();
	plane5->update();

	D3DXVECTOR3 eye = (eyes[(idx+1)%idx_max] - eyes[idx])*parcent + eyes[idx];
	parcent += 0.01f;
	if(parcent >= 1) {
		parcent = 0;
		idx = (idx+1)%idx_max;
	}
	Camera::setEye(eye);


}

void MultiPlane::draw() {

	D3DXVECTOR4 light_dir(0, 0.4f, -1, 0);
	D3DXVec4Normalize(&light_dir, &light_dir);

	D3DXVECTOR4 ambient_color(0.8f, 0.8f, 0.8f, 1);

	// Direct3D‚Ìˆ—
	ShaderDevise::device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	ShaderDevise::device()->BeginScene();


	vs_constant_table->SetVector(ShaderDevise::device(), "g_light_direction", &light_dir);
	vs_constant_table->SetVector(ShaderDevise::device(), "g_ambient", &ambient_color);

	plane1->draw(ShaderDevise::device());
	plane2->draw(ShaderDevise::device());
	plane3->draw(ShaderDevise::device());
	plane4->draw(ShaderDevise::device());
	plane5->draw(ShaderDevise::device());

	ShaderDevise::device()->EndScene();
	ShaderDevise::device()->Present( NULL, NULL, NULL, NULL );
}

void MultiPlane::release() {
	vertex_shader->Release();

	vs_constant_table->Release();
	plane1->release();
	plane2->release();
	plane3->release();
	plane4->release();
	plane5->release();
}

