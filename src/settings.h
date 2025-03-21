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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

#include "settingsbase.h"

#define SETTINGS_CONTAINS Settings::instance()->contains

#define SETTINGS_GET_STRING Settings::instance()->value<QString>
#define SETTINGS_SET_STRING Settings::instance()->setValue<QString>

#define SETTINGS_REMOVE Settings::instance()->remove

#define SETTING_WORD           "word"
#define SETTING_DICTIONARY     "dictionary"
#define SETTING_LAST_DIRECTORY "last-directory"
/*
 *  Application settings and global parameters like validation regexp
 */
class Settings : public SettingsBase
{
public:
    static
    Settings *instance();

    /*
     *  Regexp to validate the entered word. It allows only
     *  Latin characters
     */
    static
    QRegExp wordValidator();

    /*
     *  The minimum length of the entered word
     */
    static inline
    int minimumWordLength();

private:
    Settings();

    QHash<QString, QVariant> defaultValues();
};

int Settings::minimumWordLength()
{
    return 3;
}

#endif // SETTINGS_H
