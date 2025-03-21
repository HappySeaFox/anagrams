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

#include <algorithm>

#include "dictionaryloader.h"
#include "settings.h"
#include "utils.h"
#include "ui_dictionaryloader.h"

DictionaryLoader::DictionaryLoader(const QString &dictionaryPath, QWidget *parent)
    : NonRejectableDialog(parent)
    , ui(new Ui::DictionaryLoader)
    , m_error(false)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    ui->progress->startAnimation();

    m_dictionaryFile.setFileName(dictionaryPath);

    // cannot open - this is a fatal error
    if(!m_dictionaryFile.open(QFile::ReadOnly))
    {
        m_errorString = m_dictionaryFile.errorString();
        m_error = true;
        return;
    }

    nextIteration();
}

DictionaryLoader::~DictionaryLoader()
{
    delete ui;
}

void DictionaryLoader::updateProgress()
{
    ui->labelProgress->setText(QString("%1%")
                               .arg(static_cast<int>((static_cast<double>(m_dictionaryFile.pos()) / m_dictionaryFile.size()) * 100)));
}

void DictionaryLoader::nextIteration()
{
    QTimer::singleShot(0, this, SLOT(slotNextIteration()));
}

void DictionaryLoader::slotNextIteration()
{
    constexpr int MAX_LINES = 1000;
    int i = 0;

    updateProgress();

    while(!m_dictionaryFile.atEnd() && i++ < MAX_LINES)
    {
        QByteArray line = m_dictionaryFile.readLine().trimmed().toLower();

        // ignore invalid/not supported words
        if(!Settings::wordValidator().exactMatch(line))
            continue;

        // sort the word and add it to the dictionary hash as a
        // 'sorted' -> 'original words' mapping. Like that:
        // low -> [owl, low, wol...]
        std::string sortedLine = line.constData();
        std::sort(sortedLine.begin(), sortedLine.end());
        m_anagrams[sortedLine].append(std::string(line.constData()));
    }

    if(!m_dictionaryFile.atEnd())
        nextIteration();
    else
    {
        qDebug("Dictionary has been loaded. Size: %d", m_anagrams.size());

        if(m_anagrams.isEmpty())
        {
            Utils::error(tr("The dictionary is empty"), this);
            reject();
            return;
        }

        accept();
    }
}
