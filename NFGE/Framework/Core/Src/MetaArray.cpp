#include "Precompiled.h"
#include "MetaArray.h"

using namespace NFGE::Core::Meta;

NFGE::Core::Meta::MetaArray::MetaArray(const MetaType * elementType, GetElementFunc getElement, GetCountFunc getCount)
	: MetaType(MetaType::Category::Array, "Array", sizeof(std::vector<int>), nullptr, nullptr)
	, mElemenType(elementType)
	, mGetElement(std::move(getElement))
	, mGetCount(std::move(getCount))
{
}
