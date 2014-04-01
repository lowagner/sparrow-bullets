#include "basic.h"

sbVector::sbVector()
{
    x=0; y=0; z=0;
}

sbVector::sbVector( int x_, int y_, int z_ )
{
    x=spIntToFixed(x_);
    y=spIntToFixed(y_);
    z=spIntToFixed(z_);
}

sbVector::sbVector( float x_, float y_, float z_ )
{
    x=spFloatToFixed(x_);
    y=spFloatToFixed(y_);
    z=spFloatToFixed(z_);
}

sbVector::sbVector( float x_, int y_, int z_ )
{
    x=spFloatToFixed(x_);
    y=spIntToFixed(y_);
    z=spIntToFixed(z_);
}

sbVector::sbVector( int x_, float y_, float z_ )
{
    x=spIntToFixed(x_);
    y=spFloatToFixed(y_);
    z=spFloatToFixed(z_);
}

sbVector::sbVector( int x_, float y_, int z_ )
{
    x=spIntToFixed(x_);
    y=spFloatToFixed(y_);
    z=spIntToFixed(z_);
}

sbVector::sbVector( float x_, int y_, float z_ )
{
    x=spFloatToFixed(x_);
    y=spIntToFixed(y_);
    z=spFloatToFixed(z_);
}

sbVector::sbVector( int x_, int y_, float z_ )
{
    x=spIntToFixed(x_);
    y=spIntToFixed(y_);
    z=spFloatToFixed(z_);
}

sbVector::sbVector( float x_, float y_, int z_ )
{
    x=spFloatToFixed(x_);
    y=spFloatToFixed(y_);
    z=spIntToFixed(z_);
}


sbVector::sbVector( btVector3 v )
{
    x=spFloatToFixed(v.x());
    y=spFloatToFixed(v.y());
    z=spFloatToFixed(v.z());
    
    //std::cout << " sbVector init z = " << v.z() << " => fp = " << z << std::endl;
    // all working as it should
}


sbVector&
sbVector::operator = ( const sbVector& rhs )
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    return *this;
}


sbVector&
sbVector::operator += ( const sbVector& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}


const sbVector
sbVector::operator + ( const sbVector& rhs )
const
{
    sbVector result = *this;
    result += rhs;
    return result;
}

sbVector&
sbVector::operator -= ( const sbVector& rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}


const sbVector
sbVector::operator - ( const sbVector& rhs )
const
{
    sbVector result = *this;
    result -= rhs;
    return result;
}


// multiplication

sbVector& 
sbVector::operator *= ( float& rhs )
{
    Sint32 rhsfp = spFloatToFixed( rhs );
    x = spMul(x, rhsfp);
    y = spMul(x, rhsfp);
    z = spMul(x, rhsfp);
}

const sbVector 
sbVector::operator * ( float& rhs ) 
const
{
    sbVector result = *this;
    result *= rhs;
    return result;
}


sbVector& 
sbVector::operator *= ( int& rhs )
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
}

const sbVector 
sbVector::operator * ( int& rhs ) 
const
{
    sbVector result = *this;
    result *= rhs;
    return result;
}


bool
sbVector::operator == ( const sbVector& rhs ) 
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
sbVector::operator != ( const sbVector& rhs ) 
const
{
    return !( *this == rhs );
}



sbVector 
operator * ( int scalar, const sbVector rhs )
{
    sbVector result = rhs;
    return result *= scalar;
}



std::ostream& operator << ( std::ostream& os, const sbVector& v )
{
    os << "< " << spFixedToFloat(v.x) << ", " 
               << spFixedToFloat(v.y) << ", " 
               << spFixedToFloat(v.z) << ">";
    return os;
}
