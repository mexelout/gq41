#include "Common.h"
#include "ShaderDevise.h"
#include "Input.h"
#include "Camera.h"
#include "TravelingSalesmanProblemScene.h"
#include "StaticPlane.h"
#include "Vertex.h"
#include "Arrow.h"
#include <time.h>

// �萔��`
#define ITEM_NUM 6 // �A�C�e���̐�
#define GENE_NUM 10 // ��`�q�̐�
#define GENE_LENGTH (ITEM_NUM) // ��`�q�̒���
#define CROSS_RATE 0.8 // �����̕p�x
#define MUTE_RATE 0.1 // �ˑR�ψق̕p�x
#define GENERATON_NUM 50 // ������̐�

const std::string g_weight[] = {"A", "B", "C", "D", "E", "F"}; // �ꏊ��
const D3DXVECTOR3 g_value [] = {
	D3DXVECTOR3(0, 1, 0),
	D3DXVECTOR3(5, 1, 4),
	D3DXVECTOR3(0, 1, 4),
	D3DXVECTOR3(-5, 1, 3),
	D3DXVECTOR3(-5, 1, -3),
	D3DXVECTOR3(0, 1, -4),
}; // �ʒu

// �v���g�^�C�v�錾
void Init();
void Select();
void Cross();
void Mute();
void CalcFit();
int CalcFitSub(int gene_no);

// �O���[�o���ϐ�
int g_generation; // ���㐔
int g_gene[GENE_NUM][GENE_LENGTH]; // ��`�q
int g_fit[GENE_NUM]; // �K���x
int g_maxFit; // �ō��K���x
int g_maxGeneration; // �ō�����
int g_maxIndex; // �ō���`�q�ԍ�
int g_maxGene[GENE_LENGTH]; // �ō���`�q

void Init()
{
	int i, j;

	// �����̏�����
	srand((unsigned)time(NULL));

	// ���㐔�̏�����
	g_generation = 0;

	// �ō��K���x�̏�����
	g_maxFit = 0;

	// �ō�����̏�����
	g_maxGeneration = 0;

	// �ō���`�q�ԍ��̏�����
	g_maxIndex = 0;

	// ��`�q�̏�����
	for(i = 0; i < GENE_NUM; i++) {
		for(j = 0; j < GENE_LENGTH; j++) {
			int tmp = rand() % GENE_LENGTH;
			int k = 0;
			while(k < GENE_LENGTH) {
				if(g_gene[i][k] == tmp) {
					k = -1;
					tmp = rand() % GENE_LENGTH;
				}
				k++;
			}
			g_gene[i][j] = tmp;
		}
	}
}

// �I��
void Select()
{
	int i, j;
	for(i = 0; i < GENE_NUM; i+=2) {
		if(g_fit[i] < g_fit[i+1]) {
			for(j = 0; j < GENE_LENGTH; j++) {
				g_gene[i][j] = g_gene[i+1][j];
			}
		} else if(g_fit[i] > g_fit[i+1]) {
			for(j = 0; j < GENE_LENGTH; j++) {
				g_gene[i+1][j] = g_gene[i][j];
			}
		}
	}
}

// ����
void Cross()
{
	int start = rand() % GENE_LENGTH, end = start + rand() % GENE_LENGTH;
	int *genetic_a = g_gene[rand() % GENE_NUM], *genetic_b = g_gene[rand() % GENE_NUM];
	if(end > GENE_LENGTH) end = GENE_LENGTH;
	for(int i = start; i < end; i++) {
		int tmp = genetic_a[i];
		genetic_a[i] = genetic_b[i];
		genetic_b[i] = tmp;
	}
}

// �ˑR�ψ�
void Mute()
{
	int i, j;

	double r;
	int pos;

	for(i = 0; i < GENE_NUM; i++) {
		for(j = 0; j < GENE_LENGTH; j++) {
			// �ˑR�ψق��N���邩�𗐐��Ō���
			r = (rand() % 10001)/(10000.0);
			if( MUTE_RATE > r ){
				pos = rand() % GENE_LENGTH;
				// ���l�𔽓]�i�ˑR�ψفj
				g_gene[i][pos] ^= 1;
			}
		}
	}
}

// ��`�q�̓K���x���v�Z
void CalcFit()
{
	int i;

	int fit; // �K���x
	int index; // ��`�q�ԍ�

	fit = 0;
	index = 0;

	for(i = 0; i < GENE_NUM; i++) {
		// ��`�q�̓K���x���v�Z
		g_fit[i] = CalcFitSub(i);

		// �K���x�̍ő�l�����߂�
		if(fit < g_fit[i]) {
			fit = g_fit[i];
			index = i;
		}
	}

	// �\��
	{
		int i, j;

		Common::debug("\n******** ���� = %d ********\n", g_generation);

		for(i = 0; i < GENE_NUM; i++) {
			Common::debug("��`�q�ԍ�[%2d] = ", i);

			for(j = 0; j < GENE_LENGTH; j++) {
				Common::debug("%2d", g_gene[i][j]);
			}

			Common::debug(" > �K���x = %d\n", g_fit[i]);
		}
	}

	// �K���x���ō��K���x�����傫���ꍇ
	if(fit < g_maxFit) {
		// �ō���`�q����ۑ�
		g_maxFit = fit;
		g_maxGeneration = g_generation;
		g_maxIndex = index;

		for(i = 0; i < GENE_LENGTH; i++) {
			g_maxGene[i] = g_gene[g_maxIndex][i];
		}

		// �����o��
		Common::debug("!!!! ���ǂ���`�q�𔭌� !!!!\n");
		Common::debug("��`�q[%2d] = ", g_maxIndex);

		for(i = 0; i < GENE_LENGTH; i++) {
			Common::debug("%2d", g_maxGene[i]);
		}

		Common::debug("\n");
		Common::debug("�K���x = %d\n", g_maxFit);
	}
}

// ��`�q�̓K���x���v�Z�i�T�u�j
int CalcFitSub(int gene_no)
{
	int i;

	float value; // ���l

	value = 0;

	// ���l���擾
	for(i = 1; i < GENE_LENGTH; i++) {
		// ���l�𑝂₷
		value += D3DXVec3Length(&(g_value[i] - g_value[i-1]));
	}

	return value;
}



TravelingSalesmanProblemScene::TravelingSalesmanProblemScene() {
	p = NULL;
	a = NULL;
}
TravelingSalesmanProblemScene::~TravelingSalesmanProblemScene() {
}
TravelingSalesmanProblemScene* TravelingSalesmanProblemScene::init() {
	auto device = ShaderDevise::device();

	p = (new StaticPlane)->init();
	LPDIRECT3DVERTEXBUFFER9 vtx = p->vtx();
	CUSTOMVERTEX* data = (CUSTOMVERTEX*)Common::getVtxDataWithLock(vtx);
	for(int i = 0; i < 4; i++) data[i].pos *= 10;
	vtx->Unlock();

	for each(D3DXVECTOR3 p in g_value) {
		StaticPlane* _sp = (new StaticPlane)->init();
		D3DXMATRIX world(_sp->world());
		world._41 = p.x;
		world._42 = p.y;
		world._43 = p.z;
		_sp->world(world);
		vtx = _sp->vtx();
		CUSTOMVERTEX* data = (CUSTOMVERTEX*)Common::getVtxDataWithLock(vtx);
		for(int i = 0; i < 4; i++) data[i].color = 0xff00ff00;
		vtx->Unlock();

		points.push_back(_sp);
	}

	camera_rot.x = camera_rot.y = (float)M_PI_2 / 2;
	camera_len = 5;
	a = (new Arrow)->init();

	D3DXVECTOR3 start(points[0]->world()._41, points[0]->world()._42, points[0]->world()._43);
	D3DXVECTOR3 end(points[1]->world()._41, points[1]->world()._42, points[1]->world()._43);
	a->start(start).end(end);

	return this;
}
void TravelingSalesmanProblemScene::update() {
	if(InputMouse::btn(InputMouse::Right, Input::Press)) {
		camera_rot.y += InputMouse::move().x/100;
		camera_rot.x += InputMouse::move().y/100;
	}
	float len = cosf(camera_rot.x);
	camera_len -= InputMouse::wheel() / 100;
	camera_len = (camera_len < 1) ? 1 : camera_len;
	if(camera_rot.x > (M_PI_2-0.1f)) camera_rot.x = (float)(M_PI_2-0.1f);
	else if(camera_rot.x < (-M_PI_2+0.1f)) camera_rot.x = (float)(-M_PI_2+0.1f);
	Camera::setAt(D3DXVECTOR3(0, 0, 0));
	Camera::setEye(D3DXVECTOR3(cosf(camera_rot.y)*len, sinf(camera_rot.x), sinf(camera_rot.y)*len)*camera_len);

	p->update();
	a->update();

}
void TravelingSalesmanProblemScene::draw() {
	auto device = ShaderDevise::device();
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff191970, 1.0f, 0);
	device->BeginScene();
	D3DXMATRIX world(Common::identity), view(Camera::view()), proj(Camera::projection());
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	p->draw();
	a->draw();
	for each(StaticPlane* _sp in points) _sp->draw();

	device->EndScene();
}
void TravelingSalesmanProblemScene::release() {
	for each(StaticPlane* _sp in points) SAFE_RELEASE_DELETE(_sp);
	SAFE_RELEASE_DELETE(p);
	SAFE_RELEASE_DELETE(a);
}
// end file
