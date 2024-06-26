//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "audio.h"

IXAudio2*				Audio::s_pXaudio = NULL;
IXAudio2MasteringVoice*	Audio::s_pMasteringVoice = NULL;

///////////////////////////////////////////////////////////////////////////////
// Audio class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// マスター初期化処理
//-----------------------------------------------------------------------------
void Audio::InitMaster()
{
	// COM初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&s_pXaudio, 0);

	// マスタリングボイス生成
	s_pXaudio->CreateMasteringVoice(&s_pMasteringVoice);
}

//-----------------------------------------------------------------------------
// マスター終了処理
//-----------------------------------------------------------------------------
void Audio::UninitMaster()
{
	s_pMasteringVoice->DestroyVoice();
	s_pXaudio->Release();
	CoUninitialize();
}

//-----------------------------------------------------------------------------
// 読み込み処理
//-----------------------------------------------------------------------------
void Audio::Load(const char *FileName)
{

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		buflen = datachunkinfo.cksize;
		m_pSoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)m_pSoundData, buflen);


		m_nLength = readlen;
		m_nPlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	s_pXaudio->CreateSourceVoice(&m_pSourceVoice, &wfx);
	assert(m_pSourceVoice);
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Audio::Uninit()
{
	m_pSourceVoice->Stop();
	m_pSourceVoice->DestroyVoice();

	delete[] m_pSoundData;
}

//-----------------------------------------------------------------------------
// 再生処理
//-----------------------------------------------------------------------------
void Audio::Play(bool Loop)
{
	m_pSourceVoice->Stop();
	m_pSourceVoice->FlushSourceBuffers();

	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_nLength;
	bufinfo.pAudioData = m_pSoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_nPlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = m_nPlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_pSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	// 再生
	m_pSourceVoice->Start();

}

//-----------------------------------------------------------------------------
// 停止処理
//-----------------------------------------------------------------------------
void Audio::Stop()
{
	m_pSourceVoice->Stop();
}

//-----------------------------------------------------------------------------
// 音量設定処理
//-----------------------------------------------------------------------------
void Audio::SetVolume(float _vol)
{
	m_pSourceVoice->SetVolume(_vol);
}



