## Anagram derivation finder.

<p align="center">
  <a href=".github/demo.jpg"><img src=".github/demo.jpg?raw=true" alt="Demo Screenshot"/></a>
</p>

An anagram derivation is a N-letter word derived from a N-1 letter word by adding a letter and
rearranging. For example, here is a derivation of "aliens": `ail + s = sail + n = nails + e = aliens`

Write a program that will find the longest such derivation from a specific 3-letter word (one word
provided by user) in a list of words where every derived word also exists in the list of words.
words = `{ail, tennis, nails, desk, aliens, table, engine, sail, etc....}`.

Requirements:
  - fully working application - GUI or command line
  - documentation of used algorithm (can be in a form of comments in code)
  - application should be able to operate on external dictionary - text file where each line is
    another word. User should be able to choose the dictionary at runtime (or as command
    line argument)
  - application should show derivation chain when results are shown. It can be
    ail+s=sail+n=... or with arrows: ails->sail->... or any similar way
  - if there is more than 1 longest derivation (many words of the same length), the
    application can show just one result. If it will show all, we’ll be extra happy

An English dictionary can be downloaded from here: https://github.com/dwyl/english-words

QProgressIndicator is taken from https://github.com/mojocorp/QProgressIndicator