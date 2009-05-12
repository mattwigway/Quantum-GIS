
#ifndef QGSSTYLEV2_H
#define QGSSTYLEV2_H

#include <QMap>
#include <QString>

#include "qgssymbollayerv2utils.h" // QgsStringMap

class QgsSymbolV2;
class QgsSymbolLayerV2;
class QgsVectorColorRampV2;

class QDomDocument;
class QDomElement;

typedef QMap<QString, QgsSymbolV2* > QgsSymbolV2Map;
typedef QMap<QString, QgsVectorColorRampV2* > QgsVectorColorRampV2Map;

class QgsStyleV2
{
public:
  
  QgsStyleV2();
  ~QgsStyleV2();
  
  //! remove all contents of the style
  void clear();
  
  //! add symbol to style. takes symbol's ownership
  bool addSymbol(QString name, QgsSymbolV2* symbol);
  
  //! remove symbol from style (and delete it)
  bool removeSymbol(QString name);
  
  //! return a NEW copy of symbol
  QgsSymbolV2* symbol(QString name);
  
  //! return a const pointer to a symbol (doesn't create new instance)
  const QgsSymbolV2* symbolRef(QString name) const;
  
  //! return count of symbols in style
  int symbolCount();
  
  //! return a list of names of symbols
  QStringList symbolNames();
  
  
  //! add color ramp to style. takes ramp's ownership
  bool addColorRamp(QString name, QgsVectorColorRampV2* colorRamp);
  
  //! remove color ramp from style (and delete it)
  bool removeColorRamp(QString name);
  
  //! return a NEW copy of color ramp
  QgsVectorColorRampV2* colorRamp(QString name);
  
  //! return a const pointer to a symbol (doesn't create new instance)
  const QgsVectorColorRampV2* colorRampRef(QString name) const;
  
  //! return count of color ramps
  int colorRampCount();
  
  //! return a list of names of color ramps
  QStringList colorRampNames();
  
  
  //! load a file into the style
  bool load(QString filename);
  
  //! save style into a file
  bool save(QString filename);
  
  //! return last error from load/save operation
  QString errorString() { return mErrorString; }
  
protected:

  QgsStringMap parseProperties(QDomElement& element);
  void saveProperties(QgsStringMap props, QDomDocument& doc, QDomElement& element);
  
  QgsSymbolV2* loadSymbol(QDomElement& element);
  QgsSymbolLayerV2* loadSymbolLayer(QDomElement& element);
  QgsVectorColorRampV2* loadColorRamp(QDomElement& element);
  
  QDomElement saveSymbol(QString name, QgsSymbolV2* symbol, QDomDocument& doc, QgsSymbolV2Map* subSymbols = NULL);
  QDomElement saveColorRamp(QString name, QgsVectorColorRampV2* ramp, QDomDocument& doc);
  
  QgsSymbolV2Map mSymbols;
  QgsVectorColorRampV2Map mColorRamps;
  
  QString mErrorString;
};


#endif
