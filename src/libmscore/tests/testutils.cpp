//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2012-2013 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include "testutils.h"

#include <QtTest/QtTest>
#include <QTextStream>

#include "config.h"
#include "libmscore/score.h"
#include "libmscore/note.h"
#include "libmscore/chord.h"
#include "libmscore/instrtemplate.h"
#include "libmscore/page.h"
#include "libmscore/musescoreCore.h"
#include "libmscore/xml.h"
#include "libmscore/excerpt.h"
#include "thirdparty/qzip/qzipreader_p.h"

#include "framework/global/globalmodule.h"
#include "framework/fonts/fontsmodule.h"

static void initMyResources()
{
//    Q_INIT_RESOURCE(mtest);
}

namespace Ms {
//---------------------------------------------------------
//   writeReadElement
//    writes and element and reads it back
//---------------------------------------------------------

Element* MTest::writeReadElement(Element* element)
{
    //
    // write element
    //
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    XmlWriter xml(element->score(), &buffer);
    xml.header();
    element->write(xml);
    buffer.close();

    //
    // read element
    //

    XmlReader e(buffer.buffer());
    e.readNextStartElement();
    QString tag(e.name().toString());
    element = Element::name2Element(e.name(), score);
    element->read(e);
    return element;
}

//---------------------------------------------------------
//   MTest
//---------------------------------------------------------

MTest::MTest()
    : ed(0)
{
    MScore::testMode = true;
}

//---------------------------------------------------------
//   readScore
//---------------------------------------------------------

MasterScore* MTest::readScore(const QString& name)
{
    QString path = root + "/" + name;
    return readCreatedScore(path);
}

//---------------------------------------------------------
//   readCreatedScore
//---------------------------------------------------------

MasterScore* MTest::readCreatedScore(const QString& name)
{
    MasterScore* score = new MasterScore(mscore->baseStyle());
    QFileInfo fi(name);
    score->setName(fi.completeBaseName());
    QString csl  = fi.suffix().toLower();

    ScoreLoad sl;
    Score::FileError rv;
    if (csl == "mscz" || csl == "mscx") {
        rv = score->loadMsc(name, false);
    } else {
        rv = Score::FileError::FILE_UNKNOWN_TYPE;
    }

//    if (csl == "cap") {
//        rv = importCapella(score, name);
//        score->setMetaTag("originalFormat", csl);
//    } else if (csl == "capx") {
//        rv = importCapXml(score, name);
//        score->setMetaTag("originalFormat", csl);
//    } else if (csl == "ove") {
//        rv = importOve(score, name);
//    } else if (csl == "sgu") {
//        rv = importBB(score, name);
//    } else if (csl == "mscz" || csl == "mscx") {
//        rv = score->loadMsc(name, false);
//    } else if (csl == "mxl") {
//        rv = importCompressedMusicXml(score, name);
//    } else if (csl == "xml" || csl == "musicxml") {
//        rv = importMusicXml(score, name);
//    } else if (csl == "gp3" || csl == "gp4" || csl == "gp5" || csl == "gpx" || csl == "gp" || csl == "ptb") {
//        rv = importGTP(score, name);
//    } else {
//        rv = Score::FileError::FILE_UNKNOWN_TYPE;
//    }

    if (rv != Score::FileError::FILE_NO_ERROR) {
        QWARN(qPrintable(QString("readScore: cannot load <%1> type <%2>\n").arg(name).arg(csl)));
        delete score;
        score = 0;
    } else {
        for (Score* s : score->scoreList()) {
            s->doLayout();
        }
    }
    return score;
}

//---------------------------------------------------------
//   saveScore
//---------------------------------------------------------

bool MTest::saveScore(Score* score, const QString& name) const
{
    QFileInfo fi(name);
//      MScore::testMode = true;
    return score->Score::saveFile(fi);
}

//---------------------------------------------------------
//   compareFiles
//---------------------------------------------------------

bool MTest::compareFilesFromPaths(const QString& f1, const QString& f2)
{
    QString cmd = "diff";
    QStringList args;
    args.append("-u");
    args.append("--strip-trailing-cr");
    args.append(f2);
    args.append(f1);
    QProcess p;
    qDebug() << "Running " << cmd << " with arg1: " << QFileInfo(f2).fileName() << " and arg2: "
             << QFileInfo(f1).fileName();
    p.start(cmd, args);
    if (!p.waitForFinished() || p.exitCode()) {
        QByteArray ba = p.readAll();
        //qDebug("%s", qPrintable(ba));
        //qDebug("   <diff -u %s %s failed", qPrintable(compareWith),
        //   qPrintable(QString(root + "/" + saveName)));
        QTextStream outputText(stdout);
        outputText << QString(ba);
        outputText << QString("   <diff -u %1 %2 failed").arg(f2).arg(f1);
        return false;
    }
    return true;
}

bool MTest::compareFiles(const QString& saveName, const QString& compareWith) const
{
    return compareFilesFromPaths(saveName, root + "/" + compareWith);
}

//---------------------------------------------------------
//   saveCompareScore
//---------------------------------------------------------

// bool MTest::saveCompareScore(MasterScore* score, const QString& saveName, const QString& compareWith) const
bool MTest::saveCompareScore(Score* score, const QString& saveName, const QString& compareWith) const
{
    if (!saveScore(score, saveName)) {
        return false;
    }
    return compareFiles(saveName, compareWith);
}

//---------------------------------------------------------
//   saveCompareMusicXMLScore
//---------------------------------------------------------

bool MTest::saveCompareMusicXmlScore(MasterScore* score, const QString& saveName, const QString& compareWith)
{
    saveMusicXml(score, saveName);
    return compareFiles(saveName, compareWith);
}

//---------------------------------------------------------
//   saveCompareBrailleScore
//---------------------------------------------------------

bool MTest::saveCompareBrailleScore(MasterScore* score, const QString& saveName, const QString& compareWith)
{
    saveBraille(score, saveName);
    return compareFiles(saveName, compareWith);
}

//---------------------------------------------------------
//   savePdf
//---------------------------------------------------------

bool MTest::savePdf(MasterScore* cs, const QString& saveName)
{
    Q_ASSERT(false);
    return false;
//    QPrinter printerDev(QPrinter::HighResolution);
//    double w = cs->styleD(Sid::pageWidth);
//    double h = cs->styleD(Sid::pageHeight);
//    printerDev.setPaperSize(QSizeF(w,h), QPrinter::Inch);

//    printerDev.setCreator("MuseScore Version: " VERSION);
//    printerDev.setFullPage(true);
//    printerDev.setColorMode(QPrinter::Color);
////      printerDev.setDocName(cs->name());
//    printerDev.setOutputFormat(QPrinter::PdfFormat);

//    printerDev.setOutputFileName(saveName);
//    QPainter p(&printerDev);
//    p.setRenderHint(QPainter::Antialiasing, true);
//    p.setRenderHint(QPainter::TextAntialiasing, true);
//    double mag = printerDev.logicalDpiX() / DPI;
//    p.scale(mag, mag);

//    const QList<Page*> pl = cs->pages();
//    int pages    = pl.size();
//    int offset   = cs->pageNumberOffset();
//    int fromPage = printerDev.fromPage() - 1 - offset;
//    int toPage   = printerDev.toPage() - 1 - offset;
//    if (fromPage < 0) {
//        fromPage = 0;
//    }
//    if ((toPage < 0) || (toPage >= pages)) {
//        toPage = pages - 1;
//    }

//    for (int copy = 0; copy < printerDev.numCopies(); ++copy) {
//        bool firstPage = true;
//        for (int n = fromPage; n <= toPage; ++n) {
//            if (!firstPage) {
//                printerDev.newPage();
//            }
//            firstPage = false;

//            cs->print(&p, n);
//            if ((copy + 1) < printerDev.numCopies()) {
//                printerDev.newPage();
//            }
//        }
//    }
//    p.end();
//    return true;
}

//---------------------------------------------------------
//   saveMusicXml
//---------------------------------------------------------

bool MTest::saveMusicXml(MasterScore* score, const QString& saveName)
{
    Q_ASSERT(false);
    return false;// saveXml(score, saveName);
}

//---------------------------------------------------------
//   saveBraille
//---------------------------------------------------------

bool MTest::saveBraille(MasterScore* score, const QString& saveName)
{
    Q_ASSERT(false);
    return false;// Ms::saveBraille(score, saveName);
}

//---------------------------------------------------------
//   saveMimeData
//---------------------------------------------------------

bool MTest::saveMimeData(QByteArray mimeData, const QString& saveName)
{
    QFile f(saveName);
    if (!f.open(QIODevice::WriteOnly)) {
        return false;
    }

    f.write(mimeData);
    return f.error() == QFile::NoError;
}

//---------------------------------------------------------
//   saveCompareMimeData
//---------------------------------------------------------

bool MTest::saveCompareMimeData(QByteArray mimeData, const QString& saveName, const QString& compareWith)
{
    saveMimeData(mimeData, saveName);
    return compareFiles(saveName, compareWith);
}

//---------------------------------------------------------
//   extractRootFile
//---------------------------------------------------------

extern QString readRootFile(MQZipReader*, QList<QString>&);

void MTest::extractRootFile(const QString& zipFile, const QString& destination)
{
    MQZipReader f(zipFile);
    QList<QString> images;
    const QString rootfile = readRootFile(&f, images);

    if (rootfile.isEmpty()) {
        qDebug("can't find rootfile in: %s", qPrintable(zipFile));
        return;
    }

    const QByteArray ba = f.fileData(rootfile);

    QFile out(destination);
    if (!out.open(QIODevice::WriteOnly)) {
        return;
    }
    out.write(ba);
    out.close();
}

QString MTest::rootPath()
{
    return QString(libmscore_tests_DATA_ROOT);
}

//---------------------------------------------------------
//   initMTest
//---------------------------------------------------------

void MTest::initMTest()
{
    qputenv("QML_DISABLE_DISK_CACHE", "true");

    static mu::framework::GlobalModule globalModule;
    static QList<mu::framework::IModuleSetup*> modules;

    modules
        << new mu::fonts::FontsModule()
    ;

    globalModule.registerResources();
    globalModule.registerExports();
    globalModule.registerUiTypes();
    globalModule.onInit();

    //! NOTE Now we can use logger and profiler

    for (mu::framework::IModuleSetup* m : modules) {
        m->registerResources();
    }

    for (mu::framework::IModuleSetup* m : modules) {
        m->registerExports();
    }

    globalModule.resolveImports();
    for (mu::framework::IModuleSetup* m : modules) {
        m->registerUiTypes();
        m->resolveImports();
    }

    for (mu::framework::IModuleSetup* m : modules) {
        m->onInit();
    }

    globalModule.onStartApp();
    for (mu::framework::IModuleSetup* m : modules) {
        m->onStartApp();
    }

    initMyResources();
//      DPI  = 120;
//      PDPI = 120;
    MScore::noGui = true;

    // synti  = new MasterSynthesizer();
    mscore = new MScore;
    new MuseScoreCore;
    mscore->init();

    root = rootPath();
    loadInstrumentTemplates(":/instruments.xml");
    score = readScore("test.mscx");
}
}