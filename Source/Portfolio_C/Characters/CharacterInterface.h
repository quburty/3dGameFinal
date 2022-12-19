#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

#define ICharacter ICharacterInterface

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_C_API ICharacterInterface
{
	GENERATED_BODY()

public:

	//Components
	virtual class UCStateComponent* GetState() { return NULL; }
	virtual class UCStatusComponent* GetStatus() { return NULL; }
	virtual class UCMontagesComponent* GetMontage() { return NULL; }

	//AI
	virtual class UBehaviorTree* GetBehaviorTree() { return NULL; }
	virtual uint8 GetTeamID() { return 0; }
	virtual bool IsAlwaysPerceptPlayer() { return false; }

	virtual void Dead() {}

};
