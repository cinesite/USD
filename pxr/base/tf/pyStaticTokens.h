//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#ifndef PXR_BASE_TF_PY_STATIC_TOKENS_H
#define PXR_BASE_TF_PY_STATIC_TOKENS_H

/// \file tf/pyStaticTokens.h

#include "pxr/pxr.h"

#include <locale>

#include "pxr/base/tf/staticTokens.h"
#include "pxr/base/tf/preprocessorUtilsLite.h"

#include <boost/python/class.hpp>
#include <boost/python/scope.hpp>

PXR_NAMESPACE_OPEN_SCOPE

/// Macro to wrap static tokens defined with \c TF_DEFINE_PUBLIC_TOKENS to
/// Python.  It creates a class of name \p name in the current scope
/// containing just the tokens in \p seq in the static tokens named by \p key.
///
/// \hideinitializer
#define TF_PY_WRAP_PUBLIC_TOKENS(name, key, seq)                            \
    boost::python::class_<_TF_TOKENS_STRUCT_NAME(key), boost::noncopyable>( \
            name, boost::python::no_init)                                   \
        _TF_PY_TOKENS_WRAP_SEQ(key, seq)

/// Macro to wrap static tokens defined with \c TF_DEFINE_PUBLIC_TOKENS to
/// Python. This wraps tokens in \p seq in the static tokens named by \p key
/// as attributes on the current boost python scope.
///
/// \hideinitializer
#define TF_PY_WRAP_PUBLIC_TOKENS_IN_CURRENT_SCOPE(key, seq)                 \
    _TF_PY_TOKENS_WRAP_ATTR_SEQ(key, seq)

// Helper to return a static token as a string.  We wrap tokens as Python
// strings and for some reason simply wrapping the token using def_readonly
// bypasses to-Python conversion, leading to the error that there's no
// Python type for the C++ TfToken type.  So we wrap this functor instead.
class _TfPyWrapStaticToken {
public:
    _TfPyWrapStaticToken(const TfToken* token) : _token(token) { }

    std::string operator()() const
    {
        return _token->GetString();
    }

private:
    const TfToken* _token;
};

// Private macros to add a single data member.
#define _TF_PY_TOKENS_WRAP_ATTR_MEMBER(r, key, name)                        \
    boost::python::scope().attr(                                            \
        TF_PP_STRINGIZE(name)) = key->name.GetString();

#define _TF_PY_TOKENS_WRAP_MEMBER(r, key, name)                             \
    .add_static_property(TF_PP_STRINGIZE(name),                             \
        boost::python::make_function(_TfPyWrapStaticToken((&key->name)),    \
            boost::python::return_value_policy<                             \
                boost::python::return_by_value>(),                          \
            boost::mpl::vector1<std::string>()))

// Private macros to wrap a single element in a sequence.
#define _TF_PY_TOKENS_WRAP_ELEMENT(key, elem)                               \
    _TF_PY_TOKENS_WRAP_MEMBER(r, key, _TF_PY_TOKEN_GET_ELEM(elem))

#define _TF_PY_TOKENS_WRAP_ATTR_ELEMENT(key, elem)                          \
    _TF_PY_TOKENS_WRAP_ATTR_MEMBER(r, key, _TF_PY_TOKEN_GET_ELEM(elem))

#define _TF_PY_TOKEN_GET_ELEM(elem)                                         \
    _TF_PP_IFF(TF_PP_IS_TUPLE(elem),                                        \
        TF_PP_TUPLE_ELEM(0, elem), elem)

// Private macros to wrap a sequence.
#define _TF_PY_TOKENS_WRAP_SEQ(key, seq)                                    \
    TF_PP_SEQ_FOR_EACH(_TF_PY_TOKENS_WRAP_ELEMENT, key, seq)

#define _TF_PY_TOKENS_WRAP_ATTR_SEQ(key, seq)                               \
    TF_PP_SEQ_FOR_EACH(_TF_PY_TOKENS_WRAP_ATTR_ELEMENT, key, seq)

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_BASE_TF_PY_STATIC_TOKENS_H
