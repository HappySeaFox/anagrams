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

#include "searchresultlabel.h"
#include "searchresults.h"

SearchResults::SearchResults(QWidget *parent)
    : QScrollArea(parent)
{
}

void SearchResults::loadResults(const QString &word, const LongestPaths &paths, qint64 elapsedTime)
{
    qDebug("Loading search results");

    clear();

    if(paths.isEmpty())
        return;

    widget()->setUpdatesEnabled(false);

    // header with overview
    QLabel *header = new QLabel(
                         tr("Search time: <font color=green><b>%1ms</b></font>. "
                            "Found chain(s): <font color=green><b>%2</b></font>. "
                            "Longest word: <font color=green><b>%3</b></font> character(s)"
                            )
                            .arg(timeWithQuotes(elapsedTime))
                            .arg(paths.size())
                            .arg(paths.first().top().length())
                         , widget());
    header->adjustSize();
    widget()->layout()->addWidget(header);

    // also calculate the required width and height of the container
    // to display scrollbars if necessary
    int maxWidth = header->width();
    int maxHeight = header->height();

    // display the actual results
    for(const LongestPath &stack : paths)
    {
        QStringList words = QStringList() << word;

        // +1 = starting word that is not included in the chain
        words.reserve(stack.size()+1);

        for(const std::string &str : stack)
            words.append(QString::fromStdString(str));

        SearchResultLabel *label = new SearchResultLabel(words, widget());

        widget()->layout()->addWidget(label);

        if(label->width() > maxWidth)
            maxWidth = label->width();

        maxHeight += label->height() + widget()->layout()->spacing();
    }

    // add a vertical spacer
    widget()->layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // resize the container to its contents to show the scrollbars if necessary
    widget()->setFixedWidth(maxWidth + widget()->layout()->margin()*2);
    widget()->setFixedHeight(maxHeight + widget()->layout()->margin()*2);

    widget()->setUpdatesEnabled(true);
}

void SearchResults::clear()
{
    // remove previous labels & layout
    QWidget *widget = new QWidget;
    widget->setLayout(new QVBoxLayout);
    setWidget(widget);
}

QString SearchResults::timeWithQuotes(qint64 elapsedTime) const
{
    QString result = QString::number(elapsedTime);

    for(int i = result.length()-3;i > 0;i -= 3)
    {
        result.insert(i, QChar('\''));
    }

    return result;
}
