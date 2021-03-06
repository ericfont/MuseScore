//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2020 MuseScore BVBA and others
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
#ifndef MU_USERSCORES_NEWSCOREMODEL_H
#define MU_USERSCORES_NEWSCOREMODEL_H

#include <QObject>

#include "modularity/ioc.h"

#include "iuserscoresconfiguration.h"

#include "notation/inotationcreator.h"
#include "notation/notationtypes.h"
#include "context/iglobalcontext.h"

namespace mu::userscores {
class NewScoreModel : public QObject
{
    Q_OBJECT

    INJECT(scores, IUserScoresConfiguration, configuration)
    INJECT(scores, notation::INotationCreator, notationCreator)
    INJECT(scores, context::IGlobalContext, globalContext)

public:
    explicit NewScoreModel(QObject* parent = nullptr);

    Q_INVOKABLE QString preferredScoreCreationMode() const;
    Q_INVOKABLE bool createScore(const QVariant& info);

private:
    notation::ScoreCreateOptions parseOptions(const QVariantMap& info) const;
    notation::DurationType noteIconToDurationType(int noteIconCode) const;
    void updatePreferredScoreCreationMode(bool isScoreCreatedFromInstruments);
};
}

#endif // MU_USERSCORES_NEWSCOREMODEL_H
