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

#include "mainwindow.h"

/*
 *  This is the algorithm used to search anagram derivations:
 *
 *  1) load the selected dictionary with DictionaryLoader. The object of this class reads
 *  the selected dictionary line by line and constructs an internal hash. Every word
 *  obtained from the dictionary is sorted by characters. Then it is used as a key in the hash.
 *  The unsorted word gets appended to the corresponding value in the hash.
 *  The corresponding value in the hash is a list of unsorted words (and yes, this is a
 *  list of possible anagrams!)
 *
 *  Here it is an abstract example of a dictionary file:
 *
 *  betas
 *  owla
 *  beast
 *  lowa
 *  abets
 *  beats
 *  ...
 *
 *  It is parsed and saved internally as follows:
 *
 *  alow  -> [ owla, lowa ]
 *  abest -> [ beast, abets, beats, betas ]
 *  ...
 *
 *  We need this to get the possible anagrams of the selected word and do that quickly. If we
 *  need to get anagrams of "owla" ("owl" + 'a'), we sort "owla" by characters (to "alow")
 *  and get the corresponding anagrams [owla,lowa] from the hash.
 *
 *  2) Search the longest chain in the hash with AnagramFinder. AnagramFinder adds a single Latin
 *  character (from 'a' to 'z') to the word and gets all its anagrams, checks their lengths,
 *  and repeats the search recursively with the first found anagram. Pseudo code:
 *
 *  hash = parseDictionary()
 *
 *  function getAnagrams(word)
 *  {
 *      return hash.value(sort(word))
 *  }
 *
 *  function findAnagrams(word)
 *  {
 *      for char in 'a'...'z'
 *      {
 *          anagrams = getAnagrams(word + char)
 *
 *          for anagram in anagrams
 *          {
 *              checkAndCacheNewLongestChain(anagram)
 *          }
 *
 *          if anagrams is not empty
 *              findAnagrams(sort(anagrams[0]))
 *      }
 *  }
 *
 *  findAnagrams(sort("owl"))
 *
 *  AnagramFinder searches the longest chain of anagram derivations and stores it in a local
 *  vector like that:
 *
 *  owl -> lowe -> rowel -> orwell -> ... -> wholesalers
 *
 *  If it finds multiple longest chains with the same length, it stores all of them.
 *
 *  We use only the first anagram because we walk the same way for the rest of them, e.g.
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
 *
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
 *
 *  3) When AnagramFinder finishes, we display all the found chains with SearchResults
 *
 *  Notes:
 *
 *  - Only English words are allowed in dictionaries. Non-English words are filtered out
 *  - We use std::string as a string data type to improve performance. Since we don't allow
 *    non-English characters, using QString (which is UTF-16 internally) is a huge overhead. Using
 *    std::string allows to greatly improve the performance of the search algorithm
 *  - The search time displayed in the app doesn't include the time required to read and parse
 *    a dictionary
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
