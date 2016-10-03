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
//      painter->drawLine(QPointF(), pos2());               // DEBUG

      // rotate painter so that the line become horizontal
      qreal w     = pos2().x();
      qreal h     = pos2().y();
      qreal l     = sqrt(w * w + h * h);
      qreal wi = asin(-h / l) * 180.0 / M_PI;
      qreal scale = painter->worldTransform().m11();
      painter->rotate(-wi);

      painter->drawLine(QLineF(0.0, 0.0, l, 0.0));

      /*if (multiMeasureRepeat()->showText()) {
          MODIFY FROM GLISSANDO CODE
            const TextStyle& st = score()->textStyle(TextStyleType::GLISSANDO);
            QRectF r = st.fontMetrics(_spatium).boundingRect(glissando()->text());
            // if text longer than available space, skip it
            if (r.width() < l) {
                  qreal yOffset = r.height() + r.y();       // find text descender height
                  // raise text slightly above line and slightly more with WAVY than with STRAIGHT
                  yOffset += _spatium * (glissando()->glissandoType() == Glissando::Type::WAVY ? 0.4 : 0.1);
                  painter->setFont(st.font(_spatium * MScore::pixelRatio));
                  qreal x = (l - r.width()) * 0.5;
                  painter->drawText(QPointF(x, -yOffset), glissando()->text());
                  }
            }*/
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
                  return MultiMeasureRepeat()->getProperty(id);
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

      _showText      = true;
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

void MultiMeasureRepeat::write(Xml& e)
     {
      // todo copy from glissando or somehting else
      }



//---------------------------------------------------------
//   layout
//---------------------------------------------------------

void MultiMeasureRepeat::layout()
      {
  /*    qreal _spatium    = spatium();

      if (score() == gscore || !startElement() || !endElement()) {  // for use in palettes or while dragging
            if (spannerSegments().empty())
                  add(createLineSegment());
            LineSegment* s = frontSegment();
            s->setPos(QPointF());
            s->setPos2(QPointF(_spatium * GLISS_PALETTE_WIDTH, -_spatium * GLISS_PALETTE_HEIGHT));
            s->layout();
            return;
            }

      SLine::layout();
      setPos(0.0, 0.0);
      adjustReadPos();

      Note*       anchor1     = static_cast<Note*>(startElement());
      Note*       anchor2     = static_cast<Note*>(endElement());
      Chord*      cr1         = anchor1->chord();
      Chord*      cr2         = anchor2->chord();
      GlissandoSegment*       segm1 = static_cast<GlissandoSegment*>(frontSegment());
      GlissandoSegment*       segm2 = static_cast<GlissandoSegment*>(backSegment());

      // Note: line segments are defined by
      // initial point: ipos() (relative to system origin)
      // ending point:  pos2() (relative to initial point)

      // LINE ENDING POINTS TO NOTEHEAD CENTRES

      // assume gliss. line goes from centre of initial note centre to centre of ending note:
      // move first segment origin and last segment ending point from notehead origin to notehead centre
      QPointF     offs1       = QPointF(anchor1->headWidth() * 0.5, 0.0);
      QPointF     offs2       = QPointF(anchor2->headWidth() * 0.5, 0.0);

      // AVOID HORIZONTAL LINES

      int         upDown      = (0 < (anchor2->pitch() - anchor1->pitch())) - ((anchor2->pitch() - anchor1->pitch()) < 0);
      // on TAB's, glissando are by necessity on the same string, this gives an horizontal glissando line;
      // make bottom end point lower and top ending point higher
      if (cr1->staff()->isTabStaff()) {
                  qreal yOff = cr1->staff()->lineDistance() * 0.4 * _spatium;
                  offs1.ry() += yOff * upDown;
                  offs2.ry() -= yOff * upDown;
            }
      // if not TAB, angle glissando between notes on the same line
      else {
            if (anchor1->line() == anchor2->line()) {
                  offs1.ry() += _spatium * 0.25 * upDown;
                  offs2.ry() -= _spatium * 0.25 * upDown;
                  }
            }

      // move initial point of first segment and adjust its length accordingly
      segm1->setPos (segm1->ipos()  + offs1);
      segm1->setPos2(segm1->ipos2() - offs1);
      // adjust ending point of last segment
      segm2->setPos2(segm2->ipos2() + offs2);

      // FINAL SYSTEM-INITIAL NOTE
      // if the last gliss. segment attaches to a system-initial note, some extra width has to be added
      if (cr2->segment()->measure() == cr2->segment()->system()->firstMeasure() && cr2->rtick() == 0
                  // but ignore graces after, as they are not the first note of the system,
                  // even if their segment is the first segment of the system
                  && !(cr2->noteType() == NoteType::GRACE8_AFTER
                        || cr2->noteType() == NoteType::GRACE16_AFTER || cr2->noteType() == NoteType::GRACE32_AFTER)
                  // also ignore if cr1 is a child of cr2, which means cr1 is a grace-before of cr2
                  && !(cr1->parent() == cr2))
            {
            segm2->rxpos() -= GLISS_STARTOFSYSTEM_WIDTH * _spatium;
            segm2->rxpos2()+= GLISS_STARTOFSYSTEM_WIDTH * _spatium;
            }

      // INTERPOLATION OF INTERMEDIATE POINTS
      // This probably belongs to SLine class itself; currently it does not seem
      // to be needed for anything else than Glissando, though

      // get total x-width and total y-height of all segments
      qreal xTot = 0.0;
      for (SpannerSegment* segm : spannerSegments())
            xTot += segm->ipos2().x();
      qreal y0   = segm1->ipos().y();
      qreal yTot = segm2->ipos().y() + segm2->ipos2().y() - y0;
      qreal ratio = yTot / xTot;
      // interpolate y-coord of intermediate points across total width and height
      qreal xCurr = 0.0;
      qreal yCurr;
      for (int i = 0; i < spannerSegments().count()-1; i++) {
           SpannerSegment* segm = segmentAt(i);
           xCurr += segm->ipos2().x();
           yCurr = y0 + ratio * xCurr;
           segm->rypos2() = yCurr - segm->ipos().y();       // position segm. end point at yCurr
           // next segment shall start where this segment stopped
           segm = segmentAt(i+1);
           segm->rypos2() += segm->ipos().y() - yCurr;      // adjust next segm. vertical length
           segm->rypos() = yCurr;                           // position next segm. start point at yCurr
            }

      // STAY CLEAR OF NOTE APPENDAGES

      // initial note dots / ledger line / notehead
      offs1 *= -1.0;          // discount changes already applied
      int dots = cr1->dots();
      LedgerLine * ledLin = cr1->ledgerLines();
      // if dots, start at right of last dot
      // if no dots, from right of ledger line, if any; from right of notehead, if no ledger line
      offs1.rx() += (dots && anchor1->dot(dots-1) ? anchor1->dot(dots-1)->pos().x() + anchor1->dot(dots-1)->width()
                  : (ledLin ? ledLin->pos().x() + ledLin->width() : anchor1->headWidth()) );

      // final note arpeggio / accidental / ledger line / accidental / arpeggio (i.e. from outermost to innermost)
      offs2 *= -1.0;          // discount changes already applied
      if (Arpeggio* a = cr2->arpeggio())
            offs2.rx() += a->pos().x() + a->userOff().x();
      else if (Accidental* a = anchor2->accidental())
            offs2.rx() += a->pos().x() + a->userOff().x();
      else if ( (ledLin = cr2->ledgerLines()) != nullptr)
            offs2.rx() += ledLin->pos().x();

      // add another a quarter spatium of 'air'
      offs1.rx() += _spatium * 0.25;
      offs2.rx() -= _spatium * 0.25;

      // apply offsets: shorten first segment by x1 (and proportionally y) and adjust its length accordingly
      offs1.ry() = segm1->ipos2().y() * offs1.x() / segm1->ipos2().x();
      segm1->setPos(segm1->ipos() + offs1);
      segm1->setPos2(segm1->ipos2() - offs1);
      // adjust last segment length by x2 (and proportionally y)
      offs2.ry() = segm2->ipos2().y() * offs2.x() / segm2->ipos2().x();
      segm2->setPos2(segm2->ipos2() + offs2);

      for (SpannerSegment* segm : spannerSegments())
            static_cast<GlissandoSegment*>(segm)->layout();

      // compute glissando bbox as the bbox of the last segment, relative to the end anchor note
      QPointF anchor2PagePos = anchor2->pagePos();
      QPointF system2PagePos = cr2->segment()->system()->pagePos();
      QPointF anchor2SystPos = anchor2PagePos - system2PagePos;
      QRectF r = QRectF(anchor2SystPos - segm2->pos(), anchor2SystPos - segm2->pos() - segm2->pos2()).normalized();
      qreal lw = _spatium * lineWidth().val() * .5;
      setbbox(r.adjusted(-lw, -lw, lw, lw));*/
      }

//---------------------------------------------------------
//   setProperty
//---------------------------------------------------------

bool MultiMeasureRepeat::setProperty(P_ID propertyId, const QVariant& val)
      {
      switch (propertyId) {
            case P_ID::LINE_WIDTH:
                  lineWidthStyle = PropertyStyle::UNSTYLED;
                  TextLineBase::setProperty(propertyId, val);
                  break;

            case P_ID::LINE_STYLE:
                  lineStyleStyle = PropertyStyle::UNSTYLED;
                  TextLineBase::setProperty(propertyId, val);
                  break;

            default:
                  if (!TextLineBase::setProperty(propertyId, val))
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

            default:
                  return TextLineBase::propertyDefault(propertyId);
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
                  return TextLineBase::propertyStyle(id);
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
                  return TextLineBase::resetProperty(id);
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

