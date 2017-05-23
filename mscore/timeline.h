//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: timeline.h 4785 2011-09-14 10:06:35Z wschweer $
//
//  Copyright (C) 2002-2009 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef __TIMELINE_H__
#define __TIMELINE_H__

namespace Ms {

class Score;
class ScoreView;
class Page;
class Timeline;
class ViewRect;

//---------------------------------------------------------
//   TScrollArea
//    modified QScrollArea for Timeline
//---------------------------------------------------------

class TScrollArea : public QScrollArea {
      Q_OBJECT

      virtual void resizeEvent(QResizeEvent*);

   public:
      TScrollArea(QWidget* w = 0);
      void orientationChanged();
      };

//---------------------------------------------------------
//   Timeline
//---------------------------------------------------------

class Timeline : public QGraphicsView {
      Q_OBJECT

      TScrollArea* scrollArea;

   public:
      Timeline(TScrollArea* sa, QWidget* parent = 0);
      };


} // namespace Ms
#endif

