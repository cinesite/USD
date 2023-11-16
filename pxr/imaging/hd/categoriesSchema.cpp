//
// Copyright 2023 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
////////////////////////////////////////////////////////////////////////

/* ************************************************************************** */
/* **                                                                      ** */
/* ** This file is generated by a script.                                  ** */
/* **                                                                      ** */
/* ** Do not edit it directly (unless it is within a CUSTOM CODE section)! ** */
/* ** Edit hdGen/schema.py instead to make changes.                        ** */
/* **                                                                      ** */
/* ************************************************************************** */

#include "pxr/imaging/hd/categoriesSchema.h"
#include "pxr/imaging/hd/retainedDataSource.h"

#include "pxr/base/trace/trace.h"


PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PUBLIC_TOKENS(HdCategoriesSchemaTokens,
    HD_CATEGORIES_SCHEMA_TOKENS);

// --(BEGIN CUSTOM CODE: Schema Methods)--

VtArray<TfToken>
HdCategoriesSchema::GetIncludedCategoryNames()
{
    VtArray<TfToken> result;
    if (!_container) {
        return result;
    }

    TfTokenVector names = _container->GetNames();
    result.reserve(names.size());
    for (const TfToken &name : names) {
        if (HdBoolDataSourceHandle valueDs =
                HdBoolDataSource::Cast(_container->Get(name))) {
            if (valueDs->GetTypedValue(0.0f)) {
                result.push_back(name);
            }
        }
    }
    return result;
}

bool
HdCategoriesSchema::IsIncludedInCategory(const TfToken &categoryName)
{
    if (HdBoolDataSourceHandle valueDs =
            _GetTypedDataSource<HdBoolDataSource>(categoryName)) {
        if (valueDs->GetTypedValue(0.0f)) {
            return true;
        }
    }

    return false;
}

HdContainerDataSourceHandle
HdCategoriesSchema::BuildRetained(
    size_t includedNameCount,
    TfToken *includedNames,
    size_t excludedNameCount,
    TfToken *excludedNames)
{
    TRACE_FUNCTION();

    static const HdRetainedTypedSampledDataSource<bool>::Handle boolTrueDs =
            HdRetainedTypedSampledDataSource<bool>::New(true);
    static const HdRetainedTypedSampledDataSource<bool>::Handle boolFalseDs =
            HdRetainedTypedSampledDataSource<bool>::New(false);

    std::vector<HdDataSourceBaseHandle> values;

    if (includedNameCount && includedNames && excludedNameCount == 0) {
        // only included (doesn't need to combine name arrays)
        values.reserve(includedNameCount);
        for (size_t i = 0; i < includedNameCount; ++i) {
            values.push_back(boolTrueDs);
        }
        return HdRetainedContainerDataSource::New(
            includedNameCount, includedNames, values.data());
    } else if (excludedNameCount && excludedNames && includedNameCount == 0) {
        // only excluded (doesn't need to combine name arrays)
        values.reserve(excludedNameCount);
        for (size_t i = 0; i < excludedNameCount; ++i) {
            values.push_back(boolFalseDs);
        }
        return HdRetainedContainerDataSource::New(
            excludedNameCount, excludedNames, values.data());
    } else {
        // both (needs to combine the included and excluded arrays)
        TfTokenVector names;
        names.reserve(includedNameCount + excludedNameCount);
        values.reserve(includedNameCount + excludedNameCount);

        if (includedNames) {
            for (size_t i = 0; i < includedNameCount; ++i) {
                names.push_back(includedNames[i]);
                values.push_back(boolTrueDs);
            }
        }

        if (excludedNames) {
            for (size_t i = 0; i < excludedNameCount; ++i) {
                names.push_back(excludedNames[i]);
                values.push_back(boolFalseDs);
            }
        }

        return HdRetainedContainerDataSource::New(
            names.size(), names.data(), values.data());
    }
}

// --(END CUSTOM CODE: Schema Methods)--




/*static*/
HdCategoriesSchema
HdCategoriesSchema::GetFromParent(
        const HdContainerDataSourceHandle &fromParentContainer)
{
    return HdCategoriesSchema(
        fromParentContainer
        ? HdContainerDataSource::Cast(fromParentContainer->Get(
                HdCategoriesSchemaTokens->categories))
        : nullptr);
}

/*static*/
const TfToken &
HdCategoriesSchema::GetSchemaToken()
{
    return HdCategoriesSchemaTokens->categories;
}

/*static*/
const HdDataSourceLocator &
HdCategoriesSchema::GetDefaultLocator()
{
    static const HdDataSourceLocator locator(
        HdCategoriesSchemaTokens->categories
    );
    return locator;
} 
PXR_NAMESPACE_CLOSE_SCOPE