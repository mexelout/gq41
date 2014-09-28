#pragma once

#include <dsound.h>
#include <string>
#include "singleton.h"


class SoundManager : public Singleton<SoundManager> {
private:
	friend Singleton;
public:
	typedef struct SOUND{
		SOUND();
		~SOUND();
		void play();
		void release();
		DSBUFFERDESC dsbdesc;
		WAVEFORMATEX wf;
		DWORD size;
		LPDIRECTSOUNDBUFFER8 dsb;
	}SOUND;


	SoundManager& init();
	void open(SOUND& s, std::string filename);
	void play();
	void release();

private:
	SoundManager(void);
	~SoundManager(void);

	LPDIRECTSOUND8 ds8;

	SOUND sound;
};
