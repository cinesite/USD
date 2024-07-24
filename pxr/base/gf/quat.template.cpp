//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// quat.template.cpp file to make changes.

#include "pxr/pxr.h"
#include "pxr/base/gf/quat{{ SUFFIX }}.h"
#include "pxr/base/gf/ostreamHelpers.h"
#include "pxr/base/gf/vec4{{ SUFFIX }}.h"
#include "pxr/base/tf/type.h"

{% for S in SCALARS if S != SCL -%}
#include "pxr/base/gf/quat{{ SCALAR_SUFFIX(S) }}.h"
{% endfor %}

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfType) {
    TfType::Define<{{ QUAT }}>();
}

{% for S in SCALARS if S != SCL %}
{{ QUAT }}::{{ QUAT }}({{ QUATNAME(S) }} const &other)
    : _imaginary(other.GetImaginary())
    , _real(other.GetReal())
{
}
{% endfor %}

std::ostream& 
operator<<(std::ostream &out, {{ QUAT }} const &q)
{
    GfVec3{{ SUFFIX }} i = q.GetImaginary();
    {{ SCL }} real = q.GetReal();
    return out << GfVec4{{ SUFFIX }}(real, i[0], i[1], i[2]);
}

{{ SCL }}
{{ QUAT }}::Normalize({{ SCL }} eps)
{
    {{ SCL }} length = GetLength();

    if (length < eps)
        *this = GetIdentity();
    else
        *this /= length;

    return length;
}

// The derivation of this is somewhat obscure so here's how this works.
//
// If we call this quaternion Q, and the point P, the goal is to
// evaluate the expression
//
//     (Q * {{ QUAT }}(0, P) * Q.GetInverse()).GetImaginary()
// which evaluates to point, rotated by the quaternion.
// 
// A quaternion has a scalar real part and a vector imaginary part. Call them
// r and i. The multiplication of two quaternions A * B expands to:
//
//     (Ar, Ai)*(Br, Bi) = (Ar*Br-(Ai . Bi), Ar*Bi+Br*Ai+(Ai x Bi))
//                           ^real part^       ^imaginary part^
// 
// where . is vector dot product and x is vector cross product.
//
// Let us define len2 as the quaternion length squared. If the length is 1.0
// (which is true for quaternions representing rotations) len2 will also be 1.0.
//
//     len2 = Q.GetLength()^2
//          = Qr*Qr + (Qi . Qi)
//
// There are a few definitions and identities that will help us out here:
//     Q.GetInverse() == (Qr, -Qi) * s
//     (v1 x v2) = -(v2 x v1)
//     v1 x (v2 x v3) = (v1 . v3)*v2 - (v1 . v2)*v3
//     v1 x (v1 x v2) = (v1 . v2)*v1 - (v1 . v1)*v2
//
// That last identity is a special case of the one above it and it comes up in
// the derivation.
//
// Finally, let R equal the result of transforming P by Q. This gives us the
// quaternion equation:
//     (0, R) = (Qr, Qi) * (0, P) * (Qr, -Qi) / len2
//
// For sake of space, we're going to just assume that the entire right-hand side
// gets divided by len2 and save it out of the equations until the end.
//
//     (0, R) = (-(Qi . P), Qr*P + (Qi x P)) * (Qr, -Qi)
//     (0, R) = ( ..., -(Qi . P)*(-Qi) + Qr*(Qr*P + (Qi x P)) + (Qr*P + (Qi x P)) x (-Qi))
//
// where ... represents the real part which simplifies to 0 and will henceforth be ignored:
//
//     R = -(Qi . P)*(-Qi) + Qr*(Qr*P + (Qi x P)) + (Qr*P + (Qi x P)) x (-Qi)
//       = (Qi . P)*Qi + Qr^2*P + Qr*(Qi x P) + (Qr*P x (-Qi)) + ((Qi x P) x (-Qi))
//       = (Qi . P)*Qi + Qr^2*P + Qr*(Qi x P) + Qr*(Qi x P) + (Qi x (Qi x P))
//       = (Qi . P)*Qi + Qr^2*P + 2*Qr*(Qi x P) + (Qi x (Qi x P))
//       = (Qi . P)*Qi + Qr^2*P + 2*Qr*(Qi x P) + (Qi . P)*Qi - (Qi . Qi)*P
//       = 2*(Qi . P)*Qi + (Qr^2 - (Qi . Qi))*P + 2*Qr*(Qi x P)
//
// now that we've arrived here, lets reintroduce len2 to get:
//
//     R = (2*(Qi . P)*Qi + (Qr^2 - (Qi . Qi))*P + 2*Qr*(Qi x P)) / len2
//
// We can further notice that Qr^2 and (Qi . Qi) both appear in the equation
// above and in the calculation of len2, giving us:
//
//     tmpDot = Qi . Qi
//     tmpSqr = Qr * Qr
//     R = (2*(Qi . P)*Qi + (tmpSqr - tmpDot)*P + 2*Qr*(Qi x P)) / (tmpSqr + tmpDot)
//
// which takes 22 multiplications, 11 additions, and 1 division
//
GfVec3{{ SUFFIX }}
{{ QUAT }}::Transform(const GfVec3{{ SUFFIX }}& point) const
{
    {{ SCL }} tmpDot = GfDot(_imaginary, _imaginary);
    {{ SCL }} tmpSqr = _real * _real;
    return (2 * GfDot(_imaginary, point) * _imaginary +
            (tmpSqr - tmpDot) * point +
            2 * _real * GfCross(_imaginary, point)) / (tmpSqr + tmpDot);
}

{{ QUAT }} &
{{ QUAT }}::operator *=(const {{ QUAT }} &q)
{
    {{ SCL }} r1 = GetReal();
    {{ SCL }} r2 = q.GetReal();
    const GfVec3{{ SUFFIX }} &i1 = GetImaginary();
    const GfVec3{{ SUFFIX }} &i2 = q.GetImaginary();

    {{ SCL }} r = r1 * r2 - GfDot(i1, i2);

    GfVec3{{ SUFFIX }} i(
        r1 * i2[0] + r2 * i1[0] + (i1[1] * i2[2] - i1[2] * i2[1]),
        r1 * i2[1] + r2 * i1[1] + (i1[2] * i2[0] - i1[0] * i2[2]),
        r1 * i2[2] + r2 * i1[2] + (i1[0] * i2[1] - i1[1] * i2[0]));

    SetReal(r);
    SetImaginary(i);

    return *this;
}

{{ QUAT }}
GfSlerp(const {{ QUAT }}& q0, const {{ QUAT }}& q1, double alpha)
{
    return GfSlerp(alpha, q0, q1);
}

{{ QUAT }}
GfSlerp(double alpha, const {{ QUAT }}& q0, const {{ QUAT }}& q1)
{
    double cosTheta = q0.GetImaginary() * q1.GetImaginary() +
        q0.GetReal() * q1.GetReal();
    bool flip1 = false;

    if (cosTheta < 0.0) {
        cosTheta = -cosTheta;
        flip1 = true;
    }

    double scale0, scale1;

    if (1.0 - cosTheta > 0.00001 ) {
        // standard case
        {{ SCL }} theta = acos(cosTheta),
               sinTheta = sin(theta);

        scale0 = sin((1.0 - alpha) * theta) / sinTheta;
        scale1 = sin(alpha * theta) / sinTheta;
    } else {        
        // rot0 and rot1 very close - just do linear interp and renormalize.
        scale0 = 1.0 - alpha;
        scale1 = alpha;
    }

    if (flip1)
        scale1 = -scale1;

    return scale0 * q0 + scale1 * q1;
}

PXR_NAMESPACE_CLOSE_SCOPE
