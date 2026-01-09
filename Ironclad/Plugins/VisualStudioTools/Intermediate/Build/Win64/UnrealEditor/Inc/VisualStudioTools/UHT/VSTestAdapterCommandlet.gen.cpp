// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VSTestAdapterCommandlet.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVSTestAdapterCommandlet() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSTestAdapterCommandlet();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSTestAdapterCommandlet_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVSTestAdapterCommandlet *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVSTestAdapterCommandlet;
UClass* UVSTestAdapterCommandlet::GetPrivateStaticClass()
{
	using TClass = UVSTestAdapterCommandlet;
	if (!Z_Registration_Info_UClass_UVSTestAdapterCommandlet.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VSTestAdapterCommandlet"),
			Z_Registration_Info_UClass_UVSTestAdapterCommandlet.InnerSingleton,
			StaticRegisterNativesUVSTestAdapterCommandlet,
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
	return Z_Registration_Info_UClass_UVSTestAdapterCommandlet.InnerSingleton;
}
UClass* Z_Construct_UClass_UVSTestAdapterCommandlet_NoRegister()
{
	return UVSTestAdapterCommandlet::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVSTestAdapterCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VSTestAdapterCommandlet.h" },
		{ "ModuleRelativePath", "Private/VSTestAdapterCommandlet.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVSTestAdapterCommandlet constinit property declarations *****************
// ********** End Class UVSTestAdapterCommandlet constinit property declarations *******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVSTestAdapterCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVSTestAdapterCommandlet_Statics
UObject* (*const Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::ClassParams = {
	&UVSTestAdapterCommandlet::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::Class_MetaDataParams)
};
void UVSTestAdapterCommandlet::StaticRegisterNativesUVSTestAdapterCommandlet()
{
}
UClass* Z_Construct_UClass_UVSTestAdapterCommandlet()
{
	if (!Z_Registration_Info_UClass_UVSTestAdapterCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVSTestAdapterCommandlet.OuterSingleton, Z_Construct_UClass_UVSTestAdapterCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVSTestAdapterCommandlet.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVSTestAdapterCommandlet);
UVSTestAdapterCommandlet::~UVSTestAdapterCommandlet() {}
// ********** End Class UVSTestAdapterCommandlet ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSTestAdapterCommandlet_h__Script_VisualStudioTools_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVSTestAdapterCommandlet, UVSTestAdapterCommandlet::StaticClass, TEXT("UVSTestAdapterCommandlet"), &Z_Registration_Info_UClass_UVSTestAdapterCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVSTestAdapterCommandlet), 2977761309U) },
	};
}; // Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSTestAdapterCommandlet_h__Script_VisualStudioTools_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSTestAdapterCommandlet_h__Script_VisualStudioTools_2369810655{
	TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSTestAdapterCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSTestAdapterCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
