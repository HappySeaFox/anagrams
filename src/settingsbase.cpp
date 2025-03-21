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

#include "settingsbase.h"

class SettingsBasePrivate
{
public:
    QSettings *settings;
    QHash<QString, QVariant> defaultValues;
};

/*******************************************************/

SettingsBase::SettingsBase(const QString &configPath, const QHash<QString, QVariant> &defaultValues)
{
    d = new SettingsBasePrivate;

    addDefaultValues(defaultValues);

    if(!configPath.isEmpty())
    {
        // create leading directories
        QDir().mkpath(QFileInfo(configPath).canonicalPath());
        d->settings = new QSettings(configPath, QSettings::IniFormat);
    }
    else
        d->settings = new QSettings(QSettings::IniFormat,
                                    QSettings::UserScope,
                                    QCoreApplication::organizationName(),
                                    QCoreApplication::applicationName());

    qDebug("Configuration file: %s", qPrintable(d->settings->fileName()));

    d->settings->setFallbacksEnabled(false);
}

void SettingsBase::addDefaultValues(const QHash<QString, QVariant> &defaultValues)
{
    QHash<QString, QVariant>::const_iterator itEnd = defaultValues.end();

    for(QHash<QString, QVariant>::const_iterator it = defaultValues.begin();it != itEnd;++it)
    {
        d->defaultValues.insert(it.key(), it.value());
    }
}

const QHash<QString, QVariant> &SettingsBase::defaultValues()
{
    return d->defaultValues;
}

SettingsBase::~SettingsBase()
{
    delete d->settings;
    delete d;
}

QVariant SettingsBase::defaultValue(const QString &key) const
{
    return d->defaultValues.value(key);
}

bool SettingsBase::contains(const QString &key) const
{
    return d->settings->contains(key.startsWith('/') ? ("settings" + key) : key);
}

void SettingsBase::remove(const QString &key, SettingsBase::SyncType sync)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    s->remove(key);
    s->endGroup();

    if(sync == Sync)
        s->sync();
}

void SettingsBase::sync()
{
    d->settings->sync();
}

QSettings *SettingsBase::settings() const
{
    return d->settings;
}
