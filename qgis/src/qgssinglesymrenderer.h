/***************************************************************************
                         qgssinglesymrenderer.h  -  description
                             -------------------
    begin                : Oct 2003
    copyright            : (C) 2003 by Marco Hugentobler
    email                : mhugent@geo.unizh.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id */

#ifndef QGSSINGLESYMRENDERER_H
#define QGSSINGLESYMRENDERER_H

#include "qgsrenderer.h"
#include "qgsrenderitem.h"
#include "qgspoint.h"
#include "qpainter.h"
#include "qgscoordinatetransform.h"
#include "qgsdlgvectorlayerproperties.h"
class QgsFeature;

/**Render class to display all the features with a single QgsSymbol*/
class QgsSingleSymRenderer: public QgsRenderer
{
 public:
    QgsSingleSymRenderer();
    ~QgsSingleSymRenderer();
    /**Replaces the current mItem by ri*/
    void addItem(QgsRenderItem* ri);
    /**Returns a pointer to mItem*/
    QgsRenderItem* item();
    /**Renders an OGRFeature*/
    void renderFeature(QPainter* p, QgsFeature* f, QPicture* pic, double* scalefactor);
    /**Sets the initial symbology configuration for a layer. Besides of applying default symbology settings, an instance of the corresponding renderer dialog is created and associated with the layer (or with the property dialog, if pr is not 0). Finally, a pixmap for the legend is drawn (or, if pr is not 0, it is stored in the property dialog, until the settings are applied).
       @param layer the vector layer associated with the renderer
       @param pr the property dialog. This is only needed if the renderer is created from the property dialog and not yet associated with the vector layer, otherwise 0*/
    virtual void initializeSymbology(QgsVectorLayer* layer, QgsDlgVectorLayerProperties* pr=0);
    /**Returns false, no attributes neede for single symbol*/
    bool needsAttributes();
 protected:
    QgsRenderItem* mItem;
};

inline QgsRenderItem* QgsSingleSymRenderer::item()
{
    return mItem;
}
inline bool QgsSingleSymRenderer::needsAttributes(){
  return false;
}
#endif
