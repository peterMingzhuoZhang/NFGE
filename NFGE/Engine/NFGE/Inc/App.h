#ifndef INCLIDED_NFGE_APP_H
#define INCLIDED_NFGE_APP_H

#include "Timer.h"

namespace NFGE
{
	class AppState;
	class World;
	class CameraEntry;
	struct AppConfig
	{
		AppConfig() = default;
		AppConfig(std::string appName)
			: appName(std::move(appName))
		{}
		AppConfig(std::string appName, std::filesystem::path path)
			: appName(std::move(appName))
			, assetsDirectory(path)
		{}

		std::string appName = "Nicolas Four Game Engine";
		std::filesystem::path assetsDirectory = L"../../Assets";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		bool maximize = false;
		bool imGuiDocking = false;
		bool isEditor = false;
	};

	class App
	{
	public:

		template<class StateType>
		void AddState(std::string name);
		void ChangeState(std::string name);

		void Run(AppConfig appConfig);

		// Time Functions
		float GetTime();
		float GetDeltaTime();

		// Resource Functions
		Graphics::TextureId LoadTexture(const char* fileName);
		void ClearAllTextures();
		bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
		bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);

		// Debug Draw Functions
		void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Graphics::Color& color);
		void DrawLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color);
		void DrawAABB(const Math::AABB& aabb, const Graphics::Color& color);
		void DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Graphics::Color& color);
		void DrawOBB(const Math::OBB& obb, const Graphics::Color& color);
		void DrawSphere(const Math::Vector3& center, float radius, const Graphics::Color& color, uint32_t slices = 8, uint32_t rings = 4);
		void DrawSphere(float x, float y, float z, float radius, const Graphics::Color& color, uint32_t slices = 8, uint32_t rings = 4);
		void DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Graphics::Color& color);
		void DrawScreenLine(float x0, float y0, float x1, float y1, const Graphics::Color& color);
		void DrawScreenRect(const Math::Rect& rect, const Graphics::Color& color);
		void DrawScreenRect(float l, float t, float r, float b, const Graphics::Color& color);
		void DrawScreenCircle(const Math::Circle& circle, const Graphics::Color& color);
		void DrawScreenCircle(const Math::Vector2& center, float r, const Graphics::Color& color);
		void DrawScreenCircle(float x, float y, float r, const Graphics::Color& color);
		void DrawScreenDiamond(const Math::Vector2& center, float size, const Graphics::Color& color);
		void DrawScreenDiamond(float x, float y, float size, const Graphics::Color& color);
		void DrawScreenText(const char* str, float x, float y, float size, const Graphics::Color& color);

		// Sprite Functions
		void DrawSprite(Graphics::TextureId textureId, const Math::Vector2& position);
		void DrawSprite(Graphics::TextureId textureId, const Math::Vector2& position, float rotation, Graphics::Pivot pivot = Graphics::Pivot::Center);
		void DrawSprite(Graphics::TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position);
		//----------------------------------- Mingzhuo zhang Editing --------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------|||||||||||||||||||||||||||----------------------------------------------------------------------
		//------------------------------------vvvvvvvvvvvvvvvvvvvvvvvvvvv----------------------------------------------------------------------
		void DrawSprite(Graphics::TextureId textureId, const Math::Vector2& position, float rotation, float alpha);
		void DrawSprite(Graphics::TextureId textureId, const Math::Vector2& position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale, Math::Rect = Math::Rect{});
		//------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^--------------------------------------------------------------
		//------------------------------------|||||||||||||||||||||||||||||||||||--------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------- End Edite -------------------------------------------------------------------------------------
		uint32_t GetSpriteWidth(Graphics::TextureId textureId);
		uint32_t GetSpriteHeight(Graphics::TextureId textureId);
		void* GetSprite(Graphics::TextureId textureId);

		uint32_t GetScreenWidth();
		uint32_t GetScreenHeight();

		NFGE::Graphics::Camera& GetMainCamera();
		NFGE::Graphics::DirectionalLight& GetMainLight();
		void SoSoCameraControl(float turnSpeed, float moveSpeed, NFGE::Graphics::Camera& camera, float deltaTime);
		void SoSoCameraControl(float turnSpeed, float moveSpeed, CameraEntry& camera, float deltaTime);
		
		void SetWorld(World& world) {mWorld = &world;};
	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		std::map<std::string, std::unique_ptr<AppState>> mAppStates;;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;

		NFGE::Graphics::Camera myCamera;
		NFGE::Graphics::DirectionalLight mLight;
		NFGE::World* mWorld = nullptr;

		NFGE::Timer myTimer;
		Font myFont;
		bool initialized = false;
		std::vector<NFGE::Graphics::SpriteCommand> mySpriteCommands;
		std::vector<NFGE::Graphics::TextCommand> myTextCommands;

		void ExecuteSpriteCommand();
		void ExecuteTextCommand();

	};

	template<class StateType>
	void App::AddState(std::string name)
	{
		mAppStates.emplace(std::move(name), std::make_unique<StateType>());
	}

	
}

#endif // !INCLIDED_NFGE_APP_H
