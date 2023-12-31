// 版权在左，一切自由。


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (const auto& info : AttributeInformation)
	{
		if (info.AttributeTag == AttributeTag)
		{
			return info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Not found for attribute tag [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(),
		       *GetNameSafe(this ));
	}
	return FAuraAttributeInfo{};
}
