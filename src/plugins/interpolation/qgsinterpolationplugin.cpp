/***************************************************************************
                              qgsinterpolationplugin.cpp
                              --------------------------
  begin                : Marco 10, 2008
  copyright            : (C) 2008 by Marco Hugentobler
  email                : marco dot hugentobler at karto dot baug dot ethz dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsinterpolationplugin.h"
#include "qgisinterface.h"
#include "qgsinterpolationdialog.h"

static const QString name_ = QObject::tr( "Interpolation plugin" );
static const QString description_ = QObject::tr( "A plugin for interpolation based on vertices of a vector layer" );
static const QString version_ = QObject::tr( "Version 0.001" );

QgsInterpolationPlugin::QgsInterpolationPlugin( QgisInterface* iface ): mIface( iface ), mInterpolationAction( 0 )
{

}

QgsInterpolationPlugin::~QgsInterpolationPlugin()
{

}

void QgsInterpolationPlugin::initGui()
{
  if ( mIface )
  {
    mInterpolationAction = new QAction( QIcon( ":/interpolator/interpolation.png"), tr( "&Interpolation" ), 0 );
    QObject::connect( mInterpolationAction, SIGNAL( triggered() ), this, SLOT( showInterpolationDialog() ) );
    mIface->addToolBarIcon( mInterpolationAction );
    mIface->addPluginMenu( tr( "&Interpolation" ), mInterpolationAction );
  }
}

void QgsInterpolationPlugin::unload()
{
  mIface->removePluginMenu( tr( "&Interpolation" ), mInterpolationAction );
  mIface->removeToolBarIcon( mInterpolationAction );
  delete mInterpolationAction;
}

void QgsInterpolationPlugin::showInterpolationDialog()
{
  QgsInterpolationDialog dialog( 0, mIface );
  dialog.exec();
}

QGISEXTERN QgisPlugin * classFactory( QgisInterface * theQgisInterfacePointer )
{
  return new QgsInterpolationPlugin( theQgisInterfacePointer );
}

QGISEXTERN QString name()
{
  return name_;
}

QGISEXTERN QString description()
{
  return description_;
}

QGISEXTERN QString version()
{
  return version_;
}

QGISEXTERN int type()
{
  return QgisPlugin::UI;
}

QGISEXTERN void unload( QgisPlugin* theInterpolationPluginPointer )
{
  delete theInterpolationPluginPointer;
}



