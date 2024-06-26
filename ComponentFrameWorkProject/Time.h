#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <thread>
#include <iostream>
#include <windows.h>
#include <timeapi.h>

///////////////////////////////////////////////////////////////////////////////
// Time class
///////////////////////////////////////////////////////////////////////////////
class Time // シングルトン
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private:
	//=========================================================================
   // private variables.
   //=========================================================================
	uint64_t m_uMicrosecondsPerFrame ;// micro second
	uint64_t m_uCurrent_time;
	uint64_t m_uLast_time;
	uint64_t m_uDelta_time;
	uint64_t m_uStartTime;

	static Time* s_pTime;
	//=========================================================================
	// private methods.
	//=========================================================================
	Time() = delete;
	Time(uint64_t f) {
		m_uMicrosecondsPerFrame = static_cast<uint64_t>((1000.0f * 1000.0f) / f);
		m_uCurrent_time = 0;
		m_uLast_time = 0;
		m_uDelta_time = 0;
		m_uStartTime = timeGetTime();
	}

	~Time() {}
	
public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */


	//=========================================================================
	// public methods.
	//=========================================================================
	
	static void Init(uint64_t f)
	{
		if(s_pTime == nullptr)s_pTime = new Time(f);
		else 
		{
			s_pTime->m_uMicrosecondsPerFrame = static_cast<uint64_t>((1000.0f * 1000.0f) / f);
			s_pTime->m_uCurrent_time = 0;
			s_pTime->m_uLast_time = 0;
			s_pTime->m_uDelta_time = 0;
		}
		
	}
	static void Uninit() { if (s_pTime != nullptr)delete s_pTime; }
	static uint64_t  CalcDelta() {
		s_pTime->m_uCurrent_time = timeGetTime();// 現在時刻取得
		s_pTime->m_uDelta_time = s_pTime->m_uCurrent_time - s_pTime->m_uLast_time;// 前回実行時からに経過時間を取得
		s_pTime->m_uLast_time = s_pTime->m_uCurrent_time;
		return s_pTime->m_uDelta_time;
	}

	static void Wait() {
		int64_t sleep_time = s_pTime->m_uMicrosecondsPerFrame - s_pTime->m_uDelta_time;
		if (sleep_time > 0) {
			float tt = sleep_time / 1000.0f;
			//Logger::SystemLog("sleep time(ms):" + std::to_string(tt));
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//指定した相対時間だけ現スレッドをブロックする (function template)
		}
	}
	static float GetDeltaTime() { return s_pTime->m_uDelta_time < 10000?s_pTime->m_uDelta_time * (0.001f): (0.001f) ; }
	static float GetStartTime() { return s_pTime->m_uStartTime; }
	static float GetTime() { return timeGetTime(); }
	static float GetExecutionTime() { return timeGetTime() - s_pTime->m_uStartTime; }

	
};
