#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <xaudio2.h>
#include "gameObject.h"

///////////////////////////////////////////////////////////////////////////////
// Audio class
///////////////////////////////////////////////////////////////////////////////
class Audio : public Component
{
private:
	static IXAudio2*				s_pXaudio;
	static IXAudio2MasteringVoice*	s_pMasteringVoice;

	IXAudio2SourceVoice*	m_pSourceVoice{};
	BYTE*					m_pSoundData{};

	int						m_nLength{};
	int						m_nPlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void Stop();

	void SetVolume(float _vol);

};

