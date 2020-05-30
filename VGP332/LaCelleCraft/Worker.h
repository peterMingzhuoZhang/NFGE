#ifndef LACELLECRAFT_GAMEOBJECT_WORKER_HEADER
#define LACELLECRAFT_GAMEOBJECT_WORKER_HEADER

#include "Unit.h"

struct Structure;
struct LCTexture;
namespace LCUnit
{

	enum WorkerState
	{
		Idle = 0,
		InTravel,
		Attacking,
		GetAttack,
		AlmostDestoryed,
		Destoryed,
		TotalStateCount
	};


	struct Worker : Unit
	{

		static int mHealthBarSliceCount;
		static LCTexture mMoneyTexture;

		std::unique_ptr<AI::StateMachine<Worker>> stateMachine;

		Structure* mTowardStructure = nullptr;
		Structure* mLastWorkedCity = nullptr;
		Unit* mTowardUnit = nullptr;


		bool mIsGarthing = false;
		bool mIsFullCarry = false;
		bool mIsWorking = false;
		float mHourlyRate = 15.0f;
		int mCurrentCarryMoney = 0;
		float mTotalWorkTime = 8.0f;
		float mWorkTimeCounter = mTotalWorkTime;

		Worker();
		void Load(TextureGridMap& worldMap, const Vector2& initPos, int commanderIndex, const Vector2& size, float maxHealth);
		virtual void Update(float deltaTime);
		virtual void ControlUpdate(const Vector2& currentMousePos, float deltaTime) override;
		virtual void Render(const Camera2D& camera) override;
		virtual void DebugUIUpdate(float deltaTime, const Camera2D& camera) override;
		virtual void Unload();

		void Interact() = 0;

		virtual TextureId GetDefaultTexture(int index = 0) = 0;

		bool Work(float deltaTime);

		void GoInCity();
		void GetOutCity();
		void HandInSalary();

		void Attack( Unit& unit, float deltaTime) = 0;
		void Attack( Building& strcuture, float deltaTime) = 0;
		void TakeDamage(LC::Projectile& bullet) = 0;
		void Die() = 0;
	private:
	};

}
#endif // !LACELLECRAFT_GAMEOBJECT_WORKER_HEADER

