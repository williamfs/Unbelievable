#pragma once
#include "Blueprint/BlueprintSupport.h"
#include "MyProject/MyProjectCharacter.h"
#include "FirstPersonCharacter__pf2841129700.generated.h"
UCLASS(config=Game, Blueprintable, BlueprintType, meta=(ReplaceConverted="/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C", OverrideNativeName="FirstPersonCharacter_C"))
class AFirstPersonCharacter_C__pf2841129700 : public AMyProjectCharacter
{
public:
	GENERATED_BODY()
	AFirstPersonCharacter_C__pf2841129700(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	static void __CustomDynamicClassInitialization(UDynamicClass* InDynamicClass);
	static void __StaticDependenciesAssets(TArray<FBlueprintDependencyData>& AssetsToLoad);
	static void __StaticDependencies_DirectlyUsedAssets(TArray<FBlueprintDependencyData>& AssetsToLoad);
public:
};
