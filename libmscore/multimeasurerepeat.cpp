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

#include "multimeasurerepeat.h"
#include "sym.h"
#include "xml.h"
#include "system.h"
#include "measure.h"
#include "staff.h"
#include "chordrest.h"

#include "score.h"

namespace Ms {

//=========================================================
//   MultiMeasureRepeatSegment
//=========================================================

//---------------------------------------------------------
//   layout
//---------------------------------------------------------

void MultiMeasureRepeatSegment::layout()
      {
      if (staff())
            setMag(staff()->mag());
      QRectF r = QRectF(0.0, 0.0, pos2().x(), pos2().y()).normalized();
      qreal lw = spatium() * score()->styleP(StyleIdx::multiMeasureRepeatLineWidth) * .5;
      setbbox(r.adjusted(-lw, -lw, lw, lw));
      adjustReadPos();
      }

//---------------------------------------------------------
//   draw
//---------------------------------------------------------

void MultiMeasureRepeatSegment::draw(QPainter* painter) const
      {
      painter->save();
      qreal _spatium = spatium();

      QPen pen(multiMeasureRepeat()->curColor());
      pen.setWidthF(multiMeasureRepeat()->lineWidth().val() * spatium());
      pen.setCapStyle(Qt::RoundCap);
      painter->setPen(pen);

      qreal midx = pos().x() + (pos2().x() - pos().x()) / 2.0;
      qreal midy = pos().y() + (pos2().y() - pos().y()) / 2.0;

      // dots at far left and at far right of the line, about twice as big as regular repeat dot
      painter->drawEllipse(QPointF(pos().x(), pos().y() - _spatium), _spatium / 4.0, _spatium / 4.0);
      painter->drawEllipse(QPointF(pos2().x(), pos2().y() + _spatium), _spatium / 4.0, _spatium / 4.0);

      //drawSymbol(SymId::repeatDot, painter, QPointF(pos().x(), pos().y() - _spatium), .1);
      //drawSymbol(SymId::repeatDot, painter, QPointF(pos2().x(), pos2().y() + _spatium * 2.0), .1);

      // rotate painter so that the line become horizontal
      qreal w     = pos2().x();
      qreal h     = pos2().y();
      qreal l     = sqrt(w * w + h * h);
      qreal wi = asin(-h / l) * 180.0 / M_PI;
      qreal scale = painter->worldTransform().m11();
      painter->rotate(-wi);
      painter->drawLine(QLineF(0.0, 0.0, l, 0.0));

      // the text for number of measures
      if (multiMeasureRepeat()->showNumberOfMeasures()) {
            const TextStyle& st = score()->textStyle(TextStyleType::MULTI_MEASURE_REPEAT);
            const QString text = QString::number(multiMeasureRepeat()->numberOfMeasures());
            QRectF r = st.fontMetrics(_spatium).boundingRect(text);
            // if text longer than available space, skip it
            if (r.width() < l) {
                  qreal yOffset = r.height() + r.y() +  _spatium * 0.1;       // find text descender height
                  painter->setFont(st.font(_spatium * MScore::pixelRatio));
                  painter->drawText(QPointF(midx, -yOffset), text);
                  }
            }

      painter->restore();
      }

//---------------------------------------------------------
//   getProperty
//---------------------------------------------------------

QVariant MultiMeasureRepeatSegment::getProperty(P_ID id) const
      {
      switch (id) {
            // route properties of the whole Glissando element to it
            case P_ID::LINE_WIDTH:
            case P_ID::LINE_STYLE:
            case P_ID::TEXT_STYLE_TYPE:
                  return multiMeasureRepeat()->getProperty(id);
            default:
                  return LineSegment::getProperty(id);
            }
      }

//---------------------------------------------------------
//   setProperty
//---------------------------------------------------------

bool MultiMeasureRepeatSegment::setProperty(P_ID id, const QVariant& v)
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
            case P_ID::LINE_STYLE:
            case P_ID::TEXT_STYLE_TYPE:
                  return multiMeasureRepeat()->setProperty(id, v);
            default:
                  return LineSegment::setProperty(id, v);
            }
      }

//---------------------------------------------------------
//   propertyDefault
//---------------------------------------------------------

QVariant MultiMeasureRepeatSegment::propertyDefault(P_ID id) const
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
            case P_ID::LINE_STYLE:
            case P_ID::TEXT_STYLE_TYPE:
                  return multiMeasureRepeat()->propertyDefault(id);
            default:
                  return LineSegment::propertyDefault(id);
            }
      }

//---------------------------------------------------------
//   resetProperty
//---------------------------------------------------------

void MultiMeasureRepeatSegment::resetProperty(P_ID id)
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
            case P_ID::LINE_STYLE:
                  return multiMeasureRepeat()->resetProperty(id);

            default:
                  return LineSegment::resetProperty(id);
            }
      }

//---------------------------------------------------------
//   styleChanged
//---------------------------------------------------------

void MultiMeasureRepeatSegment::styleChanged()
      {
      multiMeasureRepeat()->styleChanged();
      }

//=========================================================
//   MultiMeasureRepeat
//=========================================================

MultiMeasureRepeat::MultiMeasureRepeat(Score* s)
   : SLine(s)
      {
      setFlags(ElementFlag::MOVABLE | ElementFlag::SELECTABLE);

      _showNumberOfMeasures      = true;
      _numberOfMeasures = 1;

      setDiagonal(true);
      setLineWidth(score()->styleS(StyleIdx::multiMeasureRepeatLineWidth));
      lineWidthStyle = PropertyStyle::STYLED;
      setLineStyle(Qt::PenStyle(score()->styleI(StyleIdx::multiMeasureRepeatLineStyle)));
      lineStyleStyle = PropertyStyle::STYLED;

      setAnchor(Spanner::Anchor::MEASURE);
      }

//---------------------------------------------------------
//   createLineSegment
//---------------------------------------------------------

LineSegment* MultiMeasureRepeat::createLineSegment()
      {
      LineSegment* seg = new MultiMeasureRepeatSegment(score());
      seg->setTrack(track());
      seg->setColor(color());
      return seg;
      }

//---------------------------------------------------------
//   read
//---------------------------------------------------------

void MultiMeasureRepeat::read(XmlReader& e)
      {
      //todo copy from gliassando or somethinge else
      }

//---------------------------------------------------------
//   write
//---------------------------------------------------------

void MultiMeasureRepeat::write(Xml& e) const
     {
      // todo copy from glissando or somehting else
      }

//---------------------------------------------------------
//   undoSetShowNumberOfMeasures
//---------------------------------------------------------

void MultiMeasureRepeat::undoSetShowNumberOfMeasures(bool f)
      {
      undoChangeProperty(P_ID::MULTI_MEASURE_REPEAT_SHOW_NUMBER_OF_MEASURES, f);
      }

//---------------------------------------------------------
//   layout
//---------------------------------------------------------

static const qreal      MULTI_MEASURE_REPEAT_PALETTE_HEIGHT          = 4.0;
static const qreal      MULTI_MEASURE_REPEAT_PALETTE_WIDTH           = 3 * MULTI_MEASURE_REPEAT_PALETTE_HEIGHT;

void MultiMeasureRepeat::layout()
      {
      qreal _spatium    = spatium();


      if (score() == gscore || !startElement() || !endElement()) {  // for use in palettes or while dragging
            if (spannerSegments().empty())
                  add(createLineSegment());
            LineSegment* s = frontSegment();
            s->setPos(QPointF());
            s->setPos2(QPointF(_spatium * MULTI_MEASURE_REPEAT_PALETTE_WIDTH, -_spatium * MULTI_MEASURE_REPEAT_PALETTE_HEIGHT));
            s->layout();
            return;
            }

      // code to layout multi-measure repeat goes here.
      // can probably copy most from glissando.
      // should be able to minimize measure width if no other elements

      }

//---------------------------------------------------------
//   setProperty
//---------------------------------------------------------

bool MultiMeasureRepeat::setProperty(P_ID propertyId, const QVariant& v)
      {
      switch (propertyId) {
            case P_ID::MULTI_MEASURE_REPEAT_SHOW_NUMBER_OF_MEASURES:
                  setShowNumberOfMeasures(v.toBool());
                  break;
            default:
                  if (!SLine::setProperty(propertyId, v))
                        return false;
                  break;
            }
      score()->setLayoutAll();
      return true;
      }

//---------------------------------------------------------
//   propertyDefault
//---------------------------------------------------------

QVariant MultiMeasureRepeat::propertyDefault(P_ID propertyId) const
      {
      switch (propertyId) {
            case P_ID::LINE_WIDTH:
                  return score()->style(StyleIdx::multiMeasureRepeatLineWidth);

            case P_ID::LINE_STYLE:
                  return int(score()->styleI(StyleIdx::multiMeasureRepeatLineStyle));

            case P_ID::TEXT_STYLE_TYPE:
                  return int(TextStyleType::MULTI_MEASURE_REPEAT);

            case P_ID::MULTI_MEASURE_REPEAT_SHOW_NUMBER_OF_MEASURES:
                  return true;

            default:
                  return SLine::propertyDefault(propertyId);
            }
      }

//---------------------------------------------------------
//   propertyStyle
//---------------------------------------------------------

PropertyStyle MultiMeasureRepeat::propertyStyle(P_ID id) const
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
                  return lineWidthStyle;

            case P_ID::LINE_STYLE:
                  return lineStyleStyle;

            default:
                  return SLine::propertyStyle(id);
            }
      }

//---------------------------------------------------------
//   resetProperty
//---------------------------------------------------------

void MultiMeasureRepeat::resetProperty(P_ID id)
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
                  setLineWidth(score()->styleS(StyleIdx::multiMeasureRepeatLineWidth));
                  lineWidthStyle = PropertyStyle::STYLED;
                  break;

            case P_ID::LINE_STYLE:
                  setLineStyle(Qt::PenStyle(score()->styleI(StyleIdx::multiMeasureRepeatLineStyle)));
                  lineStyleStyle = PropertyStyle::STYLED;
                  break;

            default:
                  return SLine::resetProperty(id);
            }
      }

//---------------------------------------------------------
//   styleChanged
//    reset all styled values to actual style
//---------------------------------------------------------

void MultiMeasureRepeat::styleChanged()
      {
      if (lineWidthStyle == PropertyStyle::STYLED)
            setLineWidth(score()->styleS(StyleIdx::multiMeasureRepeatLineWidth));
      if (lineStyleStyle == PropertyStyle::STYLED)
            setLineStyle(Qt::PenStyle(score()->styleI(StyleIdx::multiMeasureRepeatLineStyle)));
      }

//---------------------------------------------------------
//   getPropertyStyle
//---------------------------------------------------------

StyleIdx MultiMeasureRepeat::getPropertyStyle(P_ID id) const
      {
      switch (id) {
            case P_ID::LINE_WIDTH:
                  return StyleIdx::multiMeasureRepeatLineWidth;
            case P_ID::LINE_STYLE:
                  return StyleIdx::multiMeasureRepeatLineStyle;
            default:
                  break;
            }
      return StyleIdx::NOSTYLE;
      }

}

