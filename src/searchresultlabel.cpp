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

SearchResultLabel::SearchResultLabel(const QStringList &words, QWidget *parent)
    : QWidget(parent)
{
    new QHBoxLayout(this);

    const int arrowsNumber = words.size() - 1;
    int arrowsCreated = 0;

    for(const QString &word : words)
    {
        QLabel *label = new QLabel(word, this);
        label->setAlignment(Qt::AlignCenter);
        layout()->addWidget(label);

        if(arrowsCreated++ < arrowsNumber)
        {
            label = new QLabel(this);
            label->setAlignment(Qt::AlignCenter);

            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(arrowPixmap());
            layout()->addWidget(label);
        }
    }

    layout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

    adjustSize();
    setFixedSize(sizeHint());
}

QPixmap SearchResultLabel::arrowPixmap() const
{
    constexpr const char *arrowPath = ":/images/arrow.png";

    QPixmap pixmap;

    if(!QPixmapCache::find(arrowPath, &pixmap))
    {
        pixmap.load(arrowPath);
        QPixmapCache::insert(arrowPath, pixmap);
    }

    return pixmap;
}
