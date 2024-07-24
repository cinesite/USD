//
// Copyright 2018 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxr.h"
#include "pxr/usd/usd/usdaFileFormat.h"
#include "pxr/usd/usd/usdzFileFormat.h"
#include "pxr/usd/usd/usdzResolver.h"
#include "pxr/usd/usd/zipFile.h"

#include "pxr/usd/sdf/layer.h"

#include "pxr/usd/ar/packageUtils.h"
#include "pxr/usd/ar/resolver.h"
#include "pxr/usd/ar/resolverScopedCache.h"

#include "pxr/base/trace/trace.h"

#include "pxr/base/tf/registryManager.h"
#include "pxr/base/tf/type.h"

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PUBLIC_TOKENS(UsdUsdzFileFormatTokens, USD_USDZ_FILE_FORMAT_TOKENS);

TF_REGISTRY_FUNCTION(TfType)
{
    SDF_DEFINE_FILE_FORMAT(UsdUsdzFileFormat, SdfFileFormat);
}

UsdUsdzFileFormat::UsdUsdzFileFormat()
    : SdfFileFormat(UsdUsdzFileFormatTokens->Id,
                    UsdUsdzFileFormatTokens->Version,
                    UsdUsdzFileFormatTokens->Target,
                    UsdUsdzFileFormatTokens->Id)
{
}

UsdUsdzFileFormat::~UsdUsdzFileFormat()
{
}

bool 
UsdUsdzFileFormat::IsPackage() const
{
    return true;
}

namespace
{

std::string
_GetFirstFileInZipFile(const std::string& zipFilePath)
{
    const UsdZipFile zipFile = Usd_UsdzResolverCache::GetInstance()
        .FindOrOpenZipFile(zipFilePath).second;
    if (!zipFile) {
        return std::string();
    }

    const UsdZipFile::Iterator firstFileIt = zipFile.begin();
    return (firstFileIt == zipFile.end()) ? std::string() : *firstFileIt;
}

} // end anonymous namespace

std::string 
UsdUsdzFileFormat::GetPackageRootLayerPath(
    const std::string& resolvedPath) const
{
    TRACE_FUNCTION();
    return _GetFirstFileInZipFile(resolvedPath);
}

SdfAbstractDataRefPtr
UsdUsdzFileFormat::InitData(const FileFormatArguments& args) const
{
    return SdfFileFormat::InitData(args);
}

bool
UsdUsdzFileFormat::CanRead(const std::string& filePath) const
{
    TRACE_FUNCTION();

    const std::string firstFile = _GetFirstFileInZipFile(filePath);
    if (firstFile.empty()) {
        return false;
    }

    const SdfFileFormatConstPtr packagedFileFormat = 
        SdfFileFormat::FindByExtension(firstFile);
    if (!packagedFileFormat) {
        return false;
    }

    const std::string packageRelativePath = 
        ArJoinPackageRelativePath(filePath, firstFile);
    return packagedFileFormat->CanRead(packageRelativePath);
}

bool
UsdUsdzFileFormat::Read(
    SdfLayer* layer,
    const std::string& resolvedPath,
    bool metadataOnly) const
{
    TRACE_FUNCTION();

    return _ReadHelper</* Detached = */ false>(
        layer, resolvedPath, metadataOnly);
}

bool
UsdUsdzFileFormat::_ReadDetached(
    SdfLayer* layer,
    const std::string& resolvedPath,
    bool metadataOnly) const
{
    TRACE_FUNCTION();

    return _ReadHelper</* Detached = */ true>(
        layer, resolvedPath, metadataOnly);
}

template <bool Detached>
bool
UsdUsdzFileFormat::_ReadHelper(
    SdfLayer* layer,
    const std::string& resolvedPath,
    bool metadataOnly) const
{
    // Use a scoped cache here so we only open the .usdz asset once.
    //
    // If the call to Read below calls ArResolver::OpenAsset, it will
    // ultimately ask Usd_UsdzResolver to open the .usdz asset. The
    // scoped cache will ensure that will pick up the asset opened
    // in _GetFirstFileInZipFile instead of asking the resolver to
    // open the asset again.
    ArResolverScopedCache scopedCache;

    const std::string firstFile = _GetFirstFileInZipFile(resolvedPath);
    if (firstFile.empty()) {
        return false;
    }

    const SdfFileFormatConstPtr packagedFileFormat = 
        SdfFileFormat::FindByExtension(firstFile);
    if (!packagedFileFormat) {
        return false;
    }

    const std::string packageRelativePath = 
        ArJoinPackageRelativePath(resolvedPath, firstFile);
    return Detached ?
        packagedFileFormat->ReadDetached(
            layer, packageRelativePath, metadataOnly) :
        packagedFileFormat->Read(
            layer, packageRelativePath, metadataOnly);
}

bool
UsdUsdzFileFormat::WriteToFile(
    const SdfLayer& layer,
    const std::string& filePath,
    const std::string& comment,
    const FileFormatArguments& args) const
{
    TF_CODING_ERROR("Writing usdz layers is not allowed via this API.");
    return false;
}

bool 
UsdUsdzFileFormat::ReadFromString(
    SdfLayer* layer,
    const std::string& str) const
{
    return SdfFileFormat::FindById(UsdUsdaFileFormatTokens->Id)->
        ReadFromString(layer, str);
}

bool 
UsdUsdzFileFormat::WriteToString(
    const SdfLayer& layer,
    std::string* str,
    const std::string& comment) const
{
    return SdfFileFormat::FindById(UsdUsdaFileFormatTokens->Id)->
        WriteToString(layer, str, comment);
}

bool
UsdUsdzFileFormat::WriteToStream(
    const SdfSpecHandle &spec,
    std::ostream& out,
    size_t indent) const
{
    return SdfFileFormat::FindById(UsdUsdaFileFormatTokens->Id)->
        WriteToStream(spec, out, indent);
}

PXR_NAMESPACE_CLOSE_SCOPE

