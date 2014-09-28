#include <array>
#include "MeshField.h"
#include "Common.h"
#include "ShaderDevise.h"
#include "Vertex.h"
#include "Camera.h"

MeshField::MeshField(void) {
	vtx = NULL;
	idx = NULL;
	texture = NULL;

	// 初期設定値
	horizontal = 64;
	vertical = 64;
	cell_size = 2;
	color = 0xffffffff;
}

MeshField::~MeshField(void) {
}

MeshField* MeshField::init() {
	if(vtx || idx) return this;
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();

	int vtx_num = horizontal * vertical;
	int memory_size = sizeof(CUSTOMVERTEX)*vtx_num;
	int horizontal_cell_num = horizontal - 1, vertical_cell_num = vertical - 1;
	int idx_num = (vertical_cell_num * 2 * horizontal) + ((vertical_cell_num-1) * 2);
	D3DXVECTOR2 tex_offset(1.0f / horizontal_cell_num, 1.0f / vertical_cell_num);
	primitive_num = horizontal_cell_num * vertical_cell_num * 2 + (vertical_cell_num-1) * 4;

	device->CreateVertexBuffer(memory_size,
								D3DUSAGE_WRITEONLY,
								D3DFVF_CUSTOMVERTEX,
								D3DPOOL_MANAGED,
								&vtx,
								NULL);

	std::vector<float> vertices;
	for(int i = 0; i < vertical; i++) {
		for(int j = 0; j < horizontal; j++) {
			vertices.push_back(0);
		}
	}
	setMeshVertex(vertices);

	calcNor();

	device->CreateIndexBuffer(sizeof(WORD) * idx_num,
								0,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&idx,
								NULL);

	WORD* idx_data = NULL;
	idx->Lock(0, sizeof(WORD)*idx_num, (void**)&idx_data, 0);

	int cnt = 0;
	for(int i = 0; i < vertical_cell_num; i++) {
		for(int j = 0; j < horizontal; j++) {
			WORD id = (WORD)(i * horizontal + horizontal + j);
			idx_data[cnt] = id;
			cnt++;
			if(i > 0 && j == 0) {
				idx_data[cnt] = id;
				cnt++;
			}
			id -= horizontal;
			idx_data[cnt] = id;
			cnt++;
			if(j == horizontal_cell_num && i != (vertical_cell_num - 1)) {
				idx_data[cnt] = id;
				cnt++;
			}
		}
	}
	idx->Unlock();

	return this;
}
void MeshField::update() {
}
void MeshField::draw() {
	LPDIRECT3DDEVICE9 device = ShaderDevise::device();
	device->SetFVF(D3DFVF_CUSTOMVERTEX);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	device->SetTransform(D3DTS_WORLD, &world);

	device->SetStreamSource(0, vtx, 0, sizeof(CUSTOMVERTEX));
	device->SetIndices(idx);
	device->SetTexture(0, texture);
	int horizontal_cell_num = horizontal - 1, vertical_cell_num = vertical - 1;
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, horizontal * vertical, 0, primitive_num);

	// wire frame disp
	if(0) {
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		device->SetTexture(0, 0);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, horizontal * vertical, 0, primitive_num);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	device->SetStreamSource(0, NULL, 0, 0);
	device->SetIndices(NULL);
}
void MeshField::release() {
	SAFE_RELEASE(vtx);
	SAFE_RELEASE(idx);
	SAFE_RELEASE(texture);
}
void MeshField::calcNor() {
	int horizontal_cell_num = horizontal - 1, vertical_cell_num = vertical - 1;
	nors.resize(horizontal_cell_num * vertical_cell_num * 2);
	CUSTOMVERTEX* v_data = NULL;
	vtx->Lock(0, 0, (void**)&v_data, 0);
	vtx->Unlock();

	for(int i = 0; i < vertical_cell_num; i++) {
		for(int j = 0; j < horizontal_cell_num; j++) {
			D3DXVECTOR3 left, right, up;
			int id = 0;
			id = i * horizontal + horizontal + j;
			left = v_data[id - horizontal].pos - v_data[id].pos;
			right = v_data[id + 1].pos - v_data[id].pos;
			D3DXVec3Cross(&up, &left, &right);
			D3DXVec3Normalize(&up, &up);
			nors[i*horizontal_cell_num*2+j*2] = up;

			id = i * horizontal + j + 1;
			left = v_data[id + horizontal].pos - v_data[id].pos;
			right = v_data[id - 1].pos - v_data[id].pos;
			D3DXVec3Cross(&up, &left, &right);
			D3DXVec3Normalize(&up, &up);
			nors[i*horizontal_cell_num*2+j*2+1] = up;
		}
	}

	vtx->Lock(0, 0, (void**)&v_data, 0);
	for(int i = 0; i < vertical; i++) {
		for(int j = 0; j < horizontal; j++) {
			if(i == 0) {
				// 左奥
				if(j == 0) {
					v_data[j].nor = (nors[0] + nors[1])/2;
				}
				// 中奥
				else if(0 < j && j < horizontal_cell_num) {
					int id = j*2;
					v_data[j].nor = (nors[id] + nors[id-1] + nors[id+1])/3;
				}
				// 右奥
				else if(j == horizontal_cell_num) {
					v_data[j].nor = nors[j*2-1];
				}
			} else if(0 < i && i < vertical_cell_num) {
				// 左中
				if(j == 0) {
					int id = i*horizontal_cell_num*2;
					v_data[i*horizontal+j].nor = (nors[id-horizontal_cell_num*2] + nors[id] + nors[id+1])/3;
				}
				// 中中
				else if(0 < j && j < horizontal_cell_num) {
					int id1 = (i-1)*horizontal_cell_num*2+j*2-1, id2 = i*horizontal_cell_num*2+j*2;
					v_data[i*horizontal+j].nor = (nors[id1] + nors[id1-1] + nors[id1+1] + nors[id2] + nors[id2+1] + nors[id2-1])/6;
				}
				// 右中
				else if(j == horizontal_cell_num) {
					int id = i * horizontal_cell_num * 2 - 1;
					v_data[i*horizontal+j].nor = (nors[id] + nors[id-1] + nors[id+horizontal_cell_num*2])/3;
				}
			} else {
				// 左手前
				if(j == 0) {
					v_data[i*horizontal+j].nor = (nors[(i-1)*horizontal_cell_num*2]);
				}
				// 中手前
				else if(0 < j && j < horizontal_cell_num) {
					int id = (i-1)*horizontal_cell_num*2+j*2-1;
					v_data[i*horizontal+j].nor = (nors[id] + nors[id-1] + nors[id+1])/3;
				}
				// 右手前
				else if(j == horizontal_cell_num) {
					int id = (i-1)*horizontal_cell_num*2+(j-1)*2;
					v_data[i*horizontal+j].nor = (nors[id] + nors[id+1])/2;
				}
			}
		}
	}
	vtx->Unlock();
}

float MeshField::getHeight(const D3DXVECTOR3* pos) {
	int x = (int)(pos->x + (cell_size * horizontal/2));
	x /= (int)cell_size;
	x -= 1;
	int z = (int)(pos->z - (cell_size * vertical/2));
	z /= (int)cell_size;
	z += 1;
	z *= -1;

	if(0 <= x && x < horizontal-1 && 0 <= z && z < vertical-1) {
		int id = z * horizontal + x;
		CUSTOMVERTEX* v_data = NULL;
		vtx->Lock(0, 0, (void**)&v_data, 0);
		vtx->Unlock();

		std::array<CUSTOMVERTEX, 4> v;
		v[0] = v_data[id+horizontal];
		v[1] = v_data[id];
		v[2] = v_data[id+horizontal+1];
		v[3] = v_data[id+1];
		std::array<D3DXVECTOR3, 4> vecs;
		for(int i = 0; i < 4; i++) vecs[i] = *pos - v[i].pos;
		D3DXVECTOR3 nor = *D3DXVec3Cross(&D3DXVECTOR3(), &(v[1].pos - v[0].pos), &(v[2].pos - v[0].pos));
		float y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[2].pos - v[0].pos), &vecs[2])->y;
		float y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[1].pos - v[2].pos), &vecs[1])->y;
		float y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[0].pos - v[1].pos), &vecs[0])->y;
		if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
			return v[0].pos.y - ((nor.x * (pos->x - v[0].pos.x) + nor.z * (pos->z - v[0].pos.z)) / nor.y);
		} else {
			nor = *D3DXVec3Cross(&D3DXVECTOR3(), &(v[2].pos - v[3].pos), &(v[1].pos - v[3].pos));
			y1 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[1].pos - v[3].pos), &vecs[1])->y;
			y2 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[2].pos - v[1].pos), &vecs[2])->y;
			y3 = D3DXVec3Cross(&D3DXVECTOR3(), &(v[3].pos - v[2].pos), &vecs[3])->y;
			if(y1 <= 0 && y2 <= 0 && y3 <= 0) {
				return v[3].pos.y - ((nor.x * (pos->x - v[3].pos.x) + nor.z * (pos->z - v[3].pos.z)) / nor.y);
			}
		}
	}

	return 0;
}

// 頂点メッシュの設定
void MeshField::setMeshVertex(std::vector<float> vertices, D3DXVECTOR2 uv_offset) {
	CUSTOMVERTEX* vtx_data = NULL;
	vtx->Lock(0, 0, (void**)&vtx_data, 0);
	int size = vertices.size();
	int cnt = 0;
	int horizontal_cell_num = horizontal - 1, vertical_cell_num = vertical - 1;

	for(int i = 0; i < vertical; i++) {
		for(int j = 0; j < horizontal; j++) {
			D3DXVECTOR3 pos = D3DXVECTOR3(j * cell_size - cell_size * (horizontal_cell_num/2), vertices[i*horizontal+j], -i * cell_size + cell_size * (vertical_cell_num/2));
			vtx_data[i*horizontal+j] = CUSTOMVERTEX(pos.x, pos.y, pos.z, 0, 1, 0, color, (float)j+uv_offset.x, (float)i+uv_offset.y);
			cnt++;
			if(size < cnt) {
				break;
			}
		}
		if(size < cnt) {
			break;
		}
	}
	vtx->Unlock();
}

// 頂点メッシュの高さの取得
std::vector<float> MeshField::getMeshVertex() {
	CUSTOMVERTEX* vtx_data = NULL;
	vtx->Lock(0, 0, (void**)&vtx_data, 0);
	vtx->Unlock();
	std::vector<float> v;

	for(int i = 0; i < vertical; i++) {
		for(int j = 0; j < horizontal; j++) {
			v.push_back(vtx_data[i*horizontal+j].pos.y);
		}
	}
	return v;
}

