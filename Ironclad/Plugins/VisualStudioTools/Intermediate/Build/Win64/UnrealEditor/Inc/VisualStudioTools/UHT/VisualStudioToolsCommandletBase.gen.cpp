// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioToolsCommandletBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVisualStudioToolsCommandletBase() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVisualStudioToolsCommandletBase *****************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase;
UClass* UVisualStudioToolsCommandletBase::GetPrivateStaticClass()
{
	using TClass = UVisualStudioToolsCommandletBase;
	if (!Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VisualStudioToolsCommandletBase"),
			Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.InnerSingleton,
			StaticRegisterNativesUVisualStudioToolsCommandletBase,
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
	return Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.InnerSingleton;
}
UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase_NoRegister()
{
	return UVisualStudioToolsCommandletBase::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VisualStudioToolsCommandletBase.h" },
		{ "ModuleRelativePath", "Private/VisualStudioToolsCommandletBase.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVisualStudioToolsCommandletBase constinit property declarations *********
// ********** End Class UVisualStudioToolsCommandletBase constinit property declarations ***********
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVisualStudioToolsCommandletBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics
UObject* (*const Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::ClassParams = {
	&UVisualStudioToolsCommandletBase::StaticClass,
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
	0x000000A9u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::Class_MetaDataParams)
};
void UVisualStudioToolsCommandletBase::StaticRegisterNativesUVisualStudioToolsCommandletBase()
{
}
UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase()
{
	if (!Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton, Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVisualStudioToolsCommandletBase);
UVisualStudioToolsCommandletBase::~UVisualStudioToolsCommandletBase() {}
// ********** End Class UVisualStudioToolsCommandletBase *******************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h__Script_VisualStudioTools_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVisualStudioToolsCommandletBase, UVisualStudioToolsCommandletBase::StaticClass, TEXT("UVisualStudioToolsCommandletBase"), &Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVisualStudioToolsCommandletBase), 6677925U) },
	};
}; // Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h__Script_VisualStudioTools_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h__Script_VisualStudioTools_399865609{
	TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h__Script_VisualStudioTools_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h__Script_VisualStudioTools_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
