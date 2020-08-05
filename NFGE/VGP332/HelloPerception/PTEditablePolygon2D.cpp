#include "PTEditablePolygon2D.h"

void PTEditablePolygon2D::DebugUIUpdate(const Camera2D & camera, float deltaTime)
{
}

bool PTEditablePolygon2D::IsLastLineSegmentInterceting()
{
	if (mVertices.size() < 3)
		return false;

	LineSegment theLine{ mVertices.back(), mVertices[mVertices.size() - 2] };

	for (size_t i = mVertices.size() - 2; i > 0; i--)
	{
		EngineWrapper::LineSegment currentEdge{ mVertices[i], mVertices[i - 1] };
		
		bool isOverLap;
		bool isSameLine;
		if (customIntersect(theLine, currentEdge, isOverLap, isSameLine))
		{
			if (i == mVertices.size() - 2) // is interct with last edge---> do the reset
			{
				ResetBack();
			}
			else if ((isOverLap == false) && (isSameLine == true))
			{
				return false;
			}
			return true;
		}
	}
	return false;
}

bool PTEditablePolygon2D::IsIntercet(const LineSegment & theLine)
{
	for (int i = 0; i < mVertices.size() - 2; i++)
	{
		EngineWrapper::LineSegment currentEdge{ mVertices[i], mVertices[i + 1] };
		bool isOverLap;
		bool isSameLine;
		if (customIntersect(theLine, currentEdge, isOverLap, isSameLine))
		{
			if ((isOverLap == true) || (isSameLine == false))
			{
				return true;
			}
		}
	}
	return false;
}

bool PTEditablePolygon2D::BuildingPolygon(const Vector2& newVertice)
{
	mVertices.back() = newVertice;

	if (EngineWrapper::IsMousePressed(Mouse::LBUTTON))
	{
		if (mVertices.size() <= 2)
		{
			mVertices.push_back(newVertice);
			return mIsBuilt;
		}

		bool isIntercet = IsLastLineSegmentInterceting();

		if ((mVertices.back().x == mVertices[0].x) && (mVertices.back().y == mVertices[0].y))
		{
			if (mVertices.size() < 3)
			{
				//TODO:: Update error message "Invalid shape"
				return mIsBuilt;
			}

			if (isIntercet)// reset the first vertices
			{
				mVertices[0] = mVertices[mVertices.size() - 2];
				mVertices.pop_back();
			}

			UpdateOrientationCount(mVertices[mVertices.size() - 3], mVertices[mVertices.size() - 2], mVertices[0], 1);
			FinishBuild();
			return mIsBuilt;
		}
		else
		{
			if (isIntercet)
				return mIsBuilt;
		}

		UpdateOrientationCount(mVertices[mVertices.size() - 3], mVertices[mVertices.size() - 2], mVertices[mVertices.size() - 1], 1);

		mVertices.push_back(newVertice);
		return mIsBuilt;
	}
	return mIsBuilt;
}

void PTEditablePolygon2D::FinishBuild()
{
	mVertices.pop_back(); // take out the last varticies
	mSides = mVertices.size();

	UpdateOrientationCount(mVertices.back(), mVertices[0], mVertices[1], 1);

	mIsBuilt = true;
}

void PTEditablePolygon2D::ResetBack()
{
	int specialAdd = mVertices.size() < 4 ? 1 : 0;
	UpdateOrientationCount(mVertices[mVertices.size() - 4 + specialAdd], mVertices[mVertices.size() - 3 + specialAdd], mVertices[mVertices.size() - 2 + specialAdd], -1);

	mVertices[mVertices.size() - 2] = mVertices.back();

	UpdateOrientationCount(mVertices[mVertices.size() - 4 + specialAdd], mVertices[mVertices.size() - 3 + specialAdd], mVertices[mVertices.size() - 2 + specialAdd], 1);
}

void PTEditablePolygon2D::UpdateOrientationCount(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2, int howMany)
{
	if (IsClockwise(v0,v1,v2))
	{
		mClockwiseCount += howMany;
	}
	else
	{
		mCounterClockwiseCount += howMany;
	}
}
