/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>
#include <iostream>

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeHyperboloid.h"
#include "tgf.h"
#include "tgc.h"
#include "Trace.h"
#include "Vector3D.h"

using tgc::Pi;
using tgc::TwoPi;

SO_NODE_SOURCE(ShapeHyperboloid);

void ShapeHyperboloid::initClass()
{
	Trace trace( "ShapeHyperboloid::initClass", false );
	SO_NODE_INIT_CLASS(ShapeHyperboloid, TShape, "TShape");
}

ShapeHyperboloid::ShapeHyperboloid( )
{
	Trace trace( "ShapeHyperboloid::ShapeHyperboloid", false );

	SO_NODE_CONSTRUCTOR(ShapeHyperboloid);
	SO_NODE_ADD_FIELD(a, (3.0) );
	SO_NODE_ADD_FIELD(b, (2.0) );
	SO_NODE_ADD_FIELD(diameter, (4.0) );
}

ShapeHyperboloid::~ShapeHyperboloid()
{
	Trace trace( "ShapeHyperboloid::~ShapeHyperboloid", false );
}

double ShapeHyperboloid::GetArea() const
{
	Trace trace( "ShapeHyperboloid::getIcon", false );
	return -1.0;

}

QString ShapeHyperboloid::getIcon()
{
	Trace trace( "ShapeHyperboloid::getIcon", false );
	return ":/icons/shapehyperboloid.png";
}

bool ShapeHyperboloid::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	Trace trace( "ShapeHyperboloid::Intersect", false );

	double xo= objectRay.origin.x;
	double yo= objectRay.origin.y;
	double zo= objectRay.origin.z;

	double xd= objectRay.direction.x;
	double yd= objectRay.direction.y;
	double zd= objectRay.direction.z;

	double t0 = ( 1 / ( b.getValue()* b.getValue() *  yd * yd - a.getValue() * a.getValue() * ( xd * xd  + zd * zd) ) )
					* ( a.getValue() *  b.getValue() * b.getValue() *  yd - b.getValue() * b.getValue() * yd * yo
							+ a.getValue() * a.getValue() * (xd * xo + zd * zo )
							- 0.5 * sqrt( 4 * ( a.getValue() * b.getValue() * b.getValue()*  yd
												- b.getValue()* b.getValue() * yd * yo
												+ a.getValue()* a.getValue() * (xd * xo + zd * zo ) )
											* ( a.getValue() * b.getValue() * b.getValue()*  yd
												- b.getValue()* b.getValue() * yd * yo
												+ a.getValue()* a.getValue() * (xd * xo + zd * zo ) )
										- 4  * ( -b.getValue() * b.getValue() * yd * yd + a.getValue() * a.getValue() * ( xd * xd + zd * zd ) )
										* ( 2 * a.getValue() * b.getValue() * b.getValue() * yo - b.getValue() * b.getValue() * yo*  yo
												+ a.getValue() * a.getValue() *  (xo * xo + zo * zo ) ) ) );
	double t1 = ( 1 / ( b.getValue()* b.getValue() *  yd * yd - a.getValue() * a.getValue() * ( xd * xd  + zd * zd) ) )
						* ( a.getValue() *  b.getValue() * b.getValue() *  yd - b.getValue() * b.getValue() * yd * yo
								+ a.getValue() * a.getValue() * (xd * xo + zd * zo )
								+ 0.5 * sqrt( 4 * ( a.getValue() * b.getValue() * b.getValue()*  yd
													- b.getValue()* b.getValue() * yd * yo
													+ a.getValue()* a.getValue() * (xd * xo + zd * zo ) )
												* ( a.getValue() * b.getValue() * b.getValue()*  yd
													- b.getValue()* b.getValue() * yd * yo
													+ a.getValue()* a.getValue() * (xd * xo + zd * zo ) )
											- 4  * ( -b.getValue() * b.getValue() * yd * yd + a.getValue() * a.getValue() * ( xd * xd + zd * zd ) )
											* ( 2 * a.getValue() * b.getValue() * b.getValue() * yo - b.getValue() * b.getValue() * yo*  yo
													+ a.getValue() * a.getValue() *  (xo * xo + zo * zo ) ) ) );
	// Compute intersection distance along ray
	if( t0 > objectRay.maxt || t1 < objectRay.mint ) return false;
	double thit = ( t0 > objectRay.mint )? t0 : t1 ;
	if( thit > objectRay.maxt ) return false;

   //Compute possible cylinder hit position and $\phi
	Point3D hitPoint = objectRay( thit );
	double phi = atan2( hitPoint.y, hitPoint.x );
	if ( phi < 0. ) phi += TwoPi;

	//Evaluate Tolerance
	double tol = 0.00001;
	if( (thit - objectRay.mint) < tol ) return false;

	double ymin = a.getValue() + ( sqrt( a.getValue() * a.getValue() * b.getValue() * b.getValue() * b.getValue() * b.getValue() ) / ( b.getValue() * b.getValue() ) );
	double r = diameter.getValue() / 2;
	double ymax = ( a.getValue() * b.getValue() * b.getValue()
						+ sqrt(  a.getValue() * a.getValue() * b.getValue() * b.getValue()
									*  ( b.getValue() * b.getValue() +  r * r  ) ) )
				/ ( b.getValue() * b.getValue() );


	// Test intersection against clipping parameters

	double yradius = hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z;
	if( hitPoint.y < ymin || hitPoint.y > ymax ||  yradius > r )
	{
		if ( thit == t1 ) return false;
		if ( t1 > objectRay.maxt ) return false;
		thit = t1;

		hitPoint = objectRay( thit );
		yradius = hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z;
		if( hitPoint.y < ymin || hitPoint.y > ymax ||  yradius > r ) return false;
	}
	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );


	// Find parametric representation of hyperbola hit
	double u = yradius / ( diameter.getValue() / 2 );
	double v = acos( hitPoint.x / yradius );

	Vector3D dpdu = Dpdu( u, v );
	Vector3D dpdv = Dpdv( u, v );

	// Compute cylinder \dndu and \dndv
	Vector3D d2Pduu( 0.0,
					(2 * pow( a.getValue(), 4) * pow( b.getValue(), 4 ) *  diameter.getValue() *  diameter.getValue() )
					/ ( pow( a.getValue(), 2 )* pow( b.getValue(), 2)
							* pow( 4 *  b.getValue() * b.getValue() + diameter.getValue() * diameter.getValue() * u * u , 3 / 2.0 ) ),
					0.0 );

	Vector3D d2Pduv( - diameter.getValue() *tgc::Pi * sin( tgc::TwoPi * v ),
					0.0,
					diameter.getValue() *tgc::Pi * cos( tgc::TwoPi * v ) );
	Vector3D d2Pdvv( -2.0 * diameter.getValue() * tgc::Pi * tgc::Pi * u * cos( tgc::TwoPi * v),
					0.0,
					-2.0 * diameter.getValue() * tgc::Pi * tgc::Pi * u * sin( tgc::TwoPi * v ) );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );

	Vector3D N = Normalize( NormalVector( CrossProduct( dpdv, dpdu ) ) );


	double e = DotProduct( N, d2Pduu );
	double f = DotProduct( N, d2Pduv );
	double g = DotProduct( N, d2Pdvv );

		// Compute \dndu and \dndv from fundamental form coefficients
	double invEGF2 = 1.0 / (E*G - F*F);
	Vector3D dndu = (f*F - e*G) * invEGF2 * dpdu +
					(e*F - f*E) * invEGF2 * dpdv;
	Vector3D dndv = (g*F - f*G) * invEGF2 * dpdu +
					(f*F - g*E) * invEGF2 * dpdv;

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
								dpdu,
								dpdv,
								dndu,
								dndv,
								u, v, this );

	// Update _tHit_ for quadric intersection
	*tHit = thit;

	return true;

}

bool ShapeHyperboloid::IntersectP( const Ray& worldRay ) const
{
	Trace trace( "ShapeHyperboloid::IntersectP", false );

	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeHyperboloid::Sample( double u, double v ) const
{
	Trace trace( "ShapeHyperboloid::Sample", false );

	return GetPoint3D( u, v );
}

bool ShapeHyperboloid::OutOfRange( double u, double v ) const
{
	Trace trace( "ShapeHyperboloid::OutOfRange", false );

	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

Point3D ShapeHyperboloid::GetPoint3D (double u, double v) const
{
	Trace trace( "ShapeHyperboloid::GetPoint3D", false );

	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Function Poligon::GetPoint3D called with invalid parameters" );


	double r0 = ( diameter.getValue() / 2 ) * u;
	double phi0 = tgc::TwoPi * v;

	double x = cos(phi0 )* r0;
	double z = sin( phi0 ) * r0;
	double y = a.getValue()+ ( sqrt( a.getValue() * a.getValue() * b.getValue() * b.getValue()
						*  ( b.getValue() * b.getValue()  +  x* x + z * z ) )
			/ ( b.getValue() * b.getValue() ) );
	return Point3D( x, y, z );
}

NormalVector ShapeHyperboloid::GetNormal (double u, double v) const
{
	Trace trace( "ShapeHyperboloid::GetNormal", false );

	Vector3D dpdu = Vector3D( 0.5 * diameter.getValue() * cos( tgc::TwoPi *  v ),
			( pow( a.getValue(), 2 ) * pow( diameter.getValue(), 2 ) * u )
				/ ( 2 * sqrt( pow( a.getValue(), 2 ) * pow( b.getValue(), 2 ) *
						( 4 * pow( b.getValue(), 2 )  +  ( pow( diameter.getValue(), 2 ) * u * u ) ) ) ),
			0.5 * diameter.getValue() * sin( tgc::TwoPi * v ) );
	Vector3D dpdv = Vector3D( - diameter.getValue() * Pi * u * sin( tgc::TwoPi * v ),
			0.0,
			diameter.getValue() * Pi * u * cos( tgc::TwoPi * v ) );
	return NormalVector( Normalize( CrossProduct( dpdu, dpdv ) ) );
}

void ShapeHyperboloid::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/ )
{
	Trace trace( "ShapeHyperboloid::computeBBox", false );

	double xmin = - ( diameter.getValue() / 2 );
	double xmax = diameter.getValue() / 2;
	double ymin = a.getValue() + ( sqrt( a.getValue() * a.getValue() * b.getValue() * b.getValue() * b.getValue() * b.getValue() ) / ( b.getValue() * b.getValue() ) );
	double r = diameter.getValue() / 2;
	double ymax = ( a.getValue() * b.getValue() * b.getValue()
						+ sqrt(  a.getValue() * a.getValue() * b.getValue() * b.getValue()
									*  ( b.getValue() * b.getValue() + r * r  ) ) )
				/ ( b.getValue() * b.getValue() );

	double zmin = - ( diameter.getValue() / 2 );
	double zmax = diameter.getValue() / 2;
	box.setBounds( SbVec3f( xmin, ymin, zmin ), SbVec3f( xmax, ymax, zmax ) );
}

void ShapeHyperboloid::generatePrimitives(SoAction *action)
{
	Trace trace( "ShapeHyperboloid::generatePrimitives", false );

    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;

    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	const int rows = 30; // Number of points per row
    const int columns = 30; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for (int i = 0; i < rows; i++)
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; j++ )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal = GetNormal(ui, vj);

    		vertex[h][0] = point.x;
    		vertex[h][1] = point.y;
    		vertex[h][2] = point.z;
    		vertex[h][3] = normal.x;
    		vertex[h][4] = normal.y;
    		vertex[h][5] = normal.z;

    		pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point.

    	}
    }

	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices];
    int k = 0;
    for(int irow = 0; irow < (rows-1); irow++)
           for(int icolumn = 0; icolumn < (columns-1); icolumn++)
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices;ivert++)
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];
    	finalvertex[ivert][3] = vertex[indices[ivert]][3];
    	finalvertex[ivert][4] = vertex[indices[ivert]][4];
    	finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }
    delete[] indices;

    float u = 1;
    float v = 1;

	beginShape(action, QUADS );
    for( int i = 0; i < totalIndices; i++ )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
    }
    endShape();
}

Vector3D ShapeHyperboloid::Dpdu( double u, double v ) const
{
	Trace trace( "ShapeHyperboloid::Dpdu", false );

	Vector3D dpdu = Vector3D( 0.5 * diameter.getValue() * cos( tgc::TwoPi *  v ),
			( pow( a.getValue(), 2 ) * pow( diameter.getValue(), 2 ) * u )
				/ ( 2 * sqrt( pow( a.getValue(), 2 ) * pow( b.getValue(), 2 ) *
						( 4 * pow( b.getValue(), 2 )  +  ( pow( diameter.getValue(), 2 ) * u * u ) ) ) ),
			0.5 * diameter.getValue() * sin( tgc::TwoPi * v ) );
	return dpdu;
}

Vector3D ShapeHyperboloid::Dpdv( double u, double v ) const
{
	Trace trace( "ShapeHyperboloid::Dpdv", false );
	Vector3D dpdv = Vector3D( - diameter.getValue() * Pi * u * sin( tgc::TwoPi * v ),
			0.0,
			diameter.getValue() * Pi * u * cos( tgc::TwoPi * v ) );
	return dpdv;
}
