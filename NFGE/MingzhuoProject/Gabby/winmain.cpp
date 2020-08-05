#include "GameState.h"

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::vector<const char*> names = {	"小任小任, 龙马精神",
										"小任小任, 可爱迷人",
										"小任小任, 小鸟依人",
										"小任小任, 无比坚韧",
										"小任小任, 东北乱炖",
										"小任小任, 教你做人",
										"小任小任, 可爱绝伦"
										};

	auto nameIndex = NFGE::Math::Random() % names.size();
	NFGEApp::AddState<GameState>("GameState");
	NFGEApp::Run({names[nameIndex], L"../../Assets"});
	return 0;
}
