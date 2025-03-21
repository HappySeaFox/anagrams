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

#include "settings.h"
#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    // initial parameters
    ui->lineDictionary->setText(SETTINGS_GET_STRING(SETTING_DICTIONARY));
    ui->lineDictionary->setCursorPosition(0);

    ui->lineWord->setText(SETTINGS_GET_STRING(SETTING_WORD));
    ui->lineWord->setCursorPosition(0);
    ui->lineWord->setValidator(new QRegExpValidator(Settings::wordValidator(), ui->lineWord));

    ui->toolDictionary->setIcon(QIcon::fromTheme("folder-open", QIcon(":/images/folder.png")));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Start"));

    slotCheckData();
}

Options::~Options()
{
    delete ui;
}

QString Options::dictionary() const
{
    return ui->lineDictionary->text().trimmed();
}

QString Options::word() const
{
    return ui->lineWord->text().trimmed();
}

void Options::slotSelectDictionary()
{
    qDebug("Select a dictionary");

    QString dictionary = QFileDialog::getOpenFileName(this,
                                                      tr("Select a dictionary"),
                                                      SETTINGS_GET_STRING(SETTING_LAST_DIRECTORY),
                                                      tr("Text files (*.txt);;All files (*)"));

    if(dictionary.isEmpty())
        return;

    ui->lineDictionary->setText(QDir::toNativeSeparators(dictionary));
    SETTINGS_SET_STRING(SETTING_LAST_DIRECTORY, QFileInfo(dictionary).absolutePath());
}

void Options::slotCheckData()
{
    // - path to the dictionary must not be empty
    // - the entered word must be 3 characters or more
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                !dictionary().isEmpty()
                && word().length() >= Settings::minimumWordLength()
                );
}

void Options::slotAccept()
{
    SETTINGS_SET_STRING(SETTING_DICTIONARY, dictionary());
    SETTINGS_SET_STRING(SETTING_WORD, word());

    accept();
}
