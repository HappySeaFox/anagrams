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

#ifndef ANAGRAMFINDERTHREAD_H
#define ANAGRAMFINDERTHREAD_H

#include "anagrams.h"
#include "longestpaths.h"

/*
 *  A thread to find anagrams of the desired word in the specified dictionary hash
 */
class AnagramFinderThread : public QThread
{
public:
    AnagramFinderThread(const Anagrams &anagrams, const QString &word, QObject *parent = nullptr);

    /*
     *  Result as a list of chains like that:
     *
     *  owl -> lowe -> ...
     *  owl -> awol -> ...
     *
     *  Later we load them into SearchResults
     */
    inline
    LongestPaths longestPaths() const;

    /*
     *  Time spent on the algorithm
     */
    inline
    qint64 algorithmTime() const;

protected:
    virtual void run() override;

private:
    /*
     *  Recursive search algorithm
     */
    void prepareAndFindAnagrams(const std::string &word);
    void findAnagrams(const std::string &word);

    /*
     *  Add the specified character to the string
     */
    std::string addCharIntoSortedString(const char &ch, const std::string &word) const;

    bool longestAnagramAlreadyCached(const std::string &anagram) const;

    /*
     *  Split m_path and save into m_splittedPaths
     */
    void updateSplittedPathsWithCurrentPath();

private:
    Anagrams m_anagrams;
    QString m_word;
    // current chain
    LongestPath m_path;
    // all longest chains found
    LongestPaths m_longestPaths;
    // the longest found anagram in m_longestPaths (for performance optimization)
    std::string m_longestAnagramInCache;
    // splitted unique parts of m_longestPaths (for performance optimization)
    QSet<std::string> m_splittedPaths;
    // time to compute the whole algorihtm. We display this valus in the main UI
    qint64 m_algorithmTime;
};

LongestPaths AnagramFinderThread::longestPaths() const
{
    if(isRunning())
    {
        qWarning("The thread %p is still running, returning an empty result", static_cast<const void*>(this));
        return LongestPaths();
    }

    return m_longestPaths;
}

qint64 AnagramFinderThread::algorithmTime() const
{
    return m_algorithmTime;
}

#endif // ANAGRAMFINDERTHREAD_H
