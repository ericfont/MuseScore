//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2013 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include "timeline.h"
#include "navigator.h"
#include "musescore.h"
#include "scoreview.h"
#include "libmscore/score.h"
#include "libmscore/page.h"
#include "preferences.h"
#include "libmscore/mscore.h"
#include "libmscore/system.h"
#include "libmscore/measurebase.h"
#include "libmscore/measure.h"
#include "libmscore/chord.h"

namespace Ms {

#include<QGraphicsView>
#include<QMouseEvent>

//---------------------------------------------------------
//   showTimeline
//---------------------------------------------------------

void MuseScore::showTimeline(bool visible)
      {
      Timeline* n = static_cast<Timeline*>(_timeline->widget());
      if (n == 0 && visible) {
            n = new Timeline(_timeline);//, this);
        //    n->setScoreView(cv);
            }
      _timeline->setVisible(visible);
      getAction("toggle-timeline")->setChecked(visible);
      }

//---------------------------------------------------------
//   TScrollArea
//---------------------------------------------------------

TScrollArea::TScrollArea(QWidget* w)
   : QScrollArea(w)
      {
      setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
      setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
      setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      setMinimumHeight(40);
      setLineWidth(0);
      }

//---------------------------------------------------------
//   orientationChanged
//---------------------------------------------------------

void TScrollArea::orientationChanged()
      {
      if (MScore::verticalOrientation()) {
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            }
      else {
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            }
      }

//---------------------------------------------------------
//   resizeEvent
//---------------------------------------------------------

void TScrollArea::resizeEvent(QResizeEvent* ev)
      {
      if (widget()) {
            widget()->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
            widget()->setMinimumSize(0, 0);
            }
      if (widget() && (ev->size().height() != ev->oldSize().height()))
            widget()->resize(widget()->width(), ev->size().height());
      if (widget() && (ev->size().width() != ev->oldSize().width()))
            widget()->resize(ev->size().width(), widget()->height());
      QScrollArea::resizeEvent(ev);
      }

//---------------------------------------------------------
//   Timeline
//---------------------------------------------------------

Timeline::Timeline(TScrollArea* sa, QWidget* parent)
  : QGraphicsView(parent)
      {
      setAttribute(Qt::WA_NoBackground);
      //_score         = 0;
      scrollArea     = sa;
      scrollArea->setWidgetResizable(true);
      //_cv            = 0;
      //viewRect       = new ViewRect(this);
      setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      sa->setWidget(this);
      sa->setWidgetResizable(false);
      //_previewOnly = false;
      //_scene = new QGraphicsScene(parent->rect());
      //_scene->addRect(0, 0, 50, 50, QPen(Qt::blue), QBrush(Qt::red));
      //_scene = new QGraphicsScene(0,0,100,100);
      setScene(new QGraphicsScene);//_scene);
      scene()->addRect(0,0,100,100,QPen(Qt::blue),QBrush(Qt::red));
      scene()->setBackgroundBrush(Qt::black);
      //QGraphicsEllipseItem *item = new QGraphicsEllipseItem( 0, scene() );
      //    item->setRect( -50.0, -50.0, 100.0, 100.0 );
      //qDebug() << scene()->height();

      }

}

