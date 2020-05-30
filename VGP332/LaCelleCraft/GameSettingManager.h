#ifndef LACELLECRAFT_GAMESETTINGMANAGER
#define LACELLECRAFT_GAMESETTINGMANAGER

#include "EngineWrapper.h"

using namespace EngineWrapper;

class GameSettingManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static GameSettingManager* Get();

public:
	GameSettingManager() = default;
	~GameSettingManager();

	GameSettingManager(const GameSettingManager& other) = delete;
	GameSettingManager& operator=(const GameSettingManager& other) = delete;

	void Initlialize() // TODO--- load from file in future
	{
		mAnimationFPS = 12;
	}

	void Terminate()
	{

	}

	int GetAnimationFPS() const { return mAnimationFPS; }
private:
	int mAnimationFPS;

};

#endif // !LACELLECRAFT_GAMESETTINGMANAGER
