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

#ifndef SEARCHRESULTS_H
#define SEARCHRESULTS_H

#include <QtWidgets>
#include <QtCore>

#include "longestpaths.h"

/*
 *  Widget to display search results as a number of chains like
 *
 *  owl -> lowe -> ...
 *  owl -> awol -> ...
 *
 *  Each chain is represented by SearchResultLabel
 */
class SearchResults : public QScrollArea
{
public:
    explicit SearchResults(QWidget *parent = nullptr);

    void loadResults(const QString &word, const LongestPaths &paths, qint64 elapsedTime);

    void clear();

private:
    /*
     *  Convert 48550 to "48'550"
     */
    QString timeWithQuotes(qint64 elapsedTime) const;
};

#endif // SEARCHRESULTS_H
