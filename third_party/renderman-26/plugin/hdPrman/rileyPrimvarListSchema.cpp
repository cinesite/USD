//
// Copyright 2023 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
////////////////////////////////////////////////////////////////////////

/* ************************************************************************** */
/* **                                                                      ** */
/* ** This file is generated by a script.                                  ** */
/* **                                                                      ** */
/* ** Do not edit it directly (unless it is within a CUSTOM CODE section)! ** */
/* ** Edit hdSchemaDefs.py instead to make changes.                        ** */
/* **                                                                      ** */
/* ************************************************************************** */

#include "hdPrman/rileyPrimvarListSchema.h"

#include "pxr/imaging/hd/retainedDataSource.h"

#include "pxr/base/trace/trace.h"

// --(BEGIN CUSTOM CODE: Includes)--
// --(END CUSTOM CODE: Includes)--

PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PUBLIC_TOKENS(HdPrmanRileyPrimvarListSchemaTokens,
    HD_PRMAN_RILEY_PRIMVAR_LIST_SCHEMA_TOKENS);

// --(BEGIN CUSTOM CODE: Schema Methods)--
// --(END CUSTOM CODE: Schema Methods)--

HdSizetDataSourceHandle
HdPrmanRileyPrimvarListSchema::GetNumUniform()
#if HD_API_VERSION >= 66
                                            const
#else
                                                 
#endif
{
    return _GetTypedDataSource<HdSizetDataSource>(
        HdPrmanRileyPrimvarListSchemaTokens->numUniform);
}

HdSizetDataSourceHandle
HdPrmanRileyPrimvarListSchema::GetNumVertex()
#if HD_API_VERSION >= 66
                                            const
#else
                                                 
#endif
{
    return _GetTypedDataSource<HdSizetDataSource>(
        HdPrmanRileyPrimvarListSchemaTokens->numVertex);
}

HdSizetDataSourceHandle
HdPrmanRileyPrimvarListSchema::GetNumVarying()
#if HD_API_VERSION >= 66
                                            const
#else
                                                 
#endif
{
    return _GetTypedDataSource<HdSizetDataSource>(
        HdPrmanRileyPrimvarListSchemaTokens->numVarying);
}

HdSizetDataSourceHandle
HdPrmanRileyPrimvarListSchema::GetNumFaceVarying()
#if HD_API_VERSION >= 66
                                            const
#else
                                                 
#endif
{
    return _GetTypedDataSource<HdSizetDataSource>(
        HdPrmanRileyPrimvarListSchemaTokens->numFaceVarying);
}

HdPrmanRileyPrimvarContainerSchema
HdPrmanRileyPrimvarListSchema::GetParams()
#if HD_API_VERSION >= 66
                                            const
#else
                                                 
#endif
{
    return HdPrmanRileyPrimvarContainerSchema(_GetTypedDataSource<HdContainerDataSource>(
        HdPrmanRileyPrimvarListSchemaTokens->params));
}

/*static*/
HdContainerDataSourceHandle
HdPrmanRileyPrimvarListSchema::BuildRetained(
        const HdSizetDataSourceHandle &numUniform,
        const HdSizetDataSourceHandle &numVertex,
        const HdSizetDataSourceHandle &numVarying,
        const HdSizetDataSourceHandle &numFaceVarying,
        const HdContainerDataSourceHandle &params
)
{
    TfToken _names[5];
    HdDataSourceBaseHandle _values[5];

    size_t _count = 0;

    if (numUniform) {
        _names[_count] = HdPrmanRileyPrimvarListSchemaTokens->numUniform;
        _values[_count++] = numUniform;
    }

    if (numVertex) {
        _names[_count] = HdPrmanRileyPrimvarListSchemaTokens->numVertex;
        _values[_count++] = numVertex;
    }

    if (numVarying) {
        _names[_count] = HdPrmanRileyPrimvarListSchemaTokens->numVarying;
        _values[_count++] = numVarying;
    }

    if (numFaceVarying) {
        _names[_count] = HdPrmanRileyPrimvarListSchemaTokens->numFaceVarying;
        _values[_count++] = numFaceVarying;
    }

    if (params) {
        _names[_count] = HdPrmanRileyPrimvarListSchemaTokens->params;
        _values[_count++] = params;
    }
    return HdRetainedContainerDataSource::New(_count, _names, _values);
}

HdPrmanRileyPrimvarListSchema::Builder &
HdPrmanRileyPrimvarListSchema::Builder::SetNumUniform(
    const HdSizetDataSourceHandle &numUniform)
{
    _numUniform = numUniform;
    return *this;
}

HdPrmanRileyPrimvarListSchema::Builder &
HdPrmanRileyPrimvarListSchema::Builder::SetNumVertex(
    const HdSizetDataSourceHandle &numVertex)
{
    _numVertex = numVertex;
    return *this;
}

HdPrmanRileyPrimvarListSchema::Builder &
HdPrmanRileyPrimvarListSchema::Builder::SetNumVarying(
    const HdSizetDataSourceHandle &numVarying)
{
    _numVarying = numVarying;
    return *this;
}

HdPrmanRileyPrimvarListSchema::Builder &
HdPrmanRileyPrimvarListSchema::Builder::SetNumFaceVarying(
    const HdSizetDataSourceHandle &numFaceVarying)
{
    _numFaceVarying = numFaceVarying;
    return *this;
}

HdPrmanRileyPrimvarListSchema::Builder &
HdPrmanRileyPrimvarListSchema::Builder::SetParams(
    const HdContainerDataSourceHandle &params)
{
    _params = params;
    return *this;
}

HdContainerDataSourceHandle
HdPrmanRileyPrimvarListSchema::Builder::Build()
{
    return HdPrmanRileyPrimvarListSchema::BuildRetained(
        _numUniform,
        _numVertex,
        _numVarying,
        _numFaceVarying,
        _params
    );
} 

PXR_NAMESPACE_CLOSE_SCOPE