
#include "SoundManager.h"
#include "WindowManager.h"
#include "Common.h"
#include <tchar.h>
#include <mmsystem.h>

// Wave�t�@�C���I�[�v���֐�
bool openWave(const char *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize ) {
	if ( filepath == 0 )
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;
	char* _filepath_ = (char*)filepath;

	// Wave�t�@�C���I�[�v��
	memset( &mmioInfo, 0, sizeof(MMIOINFO) );
	hMmio = mmioOpen( _filepath_, &mmioInfo, MMIO_READ );
	if( !hMmio )
		return false; // �t�@�C���I�[�v�����s

	// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend( hMmio, &riffChunk, NULL, MMIO_FINDRIFF );
	if( mmRes != MMSYSERR_NOERROR ) {
		mmioClose( hMmio, 0 );
		return false;
	}

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend( hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK );
	if( mmRes != MMSYSERR_NOERROR ) {
		mmioClose( hMmio, 0 );
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead( hMmio, (HPSTR)&waveFormatEx, fmsize );
	if( size != fmsize ) {
		mmioClose( hMmio, 0 );
		return false;
	}

	mmioAscend( hMmio, &formatChunk, 0 );

	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend( hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK );
	if( mmRes != MMSYSERR_NOERROR ) {
		mmioClose( hMmio, 0 );
		return false;
	}
	*ppData = new char[ dataChunk.cksize ];
	size = mmioRead( hMmio, (HPSTR)*ppData, dataChunk.cksize );
	if(size != dataChunk.cksize ) {
		delete[] *ppData;
		return false;
	}
	dataSize = size;

	// �n���h���N���[�Y
	mmioClose( hMmio, 0 );

	return true;
}


SoundManager::SoundManager(void) {
}

SoundManager::~SoundManager(void) {
}

SoundManager::SOUND::SOUND() {
	ZeroMemory(this, sizeof(SoundManager::SOUND));
}

SoundManager::SOUND::~SOUND() {
}

void SoundManager::SOUND::play() {
	if(dsb) dsb->Play(0, 0, DSBPLAY_LOOPING);
}

void SoundManager::SOUND::release() {
	SAFE_RELEASE(dsb);
	ZeroMemory(this, sizeof(SoundManager::SOUND));
}

SoundManager& SoundManager::init() {
	HWND hWnd = WindowManager::inst().getWnd();

	// �T�E���h�f�o�C�X�쐬
	DirectSoundCreate8(NULL, &ds8, NULL);
	ds8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	open(sound, "Sound/rock42.wav");

	return *this;
}

void SoundManager::open(SOUND& s, std::string filename) {
	// Wave�t�@�C���I�[�v��
	char *data = 0;
	if(!openWave(filename.c_str(), s.wf, &data, s.size)) {
		return;
	}

	s.dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	s.dsbdesc.dwFlags = 0;
	s.dsbdesc.dwBufferBytes = s.size;
	s.dsbdesc.dwReserved = 0;
	s.dsbdesc.lpwfxFormat = &s.wf;
	s.dsbdesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer *ptmpBuf = 0;
	ds8->CreateSoundBuffer(&s.dsbdesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&s.dsb);
	ptmpBuf->Release();
	if(s.dsb == 0) {
		return;
	}

	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if(DS_OK == s.dsb->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, data, dwLength);
		s.dsb->Unlock( lpvWrite, dwLength, NULL, 0);
	}
	delete[] data; // �����͂�������Ȃ�

}

void SoundManager::play() {
	sound.play();
}


void SoundManager::release() {
	sound.release();
	SAFE_RELEASE(ds8);
}
