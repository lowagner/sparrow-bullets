#ifndef __BASIC__
#define __BASIC__

#include "btBulletDynamicsCommon.h"
#include "../csparrow.h"
#include <iostream>

class sbVector
{
public:
    Sint32 x, y, z;

    // conversions from all the different things you could put in
    sbVector(); // default constructor = zero vector
    sbVector( int x_, int y_, int z_ );
    sbVector( float x_, float y_, float z_ );
    sbVector( float x_, int y_, int z_ );
    sbVector( int x_, float y_, float z_ );
    sbVector( int x_, float y_, int z_ );
    sbVector( float x_, int y_, float z_ );
    sbVector( int x_, int y_, float z_ );
    sbVector( float x_, float y_, int z_ );
    sbVector( btVector3 v );

    // assignment operator
    sbVector& operator = ( const sbVector &rhs );

    // addition
    sbVector& operator += ( const sbVector &rhs );
    const sbVector operator + ( const sbVector &rhs ) const;

    // subtraction
    sbVector& operator -= ( const sbVector &rhs );
    const sbVector operator - ( const sbVector &rhs ) const;
   
    // multiplication
//    sbVector& operator *= ( const sbVector &rhs );
//    const sbVector operator * ( const sbVector &rhs ) const;
    sbVector& operator *= ( float& rhs );
    const sbVector operator * ( float& rhs ) const;
    sbVector& operator *= ( int& rhs );
    const sbVector operator * ( int& rhs ) const;

    // equality checks
    bool operator == ( const sbVector& rhs ) const;
    bool operator != ( const sbVector& rhs ) const;

    friend std::ostream& operator << ( std::ostream&, const sbVector& v );
};


sbVector operator * ( int scalar, sbVector rhs );


#endif
