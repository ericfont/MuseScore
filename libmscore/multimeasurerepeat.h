//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __MULTI_MEASURE_REPEAT_H__
#define __MULTI_MEASURE_REPEAT_H__

#include "textlinebase.h"

namespace Ms {

class MultiMeasureRepeat;

//---------------------------------------------------------
//   @@ MultiMeasureRepeatSegment
//---------------------------------------------------------

class MultiMeasureRepeatSegment : public LineSegment {
      Q_OBJECT

   protected:

   public:
      MultiMeasureRepeatSegment(Score* s) : LineSegment(s) {}
      MultiMeasureRepeat* multiMeasureRepeat() const             { return (MultiMeasureRepeat*)spanner(); }
      virtual Element::Type type() const override                { return Element::Type::MULTI_MEASURE_REPEAT_SEGMENT; }
      virtual MultiMeasureRepeatSegment* clone() const override  { return new MultiMeasureRepeatSegment(*this); }
      virtual void draw(QPainter*) const override;
      virtual void layout() override;

      virtual QVariant getProperty(P_ID id) const override;
      virtual bool setProperty(P_ID propertyId, const QVariant&) override;
      virtual QVariant propertyDefault(P_ID id) const override;

      virtual void resetProperty(P_ID id) override;
      virtual void styleChanged() override;

      friend class MultiMeasureRepeat;
      };

//---------------------------------------------------------
//   @@ MultiMeasureRepeat
//---------------------------------------------------------

class MultiMeasureRepeat : public SLine {
      Q_OBJECT

      PropertyStyle lineWidthStyle;
      PropertyStyle lineStyleStyle;

      Q_PROPERTY(bool showNumberOfMeasures                     READ showNumberOfMeasures WRITE undoSetShowNumberOfMeasures)

      bool _showNumberOfMeasures;
      int _numberOfMeasures;

   public:
      MultiMeasureRepeat(Score* s);

      // overriden inherited methods
      virtual MultiMeasureRepeat* clone() const override       { return new MultiMeasureRepeat(*this); }
      virtual Element::Type type() const override { return Element::Type::MULTI_MEASURE_REPEAT; }
      virtual LineSegment* createLineSegment();
      virtual void layout() override;
      virtual void write(Xml&) const override;
      virtual void read(XmlReader&) override;

      // MultiMeasureRepeat-specific methods
      bool showNumberOfMeasures() const               { return _showNumberOfMeasures;     }
      void setShowNumberOfMeasures(bool v)            { _showNumberOfMeasures = v;        }
      int numberOfMeasures() const               { return _numberOfMeasures;     }
      void increaseNumberOfMeasures()            { _numberOfMeasures++;        }
      void decreaseNumberOfMeasures()            { _numberOfMeasures--;        }

      void undoSetShowNumberOfMeasures(bool);

      // property methods
      virtual StyleIdx getPropertyStyle(P_ID) const override;
      virtual bool setProperty(P_ID, const QVariant&) override;
      virtual QVariant propertyDefault(P_ID) const override;
      virtual PropertyStyle propertyStyle(P_ID) const override;
      virtual void resetProperty(P_ID) override;
      virtual void styleChanged() override;
      };

}     // namespace Ms
#endif

