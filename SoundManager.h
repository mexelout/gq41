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
		void stop();
		void release();
		DSBUFFERDESC dsbdesc;
		WAVEFORMATEX wf;
		DWORD size;
		LPDIRECTSOUNDBUFFER8 dsb;
	}SOUND;


	static SoundManager& init();
	static void open(SOUND& s, std::string filename);
	static void play();
	static void stop();
	static void release();

private:
	SoundManager(void);
	~SoundManager(void);

	LPDIRECTSOUND8 ds8;

	SOUND sound;
};
