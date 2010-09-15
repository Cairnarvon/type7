This is a tool to "encrypt" passwords with Cisco's type 7 "encryption", and to reverse such "encryption".
In your Cisco IOS configuration, these passwords will generally look something like this:

>     enable password 7 0822455D0A16

Pass this password to the application as a command line argument, or through stdin:

>     $ ./unseven 0822455D0A16
>     0822455D0A16    cisco

Also included is an encryption tool, in case you need it, which works the same way:

>     $ ./enseven cisco
>     cisco   05080F1C2243

Type 7 encryption is quite simple: a random number from 0 to 15 is the encryption key, and makes up the first two digit of the ciphertext. This is used as an index to a lookup table. The algorithm loops through each character in the plaintext, XORing it with a character from the lookup table, and incrementing the index. These characters are converted to hexadecimal format, and make up the rest of the ciphertext.

This implementation is significant for being the only one I'm aware of that includes the full forty-character lookup table.

## Building

If you have GNU `make` installed, just invoking `make` will build both the encryptor (`enseven`) and the decryptor (`unseven`). It assumes a POSIX system (for `fileno`; `-D_POSIX_C_SOURCE`) and a GNU system (for `getline`; `-D_GNU_SOURCE`).

## Caveats

If it's not obvious: type 7 is not a strong cipher. Cisco's official line is that it's only to foil shoulder surfers, not to provide actual security; don't use it. The encryptor is only provided as a curiosity.

As per Cisco's implementation, trying to encrypt a password longer than 25 characters will truncate it.
