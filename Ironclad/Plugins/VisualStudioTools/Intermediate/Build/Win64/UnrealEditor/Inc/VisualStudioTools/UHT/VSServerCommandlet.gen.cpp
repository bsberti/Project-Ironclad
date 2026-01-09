// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VSServerCommandlet.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVSServerCommandlet() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSServerCommandlet();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSServerCommandlet_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVSServerCommandlet ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVSServerCommandlet;
UClass* UVSServerCommandlet::GetPrivateStaticClass()
{
	using TClass = UVSServerCommandlet;
	if (!Z_Registration_Info_UClass_UVSServerCommandlet.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VSServerCommandlet"),
			Z_Registration_Info_UClass_UVSServerCommandlet.InnerSingleton,
			StaticRegisterNativesUVSServerCommandlet,
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
	return Z_Registration_Info_UClass_UVSServerCommandlet.InnerSingleton;
}
UClass* Z_Construct_UClass_UVSServerCommandlet_NoRegister()
{
	return UVSServerCommandlet::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVSServerCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VSServerCommandlet.h" },
		{ "ModuleRelativePath", "Private/VSServerCommandlet.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVSServerCommandlet constinit property declarations **********************
// ********** End Class UVSServerCommandlet constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVSServerCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVSServerCommandlet_Statics
UObject* (*const Z_Construct_UClass_UVSServerCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVSServerCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVSServerCommandlet_Statics::ClassParams = {
	&UVSServerCommandlet::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVSServerCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UVSServerCommandlet_Statics::Class_MetaDataParams)
};
void UVSServerCommandlet::StaticRegisterNativesUVSServerCommandlet()
{
}
UClass* Z_Construct_UClass_UVSServerCommandlet()
{
	if (!Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton, Z_Construct_UClass_UVSServerCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVSServerCommandlet);
UVSServerCommandlet::~UVSServerCommandlet() {}
// ********** End Class UVSServerCommandlet ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h__Script_VisualStudioTools_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVSServerCommandlet, UVSServerCommandlet::StaticClass, TEXT("UVSServerCommandlet"), &Z_Registration_Info_UClass_UVSServerCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVSServerCommandlet), 2538279133U) },
	};
}; // Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h__Script_VisualStudioTools_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h__Script_VisualStudioTools_3990013445{
	TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_Project_Ironclad_Ironclad_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h__Script_VisualStudioTools_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
