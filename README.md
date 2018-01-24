# Huffman String Encoding

This is a simple implementation of the Huffman string encoding algorithm designed to be embedded into other programs.

The current implementation is somewhat bare-bones and is based on an initial (and not entirely functional) implementation I'd written a few years ago and recently (as of 2018) rediscovered. It should, in its current form, work for ASCII strings, but may not work as expected on Unicode data.

# Future Plans

At some point in the future, this will be extended to properly support encoding UTF8 data.

In addition, the ability to load and save the Huffman table data is planned for the future as well, both in a [Glulx](https://www.eblong.com/zarf/glulx/) compatible format and possibly in a more general format.

# License

This code is released under the MIT license and is free to use in any way and for any purpose.
