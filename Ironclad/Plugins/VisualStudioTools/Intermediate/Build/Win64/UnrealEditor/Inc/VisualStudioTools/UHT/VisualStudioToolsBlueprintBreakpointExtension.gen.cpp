// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioToolsBlueprintBreakpointExtension.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVisualStudioToolsBlueprintBreakpointExtension() {}

// ********** Begin Cross Module References ********************************************************
EDITORSUBSYSTEM_API UClass* Z_Construct_UClass_UEditorSubsystem();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVisualStudioToolsBlueprintBreakpointExtension ***************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension;
UClass* UVisualStudioToolsBlueprintBreakpointExtension::GetPrivateStaticClass()
{
	using TClass = UVisualStudioToolsBlueprintBreakpointExtension;
	if (!Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VisualStudioToolsBlueprintBreakpointExtension"),
			Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.InnerSingleton,
			StaticRegisterNativesUVisualStudioToolsBlueprintBreakpointExtension,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.InnerSingleton;
}
UClass* Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_NoRegister()
{
	return UVisualStudioToolsBlueprintBreakpointExtension::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VisualStudioToolsBlueprintBreakpointExtension.h" },
		{ "ModuleRelativePath", "Private/VisualStudioToolsBlueprintBreakpointExtension.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVisualStudioToolsBlueprintBreakpointExtension constinit property declarations 
// ********** End Class UVisualStudioToolsBlueprintBreakpointExtension constinit property declarations 
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVisualStudioToolsBlueprintBreakpointExtension>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics
UObject* (*const Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEditorSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::ClassParams = {
	&UVisualStudioToolsBlueprintBreakpointExtension::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::Class_MetaDataParams), Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::Class_MetaDataParams)
};
void UVisualStudioToolsBlueprintBreakpointExtension::StaticRegisterNativesUVisualStudioToolsBlueprintBreakpointExtension()
{
}
UClass* Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension()
{
	if (!Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.OuterSingleton, Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension.OuterSingleton;
}
UVisualStudioToolsBlueprintBreakpointExtension::UVisualStudioToolsBlueprintBreakpointExtension() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVisualStudioToolsBlueprintBreakpointExtension);
UVisualStudioToolsBlueprintBreakpointExtension::~UVisualStudioToolsBlueprintBreakpointExtension() {}
// ********** End Class UVisualStudioToolsBlueprintBreakpointExtension *****************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsBlueprintBreakpointExtension_h__Script_VisualStudioTools_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVisualStudioToolsBlueprintBreakpointExtension, UVisualStudioToolsBlueprintBreakpointExtension::StaticClass, TEXT("UVisualStudioToolsBlueprintBreakpointExtension"), &Z_Registration_Info_UClass_UVisualStudioToolsBlueprintBreakpointExtension, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVisualStudioToolsBlueprintBreakpointExtension), 2439507063U) },
	};
}; // Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsBlueprintBreakpointExtension_h__Script_VisualStudioTools_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsBlueprintBreakpointExtension_h__Script_VisualStudioTools_4051345484{
	TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsBlueprintBreakpointExtension_h__Script_VisualStudioTools_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsBlueprintBreakpointExtension_h__Script_VisualStudioTools_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
