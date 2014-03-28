#ifndef __BASIC__
#define __BASIC__

#include "btBulletDynamicsCommon.h"
#include "csparrow.h"
#include <iostream>

class sbVector3
{
public:
    Sint32 x, y, z;

    // conversions from all the different things you could put in
    sbVector3( int x_=0, int y_=0, int z_=0 );
    sbVector3( float x_, float y_, float z_ );
    sbVector3( btVector3 v );

    // assignment operator
    sbVector3& operator = ( const sbVector3 &rhs );

    // addition
    sbVector3& operator += ( const sbVector3 &rhs );
    const sbVector3 operator + ( const sbVector3 &rhs ) const;

    // subtraction
    sbVector3& operator -= ( const sbVector3 &rhs );
    const sbVector3 operator - ( const sbVector3 &rhs ) const;
   
    // multiplication
//    sbVector3& operator *= ( const sbVector3 &rhs );
//    const sbVector3 operator * ( const sbVector3 &rhs ) const;
    sbVector3& operator *= ( float& rhs );
    const sbVector3 operator * ( float& rhs ) const;
    sbVector3& operator *= ( int& rhs );
    const sbVector3 operator * ( int& rhs ) const;

    // equality checks
    bool operator == ( const sbVector3& rhs ) const;
    bool operator != ( const sbVector3& rhs ) const;

    friend std::ostream& operator << ( std::ostream&, const sbVector3& v );
};


sbVector3 operator * ( int scalar, sbVector3 rhs );


#endif
