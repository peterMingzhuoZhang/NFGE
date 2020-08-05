#include "GameState.h"

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::vector<const char*> names = {	"С��С��, ������",
										"С��С��, �ɰ�����",
										"С��С��, С������",
										"С��С��, �ޱȼ���",
										"С��С��, ��������",
										"С��С��, ��������",
										"С��С��, �ɰ�����"
										};

	auto nameIndex = NFGE::Math::Random() % names.size();
	NFGEApp::AddState<GameState>("GameState");
	NFGEApp::Run({names[nameIndex], L"../../Assets"});
	return 0;
}
