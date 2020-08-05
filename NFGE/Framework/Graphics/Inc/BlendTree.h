//===================================================================================
// Filename:	BlendTree.h
// Created by:	Mingzhuo Zhang
// Created at:	2020-02-10
// Description:	
//===================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) Mingzhuo Zhang All rights reserved.
//===================================================================================

#ifndef INCLUDE_NFGE_GRAPHICS_BLENDTREE
#define INCLUDE_NFGE_GRAPHICS_BLENDTREE

namespace NFGE::Graphics
{
	struct ClipModifyer
	{
		int mClipIndex = -1;
		float mWeight = 0.0f;

		bool mIsPlayTimeRelateWeight = false;
		float mPlayTimeWeight = 0.0f;
	};

	struct ClipNode
	{
		virtual void AddChildNode(ClipNode* childPtr) = 0;
		virtual void SetChildNode(ClipNode* childPtr, int position) = 0;
		virtual void Blending(std::vector<ClipModifyer*>& activeClipNodePtr, float weight, float time) = 0;
	};

	template<class BlendPolicy, class BlendContext>
	struct BlendNode : public ClipNode
	{
		BlendNode(const char* name, BlendContext& context, int childNodeCount, bool isLeaf = false, int clipIndex = -1, bool isTimeRelateWeight = false)
			: mName(name)
			, mIsLeaf(isLeaf)
			, mBlendPolicy()
			, mBlendContext(context)
		{
			if (isLeaf)
			{	
				ASSERT(clipIndex != -1, "[BlendTree::BlendNode] Leaf BlendNode should have 1 clipIndex");
				mClipModifyer.mClipIndex = clipIndex;
				mClipModifyer.mIsPlayTimeRelateWeight = isTimeRelateWeight;
			}
			else
			{
				ASSERT(childNodeCount > 0, "[BlendTree::BlendNode] Non-leaf BlendNode should have 0 clipIndex");
				mChildren.resize(childNodeCount);
			}
		}

		void AddChildNode(ClipNode* childPtr)
		{
			mChildren.push_back(childPtr);
		}

		void SetChildNode(ClipNode* childPtr, int position)
		{
			mChildren[position] = childPtr;
		}

		void Blending(std::vector<ClipModifyer*>& activeClipNodePtr, float weight, float time)
		{
			if (weight == 0.0f)
				return;
			if (mIsLeaf)
				mBlendPolicy(mClipModifyer, activeClipNodePtr, weight, mBlendContext, time);
			else
				mBlendPolicy(mChildren, activeClipNodePtr, weight, mBlendContext, time);
		}

		const char* mName;
		ClipModifyer mClipModifyer;
		BlendPolicy mBlendPolicy;
		BlendContext& mBlendContext;
		bool mIsLeaf;
		std::vector<ClipNode*> mChildren;
	};

	template<class BlendContext>
	struct DefaultBlendPolicy
	{
		void operator()(ClipModifyer& clipModifyer, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const BlendContext& context, float timeWeight)
		{
			clipModifyer.mWeight = weight;
			if (clipModifyer.mIsPlayTimeRelateWeight)
				clipModifyer.mPlayTimeWeight = timeWeight;

			activeClipNodePtr.push_back(&clipModifyer);
		}
		void operator()(const std::vector<ClipNode*>& mClipModifyers, std::vector<ClipModifyer*>& activeClipNodePtr, float weight, const BlendContext& context, float time)
		{
			ASSERT(false, "[DefaultBlendPolicy] should not calling this overload");
		}
	};

	class BlendTreeBase
	{
	public:
		using ClipModifyerPtrs = std::vector<ClipModifyer*>;

		virtual void Update(float deltaTime) = 0;

		virtual ClipNode* AddBlendNode(ClipNode* newNode) = 0;

		virtual const ClipModifyerPtrs& GetClipModifyers() const = 0;

		virtual float GetBlendTime() const = 0;
	};

	template<class BlendContext>
	class BlendTree : public BlendTreeBase
	{
	public:
		using ClipModifyerPtrs = std::vector<ClipModifyer*>;

		BlendTree(BlendContext blendContext)
			: mContext(blendContext)
		{

		}
		~BlendTree()
		{
			for (auto nodePtr : mAllNodes)
				delete nodePtr;
		}
		void Update(float deltaTime)
		{
			mActiveClipNodesPtr.clear();

			mRootNode->Blending(mActiveClipNodesPtr, 1.0f, mTimer);

			mTimer += deltaTime;
		}

		ClipNode* AddBlendNode(ClipNode* newNode)
		{
			if (newNode != nullptr)
			{
				//BlendNode<DefaultBlendPolicy, BlendContext> newNode = new BlendNode<DefaultBlendPolicy, BlendContext>(name, clipIndexes, true, mContext);
				mAllNodes.emplace_back(newNode);
				return newNode;
			}
		}

		const ClipModifyerPtrs& GetClipModifyers() const { return mActiveClipNodesPtr; }

		float GetBlendTime() const { return mTimer; }
		BlendContext& GetBlendContext() { return mContext; };

		void SetRootNode(ClipNode* nodePtr) { mRootNode = nodePtr; }
	private:
		std::vector<ClipNode*> mAllNodes;
		ClipNode* mRootNode;
		BlendContext mContext;
		float mTimer = 0.0f;

		ClipModifyerPtrs mActiveClipNodesPtr;
	};
}

#endif // !INCLUDE_NFGE_GRAPHICS_BLENDTREE
