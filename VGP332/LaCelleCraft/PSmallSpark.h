#ifndef NFGE_LACELLECRAFT_PSMALLSPARK_H
#define NFGE_LACELLECRAFT_PSMALLSPARK_H

#include "Projectile.h"

namespace LC
{
	struct PSmallSpark : Projectile
	{
		RTTI(PSmallSpark)
		static TextureId mDefaultTexture[directionCount];

		PSmallSpark();

		virtual void Fire(const Vector2& initPos, const Vector2& size, int commanderIndex, Building& structure) override;
		virtual void Fire(const Vector2& initPos, const Vector2& size, int commanderIndex, Unit& structure) override;
		virtual void Render(const Camera2D& camera) override;
	};
}
#endif // !NFGE_LACELLECRAFT_PSMALLSPARK_H
