## Anagram derivation finder

<p align="center">
  <a href=".github/demo.jpg"><img src=".github/demo.jpg?raw=true" alt="Demo Screenshot"/></a>
</p>

An anagram derivation is a N-letter word derived from a N-1 letter word by adding a letter and
rearranging. For example, here is a derivation of "aliens": `ail + s = sail + n = nails + e = aliens`

Anagram derivation finder finds the longest such derivation from a specific 3-letter word (one word
provided by user) in a list of words where every derived word also exists in the list of words.
words = `{ail, tennis, nails, desk, aliens, table, engine, sail, etc....}`.

An English dictionary can be downloaded from here: https://github.com/dwyl/english-words

QProgressIndicator is taken from https://github.com/mojocorp/QProgressIndicator

## Implementation details

This is the algorithm implemented to search anagram derivations:

### Build hash

Load the selected dictionary with `DictionaryLoader`. The object of this class reads the selected dictionary line by line and constructs an internal hash.

Every word obtained from the dictionary gets sorted by characters. Then it is used as a key in the hash.

The unsorted word gets appended to the corresponding value in the hash.

The corresponding value in the hash is a list of unsorted words (and yes, this is a list of possible anagrams!).

Here it is an abstract example of a dictionary file:
```
betas
owla
beast
lowa
bets
beats
...
```
It is parsed and saved internally as follows:
```
alow  -> [ owla, lowa ]
abest -> [ beast, abets, beats, betas ]
...
```
This way we get the possible anagrams of the selected word and do that quickly. If we need to get the anagrams of the word "owla" ("owl" + 'a'), we sort "owla" by characters (to "alow") and get the corresponding anagrams `[owla,lowa]` from the hash.

### Search the hash

Search the longest chain in the hash with `AnagramFinder`.

AnagramFinder adds a single Latin character (from 'a' to 'z') to the word and gets all its anagrams, checks their lengths, and repeats the search recursively with the first found anagram. Pseudo code:
```
hash = parseDictionary()

function getAnagrams(word)
{
    return hash.value(sort(word))
}

function findAnagrams(word)
{
    for char in 'a'...'z'
    {
        anagrams = getAnagrams(word + char)

        for anagram in anagrams
        {
            checkAndCacheNewLongestChain(anagram)
        }

        if anagrams is not empty
            findAnagrams(sort(anagrams[0]))
    }
}

findAnagrams(sort("owl"))
```
`AnagramFinder` searches the longest chain of anagram derivations and stores it in a local vector like that:
```
owl -> lowe -> rowel -> orwell -> ... -> wholesalers
```
If it finds multiple longest chains with the same length, it stores all of them.

We use only the first anagram because we walk the same way for the rest of them, e.g.
```
             anagrams   sort   sorted
                ↓        ↓       ↓
 owl + a +---> awol --------+-> alow + a -> ...
         |                  +-> alow + b -> ...
         |                  +-> alow + c -> ...
         |
         +---> wola --------+-> alow + a -> ... DUPLICATE!
         |                  +-> alow + b -> ... DUPLICATE!
         |                  +-> alow + c -> ... DUPLICATE!
         |
         +---> lowa --------+-> alow + a -> ... DUPLICATE!
                            +-> alow + b -> ... DUPLICATE!
                            +-> alow + c -> ... DUPLICATE!

 owl + b +---> ...
```
This is why we can reduce the unnecessary path walks to
```
 owl + a +---> awol --------+-> alow + a -> ...
         |                  +-> alow + b -> ...
         |                  +-> alow + c -> ...
         +---> wola
         +---> lowa

 owl + b +---> ...
```

### Display results

When `AnagramFinder` finishes, we display all the found chains with `SearchResults`.

Notes:
- Only English words are allowed in dictionaries. Non-English words are filtered out.
- We use `std::string` as a string data type to improve performance. Since we don't allow non-English characters, using `QString` (which is UTF-16 internally) is a huge overhead. Using `std::string` allows to greatly improve the performance of the search algorithm.
- The search time displayed in the app doesn't include the time required to read and parse a dictionary.

## Author

Dzmitry Baryshau

## License

Released under the MIT license.
