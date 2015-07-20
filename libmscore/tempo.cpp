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

#include "tempo.h"
#include "xml.h"

namespace Ms {

//---------------------------------------------------------
//   TEvent
//---------------------------------------------------------

TEvent::TEvent()
      {
      type              = TempoType::INVALID;
      tempo             = 0.0;
      time              = 0.0;
      pauseBeforeTick   = 0.0;
      pauseThroughTick  = 0.0;
      }

TEvent::TEvent(const TEvent& e)
      {
      type  = e.type;
      tempo = e.tempo;
      time  = e.time;
      pauseBeforeTick = e.pauseBeforeTick;
      pauseThroughTick = e.pauseThroughTick;
      }

TEvent::TEvent(qreal tempo, qreal pauseBeforeTick, qreal pauseThroughTick, TempoType type)
      {
      this->type  = type;
      this->tempo = tempo;
      this->time  = 0.0;
      this->pauseBeforeTick = pauseBeforeTick;
      this->pauseThroughTick = pauseThroughTick;
      }

bool TEvent::valid() const
      {
      return !(!type);
      }

//---------------------------------------------------------
//   TempoMap
//---------------------------------------------------------

TempoMap::TempoMap()
      {
      _tempo    = 2.0;        // default fixed tempo in beat per second
      _tempoSN  = 1;
      _relTempo = 1.0;
      }

//---------------------------------------------------------
//   setPauseBeforeTick
//    a PAUSE_BEFORE_TICK occurs just before reaching a tick.
//    (e.g. breaths/caesuras before a jump or repeat sign will pause playback
//    both when the jump/repeat is taken and when passing through the jump/repeat)
//---------------------------------------------------------

void TempoMap::setPauseBeforeTick(int tick, qreal seconds)
      {
      auto e = find(tick);
      if (e != end()) {
            e->second.pauseBeforeTick = seconds;
            e->second.type |= TempoType::PAUSE_BEFORE_TICK;
            }
      else {
            insert(std::pair<const int, TEvent> (tick, TEvent(tempo(tick), seconds, 0.0, TempoType::PAUSE_BEFORE_TICK)));
            }
      normalize();

      dump();
      }

//---------------------------------------------------------
//   setPauseThroughTick
//    a PAUSE_THROUGH_TICK only gets triggered when passing through a tick.
//    (e.g. a section break at a repeat sign will only pause playback when passing through the repeat sign)
//---------------------------------------------------------

void TempoMap::setPauseThroughTick(int tick, qreal seconds )
      {
      auto e = find(tick);
      if (e != end()) {
            e->second.pauseThroughTick = seconds;
            e->second.type |= TempoType::PAUSE_THROUGH_TICK;
            }
      else {
            insert(std::pair<const int, TEvent> (tick, TEvent(tempo(tick), 0.0, seconds, TempoType::PAUSE_THROUGH_TICK)));
            }
      normalize();

      dump();
      }

//---------------------------------------------------------
//   setTempo
//---------------------------------------------------------

void TempoMap::setTempo(int tick, qreal tempo)
      {
      auto e = find(tick);
      if (e != end()) {
            e->second.tempo = tempo;
            e->second.type |= TempoType::FIX;
            }
      else
            insert(std::pair<const int, TEvent> (tick, TEvent(tempo, 0.0, 0.0, TempoType::FIX)));
      normalize();

      dump();
      }

//---------------------------------------------------------
//   TempoMap::normalize
//---------------------------------------------------------

void TempoMap::normalize()
      {
      qreal time  = 0;
      int tick    = 0;
      qreal tempo = 2.0;
      for (auto e = begin(); e != end(); ++e) {

            // entries that represent a pause *only* (PAUSE_BEFORE_TICK or PAUSE_THROUGH_TICK)
            // without a tempo change (FIX or RAMP)
            // need to be corrected to continue previous tempo
            if ((e->second.type&(TempoType::PAUSE_BEFORE_TICK|TempoType::PAUSE_THROUGH_TICK)) &&
               !(e->second.type&(TempoType::FIX|TempoType::RAMP)))
                  e->second.tempo = tempo;

            int delta = e->first - tick;
            time += qreal(delta) / (MScore::division * tempo * _relTempo);
            time += e->second.pauseBeforeTick + e->second.pauseThroughTick;
            e->second.time = time;
            tick  = e->first;
            tempo = e->second.tempo;
            }
      ++_tempoSN;
      }

//---------------------------------------------------------
//   TempoMap::dump
//---------------------------------------------------------

void TempoMap::dump() const
      {
      qDebug("\nTempoMap: Tick | Type | Tempo    | PauseBefore | PauseThrough | Time");
      for (auto i = begin(); i != end(); ++i)
            qDebug("          %6d | %2d   | %5f | %5f    | %5f     | %5f",
               i->first, static_cast<char>(i->second.type), i->second.tempo, i->second.pauseBeforeTick, i->second.pauseThroughTick, i->second.time);
      }

//---------------------------------------------------------
//   clear
//---------------------------------------------------------

void TempoMap::clear()
      {
      std::map<int,TEvent>::clear();
      ++_tempoSN;
      }

//---------------------------------------------------------
//   tempo
//---------------------------------------------------------

qreal TempoMap::tempo(int tick) const
      {
      if (empty())
            return 2.0;
      auto i = lower_bound(tick);
      if (i == end()) {
            --i;
            return i->second.tempo;
            }
      if (i->first == tick)
            return i->second.tempo;
      if (i == begin())
            return 2.0;
      --i;
      return i->second.tempo;
      }

//---------------------------------------------------------
//   del
//---------------------------------------------------------

void TempoMap::del(int tick)
      {
      auto e = find(tick);
      if (e == end()) {
            qDebug("TempoMap::del event at (%d): not found", tick);
            // abort();
            return;
            }

      // don't delete event if still being used for pause
      e->second.type = (e->second.type & (TempoType::PAUSE_BEFORE_TICK|TempoType::PAUSE_THROUGH_TICK));
      if (!e->second.type )
            erase(e);

      normalize();
      }

//---------------------------------------------------------
//   setRelTempo
//---------------------------------------------------------

void TempoMap::setRelTempo(qreal val)
      {
      _relTempo = val;
      normalize();
      }

//---------------------------------------------------------
//   delTempo
//---------------------------------------------------------

void TempoMap::delTempo(int tick)
      {
      del(tick);
      ++_tempoSN;
      }

//---------------------------------------------------------
//   tick2time
//---------------------------------------------------------

qreal TempoMap::tick2time(int tick, qreal time, int* sn) const
      {
      return (*sn == _tempoSN) ? time : tick2time(tick, sn);
      }

//---------------------------------------------------------
//   time2tick
//    return cached value t if list did not change
//---------------------------------------------------------

int TempoMap::time2tick(qreal time, int t, int* sn) const
      {
      return (*sn == _tempoSN) ? t : time2tick(time, sn);
      }

//---------------------------------------------------------
//   tick2time
//---------------------------------------------------------

qreal TempoMap::tick2time(int tick, int* sn) const
      {
      qreal time  = 0.0;
      qreal delta = qreal(tick);
      qreal tempo = 2.0;

      if (!empty()) {
            int ptick  = 0;
            auto e = lower_bound(tick);
            if (e == end()) {
                  auto pe = e;
                  --pe;
                  ptick = pe->first;
                  tempo = pe->second.tempo;
                  time  = pe->second.time;
                  }
            else if (e->first == tick) {
                  ptick = tick;
                  tempo = e->second.tempo;
                  time  = e->second.time;
                  }
            else if (e != begin()) {
                  auto pe = e;
                  --pe;
                  ptick = pe->first;
                  tempo = pe->second.tempo;
                  time  = pe->second.time;
                  }
            delta = qreal(tick - ptick);
            }
      else
            qDebug("TempoMap: empty");
      if (sn)
            *sn = _tempoSN;
      time += delta / (MScore::division * tempo * _relTempo);
      return time;
      }

//---------------------------------------------------------
//   time2tick
//---------------------------------------------------------

int TempoMap::time2tick(qreal time, int* sn) const
      {
      int   tickOfPreviousEvent = 0;
      qreal timeOfPreviousEvent = 0.0;
      qreal tempoOfPreviousEvent = 2.0;  // Why was _tempo overwritten with = 2.0?  _tempo is supposed to hold initial tempo, which should default to 2.0
      for (auto nextEvent = begin(); nextEvent != end(); ++nextEvent) {

            qreal timeOfNextEvent  = nextEvent->second.time;
            int   tickOfNextEvent  = nextEvent->first;
            qreal tempoOfNextEvent = nextEvent->second.tempo;
            qreal pauseOfNextEvent = nextEvent->second.pauseBeforeTick + nextEvent->second.pauseThroughTick;

            // done if found time inside a pause
            qreal timePriorToPause = timeOfNextEvent - pauseOfNextEvent;
            if ( (timePriorToPause < time) && (time <= timeOfNextEvent)) {
                  timeOfPreviousEvent += (time - timePriorToPause);
                  break;
                  }

            // done if found time before next event
            if (time <= timeOfNextEvent)
                  break;

            timeOfPreviousEvent  = timeOfNextEvent;
            tickOfPreviousEvent  = tickOfNextEvent;
            tempoOfPreviousEvent = tempoOfNextEvent;
            }

      qreal timeDelta = time - timeOfPreviousEvent;
      int tick = tickOfPreviousEvent + lrint(timeDelta * _relTempo * MScore::division * tempoOfPreviousEvent);

      if (sn)
            *sn = _tempoSN;

      return tick;
      }

}

