/*  This file is part of ANAGRAMS (https://github.com/HappySeaFox/anagrams)

    Copyright (c) 2025 Dmitry Baryshev

    The MIT License

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "dictionaryloader.h"
#include "anagramfinder.h"
#include "mainwindow.h"
#include "settings.h"
#include "options.h"
#include "utils.h"
#include "about.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // PROJECT_* strings are defined in the cmake project file
    setWindowTitle(tr(PROJECT_LABEL));

    ui->actionStart_test->setShortcut(QKeySequence::New);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionAbout->setShortcut(QKeySequence::HelpContents);

    QTimer::singleShot(0, this, SLOT(slotConfigureAndStart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDictionary()
{
    qDebug("Load dictionary");

    // parse the selected dictionary
    DictionaryLoader loader(SETTINGS_GET_STRING(SETTING_DICTIONARY), this);

    if(loader.error())
    {
        Utils::error(tr("Cannot open the selected dictionary: %1").arg(loader.errorString()), this);
        return;
    }

    if(loader.exec() != DictionaryLoader::Accepted)
        return;

    // parsed dictionary
    m_anagrams = loader.anagrams();

    QString word = SETTINGS_GET_STRING(SETTING_WORD);

    // search anagrams we need in the parsed dictionary
    AnagramFinder finder(m_anagrams, word, this);

    if(finder.exec() != AnagramFinder::Accepted)
    {
        ui->searchResults->clear();
        return;
    }

    // finally, display results
    ui->searchResults->loadResults(word, finder.longestPaths(), finder.algorithmTime());
}

void MainWindow::slotAbout()
{
    qDebug("About");

    About about(this);
    about.exec();
}

void MainWindow::slotAboutQt()
{
    qDebug("About Qt");
    QMessageBox::aboutQt(this);
}

void MainWindow::slotConfigureAndStart()
{
    qDebug("Configure and start");

    Options options(this);

    if(options.exec() == Options::Accepted)
        loadDictionary();
}

void MainWindow::slotQuit()
{
    qDebug("Quit");
    qApp->quit();
}
