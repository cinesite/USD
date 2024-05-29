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

#ifndef EXT_RMANPKG_25_0_PLUGIN_RENDERMAN_PLUGIN_HD_PRMAN_RILEY_DISPLACEMENT_SCHEMA_H
#define EXT_RMANPKG_25_0_PLUGIN_RENDERMAN_PLUGIN_HD_PRMAN_RILEY_DISPLACEMENT_SCHEMA_H

/// \file

#include "hdPrman/api.h"
#include "hdPrman/rileySchemaTypeDefs.h"
#include "hdPrman/rileyParamListSchema.h"

#include "pxr/imaging/hd/schema.h"
#include "pxr/imaging/hd/version.h"

// --(BEGIN CUSTOM CODE: Includes)--
// --(END CUSTOM CODE: Includes)--

PXR_NAMESPACE_OPEN_SCOPE

// --(BEGIN CUSTOM CODE: Declares)--
// --(END CUSTOM CODE: Declares)--

#define HD_PRMAN_RILEY_DISPLACEMENT_SCHEMA_TOKENS \
    (rileyDisplacement) \
    (displacement) \
    (attributes) \

TF_DECLARE_PUBLIC_TOKENS(HdPrmanRileyDisplacementSchemaTokens, HDPRMAN_API,
    HD_PRMAN_RILEY_DISPLACEMENT_SCHEMA_TOKENS);

//-----------------------------------------------------------------------------


class HdPrmanRileyDisplacementSchema : public HdSchema
{
public:
    /// \name Schema retrieval
    /// @{

    HdPrmanRileyDisplacementSchema(HdContainerDataSourceHandle container)
      : HdSchema(container) {}

    /// Retrieves a container data source with the schema's default name token
    /// "rileyDisplacement" from the parent container and constructs a
    /// HdPrmanRileyDisplacementSchema instance.
    /// Because the requested container data source may not exist, the result
    /// should be checked with IsDefined() or a bool comparison before use.
    HDPRMAN_API
    static HdPrmanRileyDisplacementSchema GetFromParent(
        const HdContainerDataSourceHandle &fromParentContainer);

    /// @}

// --(BEGIN CUSTOM CODE: Schema Methods)--
// --(END CUSTOM CODE: Schema Methods)--

    /// \name Member accessor
    /// @{

    HDPRMAN_API
    HdPrmanRileyShadingNodeVectorSchema GetDisplacement()
#if HD_API_VERSION >= 66
                                            const;
#else
                                                 ;
#endif


    HDPRMAN_API
    HdPrmanRileyParamListSchema GetAttributes()
#if HD_API_VERSION >= 66
                                            const;
#else
                                                 ;
#endif
 

    /// @}

    /// \name Schema location
    /// @{

    /// Returns a token where the container representing this schema is found in
    /// a container by default.
    HDPRMAN_API
    static const TfToken &GetSchemaToken();

    /// Returns an HdDataSourceLocator (relative to the prim-level data source)
    /// where the container representing this schema is found by default.
    HDPRMAN_API
    static const HdDataSourceLocator &GetDefaultLocator();

    /// @}

    /// \name Data source locators for members
    ///
    /// The following methods return an HdDataSourceLocator (relative to the
    /// prim-level data source) where the data source for a member can be found.
    ///
    /// This is often useful for checking intersection against the
    /// HdDataSourceLocatorSet sent with HdDataSourceObserver::PrimsDirtied.
    /// @{

    /// Prim-level relative data source locator to locate displacement.
    HDPRMAN_API
    static const HdDataSourceLocator &GetDisplacementLocator();

    /// Prim-level relative data source locator to locate attributes.
    HDPRMAN_API
    static const HdDataSourceLocator &GetAttributesLocator();
    /// @} 

    /// \name Schema construction
    /// @{

    /// \deprecated Use Builder instead.
    ///
    /// Builds a container data source which includes the provided child data
    /// sources. Parameters with nullptr values are excluded. This is a
    /// low-level interface. For cases in which it's desired to define
    /// the container with a sparse set of child fields, the Builder class
    /// is often more convenient and readable.
    HDPRMAN_API
    static HdContainerDataSourceHandle
    BuildRetained(
        const HdVectorDataSourceHandle &displacement,
        const HdContainerDataSourceHandle &attributes
    );

    /// \class HdPrmanRileyDisplacementSchema::Builder
    /// 
    /// Utility class for setting sparse sets of child data source fields to be
    /// filled as arguments into BuildRetained. Because all setter methods
    /// return a reference to the instance, this can be used in the "builder
    /// pattern" form.
    class Builder
    {
    public:
        HDPRMAN_API
        Builder &SetDisplacement(
            const HdVectorDataSourceHandle &displacement);
        HDPRMAN_API
        Builder &SetAttributes(
            const HdContainerDataSourceHandle &attributes);

        /// Returns a container data source containing the members set thus far.
        HDPRMAN_API
        HdContainerDataSourceHandle Build();

    private:
        HdVectorDataSourceHandle _displacement;
        HdContainerDataSourceHandle _attributes;

    };

    /// @}
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif