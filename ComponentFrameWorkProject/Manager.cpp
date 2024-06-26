//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "manager.h"
#include "renderer.h"
#include "Scene.h"
#include "Time.h"

//-----------------------------------------------------------------------------
//　処理速度の描画
//-----------------------------------------------------------------------------
void GuiUpdate(void)
{
	ImGuiIO& m_io = ImGui::GetIO();
	float deltaTime = Time::GetDeltaTime();
	static float timeline[100];
	static float timenow[100];
	ImGui::Begin("System");
	// 秒毎フレームとフレーム毎秒を数字で描画
	ImGui::Text("Application %.3f s/frame (%.1f FPS)", deltaTime, 1 / deltaTime);

	// 配列をPushする為にずらす
	for (int i = 0; i < 99; i++)
	{
		timeline[i] = timeline[i + 1];
		timenow[i] = timenow[i + 1];
	}

	// 現在のフレーム情報をPush
	timeline[99] = deltaTime;
	timenow[99] = (float)Time::GetExecutionTime() / 1000.0f;

	// グラフ描画
	int impFlags = 0;
	int impAxisFlag = ImPlotAxisFlags_AutoFit;
	if (ImPlot::BeginPlot("Title", "ExecuteTime(s)", "FrameTime(s)", ImVec2(250, 0), impFlags, impAxisFlag, impAxisFlag))
	{
		int flags = ImPlotLineFlags_Shaded;
		ImPlot::PlotLine("CPU", timenow, timeline, 100, flags);
		ImPlot::EndPlot();
	}

	ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////
// Manager class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Manager::Init(Application* ap)
{
	// タイマー解像度を1ミリ秒に設定する
	timeBeginPeriod(1);
	
	// レンダラ―の初期化
	Renderer::Init(ap);

	// GUIの初期化
	GUI::Init(ap->GetWindow(), Renderer::GetDevice(), Renderer::GetDeviceContext());
	
	// シーンの生成
	Scene::CreateScene();
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Manager::Uninit()
{
	// シーンの終了処理
	Scene::DestoryScene();

	// GUIの終了処理
	GUI::Uninit();

	// レンダラー終了処理
	Renderer::Uninit();
	
	// タイマ解像度を元に戻す
	timeEndPeriod(1);

}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Manager::Update(uint64_t d)
{
	GUI::Begin();
	Scene::UpdateScene();
	GuiUpdate();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Manager::Draw(uint64_t d)
{
	Renderer::Begin();
	Scene::DrawScene();
	GUI::End();
	Renderer::End();
}
