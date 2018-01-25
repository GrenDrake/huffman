# Huffman Encoding

This is a simple implementation of the Huffman encoding algorithm and is designed to be embedded into other programs. This implementation is specifically intended for use with string data, but could be readily modified to deal with binary data.

The current implementation is somewhat bare-bones and is based on an initial (and not entirely functional) implementation I'd written a few years ago while experimenting with the [Glulx](https://www.eblong.com/zarf/glulx/) virtual machine and recently (as of 2018) rediscovered. It should work for ASCII or UTF8 strings, but may not work as expected with other Unicode encodings or with non-Unicode encodings.

# Future Plans

My main future plan is to add the ability to load and save the Huffman table data, both in a Glulx compatible format and possibly in a more general format.

# License

This code is released under the MIT license and is free to use in any way and for any purpose.
