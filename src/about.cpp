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

#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::About)
{
    ui->setupUi(this);

    // PROJECT_* strings are defined in the cmake project file
    ui->labelProduct->setText(PROJECT_LABEL);
    ui->labelVersion->setText(tr("Version %1").arg(PROJECT_VERSION));
    ui->labelIcon->setPixmap(QIcon(":/images/" PROJECT_NAME ".ico").pixmap(48, 48));

    // %1 will be replaced with the product name
    ui->labelWarranty->setText(tr("%1 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.")
                               .arg(PROJECT_LABEL));

#ifdef Q_OS_MAC
    ui->pushOK->hide();
#else
    setWindowTitle(tr("About"));
#endif

    adjustSize();
}

About::~About()
{
    delete ui;
}
