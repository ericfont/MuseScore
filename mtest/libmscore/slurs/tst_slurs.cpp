//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2015 Werner Schweer & others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include <QtTest/QtTest>
#include "mtest/testutils.h"
#include "libmscore/chord.h"
#include "libmscore/excerpt.h"
#include "libmscore/glissando.h"
#include "libmscore/layoutbreak.h"
#include "libmscore/lyrics.h"
#include "libmscore/measure.h"
#include "libmscore/part.h"
#include "libmscore/staff.h"
#include "libmscore/score.h"
#include "libmscore/system.h"
#include "libmscore/undo.h"

#define DIR QString("libmscore/slurs/")

using namespace Ms;

//---------------------------------------------------------
//   TestSlurs
//---------------------------------------------------------

class TestSlurs : public QObject, public MTest
      {
      Q_OBJECT

   private slots:
      void initTestCase();
      void slurs_105951();
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestSlurs::initTestCase()
      {
      initMTest();
      }

//---------------------------------------------------------
///  slurs_105951
///   Test for crash if delete elements of type slur/tie when a slur/tie crosses linebreak
//---------------------------------------------------------

void TestSlurs::slurs_105951()
      {
      Score* score = readScore(DIR + "slurs_105951-slur-across-linebreak.mscx");
      QVERIFY(score);
      delete score;
      }

QTEST_MAIN(TestSlurs)
#include "tst_slurs.moc"

