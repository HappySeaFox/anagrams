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

#ifndef SETTINGSBASE_H
#define SETTINGSBASE_H

#include <QtCore>

class SettingsBasePrivate;

/*
 *  Base class for configuration settings
 */
class SettingsBase
{
public:
    ~SettingsBase();

    enum SyncType { NoSync, Sync };

    template <typename T>
    T value(const QString &key);

    template <typename T>
    T value(const QString &key, const T &def);

    template <typename T>
    void setValue(const QString &key, const T &value, SyncType sync = Sync);

    QVariant defaultValue(const QString &key) const;

    /*
     *  Returns 'true' if the setting 'key' exists. If 'key' starts with '/'
     *  the setting is returned from "settings" group (almost all settings are stored there).
     *
     *  For example:
     *
     *  contains("list")  - will look for the "list" key in the global section
     *  contains("/list") - will look for the "list" key in the "settings" section
     */
    bool contains(const QString &key) const;

    /*
     *  Remove the specified key from the section "settings"
     */
    void remove(const QString &key, SyncType sync = Sync);

    /*
     *  Sync settings with a underlying file
     */
    void sync();

protected:
    SettingsBase(const QString &configPath, const QHash<QString, QVariant> &defaultValues = QHash<QString, QVariant>());

private:
    /*
     *  Install some default values. The new values are appended to the existing ones
     */
    void addDefaultValues(const QHash<QString, QVariant> &defaultValues);

    /*
     *  Installed default values
     */
    const QHash<QString, QVariant> &defaultValues();

    QSettings *settings() const;

private:
    SettingsBasePrivate *d;
};

/**********************************/

template <typename T>
T SettingsBase::value(const QString &key)
{
    T def = T();
    QHash<QString, QVariant>::const_iterator it = defaultValues().find(key);

    if(it != defaultValues().end())
        def = it.value().value<T>();

    return value<T>(key, def);
}

template <typename T>
T SettingsBase::value(const QString &key, const T &def)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    QVariant cachedValue = s->value(key, QVariant::fromValue(def));
    s->endGroup();

    return cachedValue.value<T>();
}

template <typename T>
void SettingsBase::setValue(const QString &key, const T &newValue, SettingsBase::SyncType sync)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    s->setValue(key, QVariant::fromValue(newValue));
    s->endGroup();

    if(sync == Sync)
        s->sync();
}

#endif // SETTINGSBASE_H
