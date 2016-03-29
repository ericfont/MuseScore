//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2012 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include <QtTest/QtTest>
#include "mtest/testutils.h"
#include "libmscore/score.h"
#include "libmscore/measure.h"
#include "libmscore/segment.h"
#include "libmscore/chordrest.h"
#include "libmscore/instrchange.h"
#include "libmscore/staff.h"
#include "libmscore/part.h"
#include "libmscore/undo.h"
#include "synthesizer/midipatch.h"

#define DIR QString("libmscore/instrumentchange/")

using namespace Ms;

//---------------------------------------------------------
//   TestInstrumentChange
//---------------------------------------------------------

class TestInstrumentChange : public QObject, public MTest {
      Q_OBJECT

      Score* test_pre(const char* p);
      void test_post(Score* score, const char* p);

   private slots:
      void initTestCase();
      void testAdd();
      void testDelete();
      void testChange();
      void testMixer();
      void testCopy();
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestInstrumentChange::initTestCase()
      {
qDebug("before initMTest");
      initMTest();
qDebug("after initMTest");
      }

//---------------------------------------------------------
//   chordsymbol
//---------------------------------------------------------

Score* TestInstrumentChange::test_pre(const char* p)
      {
qDebug("test_pre1");
      QString p1 = DIR + p + ".mscx";
      Score* score = readScore(p1);
qDebug("test_pre2");
      score->doLayout();
qDebug("test_pre3");
      return score;
      }

void TestInstrumentChange::test_post(Score* score, const char* p)
      {
qDebug("test_post1");
      QString p1 = p;
      p1 += "-test.mscx";
      QString p2 = DIR + p + "-ref.mscx";
qDebug("test_post2");
      QVERIFY(saveCompareScore(score, p1, p2));
qDebug("test_post3");
      delete score;
qDebug("test_post4");
      }

void TestInstrumentChange::testAdd()
      {
qDebug("testAdd1");
      Score* score = test_pre("add");
      Measure* m = score->firstMeasure()->nextMeasure();
      Segment* s = m->first(Segment::Type::ChordRest);
      InstrumentChange* ic = new InstrumentChange(score);
qDebug("testAdd2");
      ic->setParent(s);
      ic->setTrack(0);
      ic->setXmlText("Instrument");
      score->undoAddElement(ic);
qDebug("testAdd3");
      score->doLayout();
qDebug("testAdd4");
      test_post(score, "add");
qDebug("testAdd5");
      }

void TestInstrumentChange::testDelete()
      {
qDebug("testDelete1");
      Score* score = test_pre("delete");
      Measure* m = score->firstMeasure()->nextMeasure();
      Segment* s = m->first(Segment::Type::ChordRest);
      InstrumentChange* ic = static_cast<InstrumentChange*>(s->annotations()[0]);
qDebug("testDelete2");
      score->deleteItem(ic);
qDebug("testDelete3");
      score->doLayout();
qDebug("testDelete4");
      test_post(score, "delete");
qDebug("testDelete5");
      }

void TestInstrumentChange::testChange()
      {
qDebug("testChange1");
      Score* score = test_pre("change");
      Measure* m = score->firstMeasure()->nextMeasure();
      Segment* s = m->first(Segment::Type::ChordRest);
      InstrumentChange* ic = static_cast<InstrumentChange*>(s->annotations()[0]);
      Instrument* ni = score->staff(1)->part()->instrument();
      ic->setInstrument(*ni);
qDebug("testChange2");
      score->startCmd();
      ic->setXmlText("Instrument Oboe");
      score->undo(new ChangeInstrument(ic, ic->instrument()));
qDebug("testChange3");
      score->endCmd();
qDebug("testChange4");
      score->doLayout();
qDebug("testChange5");
      test_post(score, "change");
qDebug("testChange6");
      }

void TestInstrumentChange::testMixer()
      {
qDebug("testMixer1");
      Score* score = test_pre("mixer");
      Measure* m = score->firstMeasure()->nextMeasure();
      Segment* s = m->first(Segment::Type::ChordRest);
      InstrumentChange* ic = static_cast<InstrumentChange*>(s->annotations()[0]);
      int idx = score->staff(0)->channel(s->tick(), 0);
      Channel* c = score->staff(0)->part()->instrument(s->tick())->channel(idx);
qDebug("testMixer2");
      MidiPatch* mp = new MidiPatch;
      mp->bank = 0;
      mp->drum = false;
      mp->name = "Viola";
      mp->prog = 41;
      mp->synti = "Fluid";
      score->startCmd();
qDebug("testMixer3");
      ic->setXmlText("Mixer Viola");
      score->undo(new ChangePatch(score, c, mp));
qDebug("testMixer4");
      score->endCmd();
qDebug("testMixer5");
      score->doLayout();
qDebug("testMixer6");
      test_post(score, "mixer");
qDebug("testMixer7");
      }

void TestInstrumentChange::testCopy()
      {
qDebug("testCopy1");
      Score* score = test_pre("copy");
      Measure* m = score->firstMeasure()->nextMeasure();
      Segment* s = m->first(Segment::Type::ChordRest);
      InstrumentChange* ic = static_cast<InstrumentChange*>(s->annotations()[0]);
qDebug("testCopy2");
      m = m->nextMeasure();
      s = m->first(Segment::Type::ChordRest);
      InstrumentChange* nic = new InstrumentChange(*ic);
qDebug("testCopy3");
      nic->setParent(s);
      nic->setTrack(4);
qDebug("testCopy4");
      score->undoAddElement(nic);
qDebug("testCopy5");
      score->doLayout();
qDebug("testCopy6");
      test_post(score, "copy");
qDebug("testCopy7");
      }

QTEST_MAIN(TestInstrumentChange)
#include "tst_instrumentchange.moc"
