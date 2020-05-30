#ifndef INCLIDED_NFGE_APPSTATE_H
#define INCLIDED_NFGE_APPSTATE_H

namespace NFGE
{
	class AppState
	{
	public:
		virtual void Initialize() = 0;
		virtual void Terminate() = 0;

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void DebugUI() = 0;

	private:

	};
}




#endif // !INCLIDED_NFGE_APPSTATE_H
