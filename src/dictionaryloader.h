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

#ifndef DICTIONARYLOADER_H
#define DICTIONARYLOADER_H

#include "anagrams.h"

#include <QtWidgets>

#include "nonrejectabledialog.h"

namespace Ui
{
    class DictionaryLoader;
}

/*
 *  Dialog to load and parse the specified dictionary. A dictionary
 *  may be huge, that's why we use a blocking (dialog) mechanism. DictionaryLoader reads
 *  file by chunks of 1000 lines
 *
 *  Returns a hash as a result. Hash format is documented in main.cpp
 */
class DictionaryLoader : public NonRejectableDialog
{
    Q_OBJECT

public:
    explicit DictionaryLoader(const QString &dictionaryPath, QWidget *parent = nullptr);
    ~DictionaryLoader();

    /*
     *  If an error has occured in parsing, error() returns true
     */
    inline
    bool error() const;

    /*
     *  Actual error when error() is true
     */
    inline
    QString errorString() const;

    /*
     *  Actual parse result
     */
    inline
    Anagrams anagrams() const;

private:
    void updateProgress();
    // load a next bunch of lines
    void nextIteration();

private slots:
    void slotNextIteration();

private:
    Ui::DictionaryLoader *ui;
    Anagrams m_anagrams;
    QFile m_dictionaryFile;
    bool m_error;
    QString m_errorString;
};

bool DictionaryLoader::error() const
{
    return m_error;
}

QString DictionaryLoader::errorString() const
{
    return m_errorString;
}

Anagrams DictionaryLoader::anagrams() const
{
    return m_anagrams;
}

#endif // DICTIONARYLOADER_H
