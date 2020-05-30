
//====================================================================================================
// Filename:	EngineWrapper.cpp
// Created by:	Mingzhuo Zhang
//====================================================================================================
#include "EngineWrapper.h"

using namespace EngineWrapper;

bool EngineWrapper::ClosePointInTriangle_Custom_OnEdgeCount(const Vector3 & p0, const Vector3 & p1, const Vector3 & p2, const Vector3 & checkingPoint)
{
	bool isInside = true;

	// check Regine p0
	if ((NFGE::Math::Dot(checkingPoint - p0, p1 - p0) < 0.0f) && (Dot(checkingPoint - p0, p2 - p0) < 0.0f))
	{
		isInside = false;
	}
	// check Regine p1
	else if ((NFGE::Math::Dot(checkingPoint - p1, p0 - p1) < 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p2 - p1) < 0.0f))
	{
		isInside = false;
	}
	// check Regine p2
	else if ((NFGE::Math::Dot(checkingPoint - p2, p0 - p2) < 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p1 - p2) < 0.0f))
	{
		isInside = false;
	}
	// check Regine p0-p1
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p2 - p1, p0 - p1), p0 - p1)), checkingPoint - p1) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p0, p1 - p0) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p0 - p1) > 0.0f))
	{
		isInside = false;
	}
	// check Regine p1-p2
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p0 - p2, p1 - p2), p1 - p2)), checkingPoint - p2) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p2 - p1) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p1 - p2) > 0.0f))
	{
		isInside = false;
	}
	// check Regine p2-p0
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p1 - p0, p2 - p0), p2 - p0)), checkingPoint - p0) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p0 - p2) > 0.0f) && (NFGE::Math::Dot(checkingPoint - p0, p2 - p0) > 0.0f))
	{
		isInside = false;
	}

	return isInside;
}
bool EngineWrapper::ClosePointInTriangle(const Vector3 & p0, const Vector3 & p1, const Vector3 & p2, const Vector3 & checkingPoint, Vector3 & returnPoint, float & returnDistance, Vector3 & farPoint)
{
	bool isInside = true;

	// check Regine p0
	if ((NFGE::Math::Dot(checkingPoint - p0, p1 - p0) < 0.0f) && (Dot(checkingPoint - p0, p2 - p0) < 0.0f))
	{
		isInside = false;
		returnPoint = p0;
		returnDistance = NFGE::Math::Magnitude(checkingPoint - p0);

		Vector3 dir = NFGE::Math::Normalize(returnPoint - checkingPoint);
		farPoint = Dot(p1, dir) > Dot(p2, dir) ? p1 : p2;
	}
	// check Regine p1
	else if ((NFGE::Math::Dot(checkingPoint - p1, p0 - p1) < 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p2 - p1) < 0.0f))
	{
		isInside = false;
		returnPoint = p1;
		returnDistance = NFGE::Math::Magnitude(checkingPoint - p1);
		farPoint = p2;

		Vector3 dir = NFGE::Math::Normalize(returnPoint - checkingPoint);
		farPoint = NFGE::Math::Dot(p0, dir) > NFGE::Math::Dot(p2, dir) ? p0 : p2;
	}
	// check Regine p2
	else if ((NFGE::Math::Dot(checkingPoint - p2, p0 - p2) < 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p1 - p2) < 0.0f))
	{
		isInside = false;
		returnPoint = p2;
		returnDistance = NFGE::Math::Magnitude(checkingPoint - p2);
		farPoint = p0;

		Vector3 dir = NFGE::Math::Normalize(returnPoint - checkingPoint);
		farPoint = NFGE::Math::Dot(p1, dir) > NFGE::Math::Dot(p0, dir) ? p1 : p0;
	}
	// check Regine p0-p1
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p2 - p1, p0 - p1), p0 - p1)), checkingPoint - p1) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p0, p1 - p0) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p0 - p1) >= 0.0f))
	{
		isInside = false;
		Vector3 dir = NFGE::Math::Normalize(NFGE::Math::Cross(NFGE::Math::Cross(p2 - p1, p0 - p1), p0 - p1));
		returnDistance = NFGE::Math::Dot((checkingPoint - p0), dir);
		returnPoint = checkingPoint + -dir * returnDistance;
		farPoint = p2;
	}
	// check Regine p1-p2
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p0 - p2, p1 - p2), p1 - p2)), checkingPoint - p2) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p1, p2 - p1) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p1 - p2) >= 0.0f))
	{
		isInside = false;
		Vector3 dir = NFGE::Math::Normalize(NFGE::Math::Cross(NFGE::Math::Cross(p0 - p2, p1 - p2), p1 - p2));
		returnDistance = Dot((checkingPoint - p1), dir);
		returnPoint = checkingPoint + -dir * returnDistance;
		farPoint = p0;
	}
	// check Regine p2-p0
	else if ((NFGE::Math::Dot((NFGE::Math::Cross(NFGE::Math::Cross(p1 - p0, p2 - p0), p2 - p0)), checkingPoint - p0) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p2, p0 - p2) >= 0.0f) && (NFGE::Math::Dot(checkingPoint - p0, p2 - p0) >= 0.0f))
	{
		isInside = false;
		Vector3 dir = NFGE::Math::Normalize(NFGE::Math::Cross(NFGE::Math::Cross(p1 - p0, p2 - p0), p2 - p0));
		returnDistance = Dot((checkingPoint - p2), dir);
		returnPoint = checkingPoint + -dir * returnDistance;
		farPoint = p1;
	}

	return isInside;
}

bool EngineWrapper::IsPointInTriangle(const Vector2 & p0, const Vector2 & p1, const Vector2 & p2, const Vector2 & checkingPoint)
{
	Vector3 returnPoint;
	float returnDistance;
	Vector3 farPoint;
	return ClosePointInTriangle(
		Vector3(p0.x, p0.y, 0.0f),
		Vector3(p1.x, p1.y, 0.0f),
		Vector3(p2.x, p2.y, 0.0f),
		Vector3(checkingPoint.x, checkingPoint.y, 0.0f),
		returnPoint,
		returnDistance,
		farPoint
	);
}

bool EngineWrapper::IsPointInTriangle_Custom_OnEdgeCount(const Vector2 & p0, const Vector2 & p1, const Vector2 & p2, const Vector2 & checkingPoint)
{
	return ClosePointInTriangle_Custom_OnEdgeCount(
		Vector3(p0.x, p0.y, 0.0f),
		Vector3(p1.x, p1.y, 0.0f),
		Vector3(p2.x, p2.y, 0.0f),
		Vector3(checkingPoint.x, checkingPoint.y, 0.0f)
	);
}

Rect EngineWrapper::GetMiniRect(const Triangle & tri)
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;

	for (auto& point: tri.v)
	{
		if (point.x < minX)
			minX = point.x;
		if (point.x > maxX)
			maxX = point.x;
		if (point.y < minY)
			minY = point.y;
		if (point.y > maxY)
			maxY = point.y;
	}
	return Rect(minX, minY, maxX, maxY);

}

EngineWrapper::TextureId EngineWrapper::LoadTexture(const char * filename)
{
	return NFGE::Graphics::TextureManager::Get()->LoadTexture(filename);
}

void EngineWrapper::DrawSprite(TextureId textureId, Vector2 position, float rotation, float alpha, float anchorX, float anchorY, float xScale, float yScale)
{
	NFGE::sApp.DrawSprite(textureId, position, rotation, alpha, anchorX, anchorY, xScale, yScale);
}

float EngineWrapper::GetSpriteWidth(TextureId texture)
{
	return static_cast<float>(NFGE::Graphics::TextureManager::Get()->GetSpriteWidth(texture));
}

float EngineWrapper::GetSpriteHeight(TextureId texuture)
{
	return static_cast<float>(NFGE::Graphics::TextureManager::Get()->GetSpriteHeight(texuture));
}

void * EngineWrapper::GetSprite(TextureId texture)
{
	return NFGE::Graphics::TextureManager::Get()->GetSprite(texture);
}

int EngineWrapper::GetScreenWidth()
{
	return NFGE::sApp.GetScreenWidth();
}

int EngineWrapper::GetScreenHeight()
{
	return NFGE::sApp.GetScreenHeight();
}

void EngineWrapper::DrawScreenCircle(const Vector2 & vector, float radius, const Color & color)
{
	NFGE::sApp.DrawScreenCircle(vector, radius, color);
}

void EngineWrapper::DrawScreenRect(const Rect & rect, const Color & color)
{
	NFGE::sApp.DrawScreenRect(rect, color);
}

void EngineWrapper::DrawScreenLine(const Vector2 & pos1, const Vector2 & pos2, const Color & color)
{
	NFGE::sApp.DrawScreenLine(pos1, pos2, color);
}

int EngineWrapper::GetMouseScreenX()
{
	return NFGE::Input::InputSystem::Get()->GetMouseScreenX();
}

int EngineWrapper::GetMouseScreenY()
{
	return NFGE::Input::InputSystem::Get()->GetMouseScreenY();
}

bool EngineWrapper::IsMousePressed(Mouse mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMousePressed(static_cast<Mouse>(mouseButton));
}

bool EngineWrapper::IsMouseDown(Mouse mouseButton)
{
	return NFGE::Input::InputSystem::Get()->IsMouseDown(static_cast<Mouse>(mouseButton));
}

bool EngineWrapper::IsKeyPressed(Keys button)
{
	return NFGE::Input::InputSystem::Get()->IsKeyPressed(static_cast<Keys>(button));
}

bool EngineWrapper::PointInCircle(const Vector2 & mousePosition, const Circle & circle)
{
	return NFGE::Math::PointInCircle(mousePosition, circle);
}

bool EngineWrapper::PointInRect(const Vector2 & mousePosition, const Rect & rect)
{
	return NFGE::Math::PointInRect(mousePosition, rect);
}

bool EngineWrapper::Intersect(const LineSegment& line1, const LineSegment& line2)
{
	return NFGE::Math::Intersect(line1, line2);
}

bool EngineWrapper::Intersect(const LineSegment & a, const LineSegment & b, Vector2 & intersectPoint)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	intersectPoint = a.from + (a.from - a.to) * ua;
	return true;
}

bool EngineWrapper::customIntersect(const LineSegment & a, const LineSegment & b, bool& IsOverLap, bool& IsSameLine)
{
		// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
		IsOverLap = false;
		IsSameLine = false;

		float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
		float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
		float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

		// First check for special cases
		if (denom == 0.0f)
		{
			if (ua == 0.0f && ub == 0.0f)
			{
				// The line segments are the same
				float distSqr_1 = NFGE::Math::MagnitudeSqr(a.from - b.from);
				float distSqr_2 = NFGE::Math::MagnitudeSqr(a.from - b.to);
				float LengthASqr = NFGE::Math::MagnitudeSqr(a.from - a.to);
				float LengthBSqr = NFGE::Math::MagnitudeSqr(b.from - b.to);
				if ((distSqr_1 == LengthBSqr) || (distSqr_1 == LengthASqr) || (distSqr_2 == LengthASqr) || (distSqr_2 == LengthBSqr))
					IsOverLap = true;
				IsSameLine = true;
				return true;
			}
			else
			{
				// The line segments are parallel
				return false;
			}
		}

		

		ua /= denom;
		ub /= denom;

		if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
		{
			return false;
		}
		else
		{
			if (((a.from.x == b.from.x)&&(a.from.y == b.from.y)) || ((a.from.x == b.to.x)&&(a.from.y == b.to.y)) ||							//Make the lines share ends are not mark as intercet
			((a.to.x == b.from.x) && (a.to.y == b.from.y)) || ((a.to.x == b.to.x) && (a.to.y == b.to.y)))
			{
			return false;
			}

		}

		return true;
	}

float EngineWrapper::Cross2D(const Vector2 & a, const Vector2 & b)
{
	return -((a.x * b.y) - (a.y * b.x)); // Base on the left hand rule system we need to negative it;
}

bool EngineWrapper::IsClockwise(const Vector2& a, const Vector2& b, const Vector2& c)
{
	Vector2 v1 = b - a;
	Vector2 v2 = c - b;
	return Cross2D(v1, v2) < 0.0f;
}

bool EngineWrapper::OpenFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.OpenFileDialog(fileName, title, filter);
}

bool EngineWrapper::SaveFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	return NFGE::sApp.SaveFileDialog(fileName, title, filter);
}
