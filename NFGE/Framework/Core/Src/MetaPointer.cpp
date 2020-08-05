#include "Precompiled.h"
#include "MetaPointer.h"

using namespace NFGE::Core::Meta;

NFGE::Core::Meta::MetaPointer::MetaPointer(const MetaType * pointer)
	: MetaType(MetaType::Category::Pointer, "Pointer", sizeof(nullptr), nullptr, nullptr)
	, mPointerType(pointer)
{
}
