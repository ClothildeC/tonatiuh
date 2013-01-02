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

#include <QAbstractButton>
#include <QFileDialog>
#include <QDir>

#include "RandomDeviateFactory.h"
#include "RayTraceDialog.h"
#include "TPhotonMapFactory.h"

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the default values.
 */
RayTraceDialog::RayTraceDialog( QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f ), m_numRays( 0 ), m_selectedRandomFactory( -1 ), m_fraction( 0.0 ), m_drawPhotons( false ), m_selectedPhotonMapFactory( -1 ), m_increasePhotonMap( false )
{
	setupUi( this );
	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( applyChanges( QAbstractButton* ) ) );
}

/**
 * Creates a dialog to ray tracer options with the given \a parent and \a f flags.
 *
 * The variables take the values specified by \a numRats, \a faction, \a drawPhotons and \a increasePhotonMap.
 */
RayTraceDialog::RayTraceDialog( int numRays, QVector< RandomDeviateFactory* > randomFactoryList, double fraction,int widthDivisions, int heightDivisions, bool drawPhotons, QVector< TPhotonMapFactory* > photonMapFactoryList, int selectedRandomFactory, int selectedPhotonMapFactory, bool increasePhotonMap, QWidget * parent, Qt::WindowFlags f )
:QDialog ( parent, f ), m_numRays( numRays ), m_selectedRandomFactory(selectedRandomFactory), m_fraction( fraction ),m_widthDivisions(widthDivisions),m_heightDivisions(heightDivisions), m_drawPhotons( drawPhotons ), m_selectedPhotonMapFactory( selectedPhotonMapFactory ), m_increasePhotonMap( increasePhotonMap )
{
	setupUi( this );
	raysSpinBox->setValue( m_numRays );
	for( int index = 0; index < randomFactoryList.size(); ++index )
	{
		randomCombo->addItem( randomFactoryList[index]->RandomDeviateIcon(), randomFactoryList[index]->RandomDeviateName() );
	}
	if( m_selectedRandomFactory < 0 && randomFactoryList.size() > 0 )
		m_selectedRandomFactory =0;
	randomCombo->setCurrentIndex( m_selectedRandomFactory );

	drawSpin->setValue( m_fraction );
	widthDivisionsSpinBox->setValue( m_widthDivisions );
	heightDivisionsSpinBox->setValue( m_heightDivisions );
	photonsCheck->setChecked( m_drawPhotons );

	for( int index = 0; index < photonMapFactoryList.size(); ++index )
	{
		photonmapTypeCombo->addItem( photonMapFactoryList[index]->TPhotonMapIcon(), photonMapFactoryList[index]->TPhotonMapName() );
	}
	if( ( m_selectedPhotonMapFactory < 0  ) & ( photonMapFactoryList.size() > 0 ) ) m_selectedPhotonMapFactory = 0;
	photonmapTypeCombo->setCurrentIndex( m_selectedPhotonMapFactory );

	if ( m_increasePhotonMap )
		increaseMapRadio->setChecked( true );
	else
		newMapRadio->setChecked( true );

	connect( this, SIGNAL( accepted() ), this, SLOT( saveChanges() ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( applyChanges( QAbstractButton* ) ) );
}

/*!
 * Destroys the RayTraceDialog object.
 */
RayTraceDialog::~RayTraceDialog()
{
}

/**
 * Returns the number of rays to trace.
 */
int RayTraceDialog::GetNumRays() const
{
	return m_numRays;
}

/**
 * Returns the fraction of trace rays to draw.
 */
double RayTraceDialog::GetRaysFactionToDraw() const
{
	return m_fraction;
};

/**
 * Returns the the width divisions applied to the sun shape.
 */
int RayTraceDialog::GetWidthDivisions() const
{
	return m_widthDivisions;
};

/**
 * Returns the the width divisions applied to the sun shape.
 */
int RayTraceDialog::GetHeightDivisions() const
{
	return m_heightDivisions;
};

/**
 * Returns the factory to create a new random generator.
 */
int RayTraceDialog::GetRandomDeviateFactoryIndex() const
{
	return m_selectedRandomFactory;
}


/**
 * Returns if the photons are going to be represented.
 */
bool RayTraceDialog::DrawPhotons() const
{
	return m_drawPhotons;
}

/**
 * Returns the factory to create a new photon map.
 */
int RayTraceDialog::GetPhotonMapFactoryIndex() const
{
	return m_selectedPhotonMapFactory;
}

/**
 * Returns if the the tracer use the same photon map used in the previous tracer process.
 */
bool RayTraceDialog::IncreasePhotonMap() const
{
	return m_increasePhotonMap;
}

/**
 * If the applyChanges button is clicked the dialog values are saved.
 */
void RayTraceDialog::applyChanges( QAbstractButton* button  )
{
	if( buttonBox->buttonRole( button ) == QDialogButtonBox::ApplyRole )
		saveChanges();
}

/**
 * Saves the values of the dialog.
 */
void RayTraceDialog::saveChanges()
{
	m_numRays = raysSpinBox->value();
	m_selectedRandomFactory = randomCombo->currentIndex();

	m_fraction = drawSpin->value();
	m_widthDivisions= widthDivisionsSpinBox->value();
	m_heightDivisions= heightDivisionsSpinBox->value();
	m_drawPhotons = photonsCheck->isChecked();

	m_selectedPhotonMapFactory = photonmapTypeCombo->currentIndex();
	if( newMapRadio->isChecked() )
		m_increasePhotonMap = false;
	else
		m_increasePhotonMap = true;
}