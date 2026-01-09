// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioToolsCommandlet.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVisualStudioToolsCommandlet() {}

// ********** Begin Cross Module References ********************************************************
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandlet();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandlet_NoRegister();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVisualStudioToolsCommandlet *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVisualStudioToolsCommandlet;
UClass* UVisualStudioToolsCommandlet::GetPrivateStaticClass()
{
	using TClass = UVisualStudioToolsCommandlet;
	if (!Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VisualStudioToolsCommandlet"),
			Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.InnerSingleton,
			StaticRegisterNativesUVisualStudioToolsCommandlet,
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
	return Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.InnerSingleton;
}
UClass* Z_Construct_UClass_UVisualStudioToolsCommandlet_NoRegister()
{
	return UVisualStudioToolsCommandlet::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VisualStudioToolsCommandlet.h" },
		{ "ModuleRelativePath", "Private/VisualStudioToolsCommandlet.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVisualStudioToolsCommandlet constinit property declarations *************
// ********** End Class UVisualStudioToolsCommandlet constinit property declarations ***************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVisualStudioToolsCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics
UObject* (*const Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UVisualStudioToolsCommandletBase,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::ClassParams = {
	&UVisualStudioToolsCommandlet::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::Class_MetaDataParams)
};
void UVisualStudioToolsCommandlet::StaticRegisterNativesUVisualStudioToolsCommandlet()
{
}
UClass* Z_Construct_UClass_UVisualStudioToolsCommandlet()
{
	if (!Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.OuterSingleton, Z_Construct_UClass_UVisualStudioToolsCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVisualStudioToolsCommandlet.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVisualStudioToolsCommandlet);
UVisualStudioToolsCommandlet::~UVisualStudioToolsCommandlet() {}
// ********** End Class UVisualStudioToolsCommandlet ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandlet_h__Script_VisualStudioTools_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVisualStudioToolsCommandlet, UVisualStudioToolsCommandlet::StaticClass, TEXT("UVisualStudioToolsCommandlet"), &Z_Registration_Info_UClass_UVisualStudioToolsCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVisualStudioToolsCommandlet), 36057173U) },
	};
}; // Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandlet_h__Script_VisualStudioTools_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandlet_h__Script_VisualStudioTools_1610014275{
	TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
