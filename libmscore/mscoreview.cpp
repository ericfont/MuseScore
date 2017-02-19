//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2013 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "mscoreview.h"
#include "score.h"
#include "page.h"
#include "chordrest.h"

namespace Ms {

//---------------------------------------------------------
//   elementLower
//---------------------------------------------------------

static bool elementLower(const Element* e1, const Element* e2)
      {
      if (!e1->selectable())
            return false;
      return e1->z() < e2->z();
      }

//---------------------------------------------------------
//   elementAt
//---------------------------------------------------------

Element* MuseScoreView::elementAt(const QPointF& p)
      {
      QList<Element*> el = elementsAt(p);
#if 0
      qDebug("elementAt");
      foreach(const Element* e, el)
            qDebug("  %s %d", e->name(), e->selected());
#endif
      Element* e = el.value(0);
      if (e && (e->type() == ElementType::PAGE))
            e = el.value(1);
      return e;
      }

//---------------------------------------------------------
//   point2page
//---------------------------------------------------------

Page* MuseScoreView::point2page(const QPointF& p)
      {
      if (score()->layoutMode() == LayoutMode::LINE)
            return score()->pages().isEmpty() ? 0 : score()->pages().front();
      foreach(Page* page, score()->pages()) {
            if (page->bbox().translated(page->pos()).contains(p))
                  return page;
            }
      return 0;
      }

//---------------------------------------------------------
//   elementsAt
//    p is in canvas coordinates
//---------------------------------------------------------

const QList<Element*> MuseScoreView::elementsAt(const QPointF& p)
      {
      QList<Element*> el;

      Page* page = point2page(p);
      if (page) {
            el = page->items(p - page->pos());
            qSort(el.begin(), el.end(), elementLower);
            }
      return el;
      }



//---------------------------------------------------------
//   nearestChordRest
//---------------------------------------------------------

ChordRest* MuseScoreView::nearestChordRest(const QPointF& p)
      {
      return 0;
   /*   Page* page = point2page(p);
      if (!page) {
            // qDebug("  no page");
            return 0;
            }

      p       -= page->pos();
      double w = (preferences.proximity * .5) / matrix().m11();
      QRectF r(p.x() - w, p.y() - w, 3.0 * w, 3.0 * w);

      QList<Element*> el = page->items(r);
      QList<Element*> ll;
      for (Element* e : el) {
            e->itemDiscovered = 0;
            if (!e->selectable() || e->isPage())
                  continue;
            if (e->contains(p))
                  ll.append(e);
            }
      int n = ll.size();
      if ((n == 0) || ((n == 1) && (ll[0]->isMeasure()))) {
            //
            // if no relevant element hit, look nearby
            //
            for (Element* e : el) {
                  if (e->isPage() || !e->selectable())
                        continue;
                  if (e->intersects(r))
                        ll.append(e);
                  }
            }
      if (ll.empty()) {
            // qDebug("  nothing found");
            return 0;
            }
      qSort(ll.begin(), ll.end(), elementLower);

#if 0
      qDebug("elementNear");
      foreach(const Element* e, ll)
            qDebug("  %s selected %d z %d", e->name(), e->selected(), e->z());
#endif
      Element* e = ll.at(0);
      return e;*/

      }

}

