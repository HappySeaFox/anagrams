## Anagram derivation finder.

<p align="center">
  <a href=".github/demo.jpg"><img src=".github/demo.jpg?raw=true" alt="Demo Screenshot"/></a>
</p>

An anagram derivation is a N-letter word derived from a N-1 letter word by adding a letter and
rearranging. For example, here is a derivation of "aliens": `ail + s = sail + n = nails + e = aliens`

Write a program that will find the longest such derivation from a specific 3-letter word (one word
provided by user) in a list of words where every derived word also exists in the list of words.
words = `{ail, tennis, nails, desk, aliens, table, engine, sail, etc....}`.

An English dictionary can be downloaded from here: https://github.com/dwyl/english-words

QProgressIndicator is taken from https://github.com/mojocorp/QProgressIndicator