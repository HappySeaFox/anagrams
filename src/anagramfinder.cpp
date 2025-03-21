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

#include <QtCore>

#include "anagramfinderthread.h"
#include "anagramfinder.h"
#include "anagrams.h"
#include "ui_anagramfinder.h"

AnagramFinder::AnagramFinder(const Anagrams &anagrams, const QString &word, QWidget *parent)
    : NonRejectableDialog(parent)
    , ui(new Ui::AnagramFinder)
    , m_thread(new AnagramFinderThread(anagrams, word, this))
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    ui->progress->startAnimation();

    // when the thread finishes(), we close the dialog
    connect(m_thread, SIGNAL(finished()), this, SLOT(accept()));

    // delayed start
    QTimer::singleShot(0, this, SLOT(slotStart()));
}

AnagramFinder::~AnagramFinder()
{
    delete ui;
}

LongestPaths AnagramFinder::longestPaths() const
{
    return m_thread->longestPaths();
}

qint64 AnagramFinder::algorithmTime() const
{
    return m_thread->algorithmTime();
}

void AnagramFinder::slotStart()
{
    m_thread->start();
}
