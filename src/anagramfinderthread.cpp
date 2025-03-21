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

#include "anagramfinderthread.h"

/*
 *  Hardcode the alphabet since the character table is not continuous
 *  according to the C++ standard, so we cannot reliably do
 *
 *  char ch = 'a';
 *  ch++; // get 'b'
 */
constexpr const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

AnagramFinderThread::AnagramFinderThread(const Anagrams &anagrams, const QString &word, QObject *parent)
    : QThread(parent)
    , m_anagrams(anagrams)
    , m_word(word)
    , m_algorithmTime(0)
{
    // reduce allocations a little bit
    m_path.reserve(32);
}

void AnagramFinderThread::run()
{
    QElapsedTimer algorithmTimeCounter;
    algorithmTimeCounter.start();

    prepareAndFindAnagrams(m_word.toLower().toStdString());

    m_algorithmTime = algorithmTimeCounter.elapsed();
}

void AnagramFinderThread::prepareAndFindAnagrams(const std::string &word)
{
    // sort the word now to save CPU time later
    auto sortedWord = word;

    std::sort(sortedWord.begin(), sortedWord.end());

    findAnagrams(sortedWord);
}

void AnagramFinderThread::findAnagrams(const std::string &word)
{
    const char *alphabetIndex = alphabet;

    while(*alphabetIndex)
    {
        // add a single character from the alphabet and find its anagrams. Repeat the search
        // recursively with the first anagram. When recursion has finished, return here and add
        // the next character from the alphabet and find its anagrams etc.
        const auto &anagrams = m_anagrams.value(addCharIntoSortedString(*alphabetIndex++, word));

        for(int i = 0;i < anagrams.size();i++)
        {
            const std::string &anagram = anagrams[i];
            bool dive = true;

            m_path.push(anagram);

            // longest chain - update the cache with it. We can check the length only
            // of the first anagram
            if(i == 0 && anagram.length() > m_longestAnagramInCache.length())
            {
                m_longestPaths = LongestPaths() << m_path;
                m_splittedPaths.clear();
                m_longestAnagramInCache = anagram;

                updateSplittedPathsWithCurrentPath();
            }

            // check if we need to cache the chain with the same length
            if(anagram.length() == m_longestAnagramInCache.length())
            {
                // cache a new longest anagram with its chain
                if(!longestAnagramAlreadyCached(anagram))
                {
                    m_longestPaths.append(m_path);
                    updateSplittedPathsWithCurrentPath();
                }
            }
            // since we allow recursion for the first anagram only, allow cache checking
            // for the first anagram too
            else if(i == 0)
            {
                // the anagram is not the longest anagram currently found, and it is presented
                // somewhere in the cached longest chains, so we just ignore it and don't dive
                // into recursion to avoid a duplicate path walk
                if(m_splittedPaths.contains(anagram))
                    dive = false;
            }

            /*
             *  dive deeper for the first anagram only. We use the first anagram only because
             *  we walk the same way for the rest of them, e.g.
             *
             *               anagrams   sort   sorted
             *                  ↓        ↓       ↓
             *   owl + a +---> awol --------+-> alow + a -> ...
             *           |                  +-> alow + b -> ...
             *           |                  +-> alow + c -> ...
             *           |
             *           +---> wola --------+-> alow + a -> ... DUPLICATE!
             *           |                  +-> alow + b -> ... DUPLICATE!
             *           |                  +-> alow + c -> ... DUPLICATE!
             *           |
             *           +---> lowa --------+-> alow + a -> ... DUPLICATE!
             *                              +-> alow + b -> ... DUPLICATE!
             *                              +-> alow + c -> ... DUPLICATE!
             *   owl + b +---> ...
             *
             *  This is why we can reduce the unnecessary path walks to
             *
             *   owl + a +---> awol --------+-> alow + a -> ...
             *           |                  +-> alow + b -> ...
             *           |                  +-> alow + c -> ...
             *           +---> wola
             *           +---> lowa
             *
             *   owl + b +---> ...
             */
            if(i == 0 && dive)
                prepareAndFindAnagrams(anagram);

            m_path.pop();
        }
    }
}

std::string AnagramFinderThread::addCharIntoSortedString(const char &ch, const std::string &word) const
{
    std::string newWord = word;

    // find a correct place to insert a new character into the sorted word
    auto it = std::lower_bound(newWord.begin(), newWord.end(), ch, std::less<char>());

    if(it == newWord.end())
        newWord.append(1, ch);
    else
        newWord.insert(it, 1, ch);

    return newWord;
}

bool AnagramFinderThread::longestAnagramAlreadyCached(const std::string &anagram) const
{
    for(const LongestPath &stack : m_longestPaths)
    {
        if(stack.top() == anagram)
            return true;
    }

    return false;
}

void AnagramFinderThread::updateSplittedPathsWithCurrentPath()
{
    for(const auto &str : m_path)
        m_splittedPaths.insert(str);
}
