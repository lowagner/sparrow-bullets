#include "basic.h"

sbVector3::sbVector3( int x_, int y_, int z_ )
{
    x=spIntToFixed(x_);
    y=spIntToFixed(y_);
    z=spIntToFixed(z_);
}

sbVector3::sbVector3( float x_, float y_, float z_ )
{
    x=spFloatToFixed(x_);
    y=spFloatToFixed(y_);
    z=spFloatToFixed(z_);
}


sbVector3::sbVector3( btVector3 v )
{
    x=v.x();
    y=v.y();
    z=v.z();
}


sbVector3&
sbVector3::operator = ( const sbVector3& rhs )
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    return *this;
}


sbVector3&
sbVector3::operator += ( const sbVector3& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}


const sbVector3
sbVector3::operator + ( const sbVector3& rhs )
const
{
    sbVector3 result = *this;
    result += rhs;
    return result;
}

sbVector3&
sbVector3::operator -= ( const sbVector3& rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}


const sbVector3
sbVector3::operator - ( const sbVector3& rhs )
const
{
    sbVector3 result = *this;
    result -= rhs;
    return result;
}


// multiplication

sbVector3& 
sbVector3::operator *= ( float& rhs )
{
    Sint32 rhsfp = spFloatToFixed( rhs );
    x *= rhsfp;
    y *= rhsfp;
    z *= rhsfp;
}

const sbVector3 
sbVector3::operator * ( float& rhs ) 
const
{
    sbVector3 result = *this;
    result *= rhs;
    return result;
}


sbVector3& 
sbVector3::operator *= ( int& rhs )
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
}

const sbVector3 
sbVector3::operator * ( int& rhs ) 
const
{
    sbVector3 result = *this;
    result *= rhs;
    return result;
}


bool
sbVector3::operator == ( const sbVector3& rhs ) 
const
{
    if ( x != rhs.x )
        return false;
    if ( y != rhs.y )
        return false;
    if ( z != rhs.z )
        return false;
    return true;
}

bool
sbVector3::operator != ( const sbVector3& rhs ) 
const
{
    return !( *this == rhs );
}



sbVector3 
operator * ( int scalar, const sbVector3 rhs )
{
    sbVector3 result = rhs;
    return result *= scalar;
}



std::ostream& operator << ( std::ostream& os, const sbVector3& v )
{
    os << "< " << spFixedToFloat(v.x) << ", " 
               << spFixedToFloat(v.y) << ", " 
               << spFixedToFloat(v.z) << ">";
    return os;
}
