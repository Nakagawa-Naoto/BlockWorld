//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "manager.h"
#include "renderer.h"
#include "Scene.h"
#include "Time.h"

//-----------------------------------------------------------------------------
//�@�������x�̕`��
//-----------------------------------------------------------------------------
void GuiUpdate(void)
{
	ImGuiIO& m_io = ImGui::GetIO();
	float deltaTime = Time::GetDeltaTime();
	static float timeline[100];
	static float timenow[100];
	ImGui::Begin("System");
	// �b���t���[���ƃt���[�����b�𐔎��ŕ`��
	ImGui::Text("Application %.3f s/frame (%.1f FPS)", deltaTime, 1 / deltaTime);

	// �z���Push����ׂɂ��炷
	for (int i = 0; i < 99; i++)
	{
		timeline[i] = timeline[i + 1];
		timenow[i] = timenow[i + 1];
	}

	// ���݂̃t���[������Push
	timeline[99] = deltaTime;
	timenow[99] = (float)Time::GetExecutionTime() / 1000.0f;

	// �O���t�`��
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
// ����������
//-----------------------------------------------------------------------------
void Manager::Init(Application* ap)
{
	// �^�C�}�[�𑜓x��1�~���b�ɐݒ肷��
	timeBeginPeriod(1);
	
	// �����_���\�̏�����
	Renderer::Init(ap);

	// GUI�̏�����
	GUI::Init(ap->GetWindow(), Renderer::GetDevice(), Renderer::GetDeviceContext());
	
	// �V�[���̐���
	Scene::CreateScene();
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void Manager::Uninit()
{
	// �V�[���̏I������
	Scene::DestoryScene();

	// GUI�̏I������
	GUI::Uninit();

	// �����_���[�I������
	Renderer::Uninit();
	
	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);

}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void Manager::Update(uint64_t d)
{
	GUI::Begin();
	Scene::UpdateScene();
	GuiUpdate();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void Manager::Draw(uint64_t d)
{
	Renderer::Begin();
	Scene::DrawScene();
	GUI::End();
	Renderer::End();
}
