/***************************************************************************
                         qgscontinuouscolrenderer.cpp  -  description
                             -------------------
    begin                : Nov 2003
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
/* $Id$ */
#include "qgscontinuouscolrenderer.h"
#include "qgis.h"
#include "qgsvectorlayer.h"
#include <cfloat>
#include "qgslegenditem.h"
#include "qgscontcoldialog.h"

QgsContinuousColRenderer::~QgsContinuousColRenderer()
{
  if (m_minimumItem)
    {
      delete m_minimumItem;
    }
  if (m_maximumItem)
    {
      delete m_maximumItem;
    }
}

void QgsContinuousColRenderer::setMinimumItem(QgsRenderItem * it)
{
  if (m_minimumItem)
    {
      delete m_minimumItem;
    }
  m_minimumItem = it;
}

void QgsContinuousColRenderer::setMaximumItem(QgsRenderItem * it)
{
  if (m_maximumItem)
    {
      delete m_maximumItem;
    }
  m_maximumItem = it;
}

void QgsContinuousColRenderer::initializeSymbology(QgsVectorLayer * layer, QgsDlgVectorLayerProperties * pr)
{
  bool toproperties = false;    //if false: rendererDialog is associated with the vector layer and image is rendered, true: rendererDialog is associated with buffer dialog of vector layer properties and no image is rendered
  if (pr)
    {
      toproperties = true;
    }

  setClassificationField(0);    //the classification field does not matter

  if (layer)
    {
      QgsSymbol sy;
      sy.brush().setStyle(Qt::SolidPattern);
      sy.pen().setStyle(Qt::SolidLine);
      sy.pen().setWidth(1);

      //random fill colors for points and polygons and pen colors for lines
      int red = 1 + (int) (255.0 * rand() / (RAND_MAX + 1.0));
      int green = 1 + (int) (255.0 * rand() / (RAND_MAX + 1.0));
      int blue = 1 + (int) (255.0 * rand() / (RAND_MAX + 1.0));

      //font tor the legend text
      //TODO Make the font a user option

      QFont f("times", 12, QFont::Normal);
      QFontMetrics fm(f);

      QPixmap *pixmap;
      if (toproperties)
        {
          pixmap = pr->getBufferPixmap();
      } else
        {
          pixmap = layer->legendPixmap();
        }
      QString name = layer->name();
      int width = 40 + fm.width(layer->name());
      int height = (fm.height() + 10 > 35) ? fm.height() + 10 : 35;
      pixmap->resize(width, height);
      pixmap->fill();
      QPainter p(pixmap);

      if (layer->vectorType() == QGis::Line)
        {
          sy.pen().setColor(QColor(red, green, blue));
          //paint the pixmap for the legend
          p.setPen(sy.pen());
          p.drawLine(10, pixmap->height() - 25, 25, pixmap->height() - 10);
      } else
        {
          sy.brush().setColor(QColor(red, green, blue));
          sy.pen().setColor(QColor(0, 0, 0));
          //paint the pixmap for the legend
          p.setPen(sy.pen());
          p.setBrush(sy.brush());
          if (layer->vectorType() == QGis::Point)
            {
              p.drawRect(20, pixmap->height() - 17, 5, 5);
          } else                //polygon
            {
              p.drawRect(10, pixmap->height() - 25, 20, 15);
            }

        }

      p.setPen(Qt::black);
      p.setFont(f);
      p.drawText(35, pixmap->height() - 10, name);

      QgsRenderItem *QgsRenderItem1 = new QgsRenderItem(sy, QString::number(DBL_MIN, 'f', 6), "");
      QgsRenderItem *QgsRenderItem2 = new QgsRenderItem(sy, QString::number(DBL_MAX, 'f', 6), "");

      setMinimumItem(QgsRenderItem1);
      setMaximumItem(QgsRenderItem2);

      QgsContColDialog *dialog = new QgsContColDialog(layer);

      if (toproperties)
        {
          pr->setBufferDialog(dialog);
      } else
        {
          layer->setRendererDialog(dialog);
          QgsLegendItem *item;
          if (item = layer->legendItem())
            {
              item->setPixmap(0, (*pixmap));
            }
        }
  } else
    {
      qWarning("Warning, layer is null in QgsContinuousSymRenderer::initializeSymbology(..)");
    }
}

void QgsContinuousColRenderer::renderFeature(QPainter * p, QgsFeature * f)
{
  if ((m_minimumItem && m_maximumItem))
    {

      //first find out the value for the classification attribute
      std::vector < QgsFeatureAttribute > vec = f->attributeMap();
      //std::cout << "classification field: " << m_classificationField << std::endl << std::flush;
      double value = vec[m_classificationField].fieldValue().toDouble();
      //std::cout << "value: " << value << std::endl << std::flush;

      double fvalue = vec[m_classificationField].fieldValue().toDouble();
      double minvalue = m_minimumItem->value().toDouble();
      double maxvalue = m_maximumItem->value().toDouble();

      QColor mincolor, maxcolor;

      unsigned char *feature = f->getGeometry();
      int wkbType = (int) feature[1];

      if (wkbType == QGis::WKBLineString || wkbType == QGis::WKBMultiLineString)
        {
          mincolor = m_minimumItem->getSymbol()->pen().color();
          maxcolor = m_maximumItem->getSymbol()->pen().color();
      } else                    //if(point or polygon)
        {
          p->setPen(m_minimumItem->getSymbol()->pen());
          mincolor = m_minimumItem->getSymbol()->fillColor();
          maxcolor = m_maximumItem->getSymbol()->fillColor();
        }

      int red =
        int (maxcolor.red() * (fvalue - minvalue) / (maxvalue - minvalue) +
             mincolor.red() * (maxvalue - fvalue) / (maxvalue - minvalue));
      int green =
        int (maxcolor.green() * (fvalue - minvalue) / (maxvalue - minvalue) +
             mincolor.green() * (maxvalue - fvalue) / (maxvalue - minvalue));
      int blue =
        int (maxcolor.blue() * (fvalue - minvalue) / (maxvalue - minvalue) +
             mincolor.blue() * (maxvalue - fvalue) / (maxvalue - minvalue));

      if (wkbType == QGis::WKBLineString || wkbType == QGis::WKBMultiLineString)
        {
	    p->setPen(QPen(QColor(red, green, blue),m_minimumItem->getSymbol()->pen().width()));//make sure the correct line width is used
	} 
      else
        {
          p->setBrush(QColor(red, green, blue));
        }
    }
}
