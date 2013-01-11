# Type 7

This is a tool to encrypt passwords with Cisco's type 7 encryption, and to reverse such encryption.

## Building

If you have GNU `make` installed, just invoking `make` will build both the encryptor (`enseven`) and the decryptor (`unseven`). It assumes a POSIX system (for `fileno`; `-D_POSIX_C_SOURCE`).

## Using

In your Cisco IOS configuration, type 7 passwords will generally look something like this:

>     enable password 7 0822455D0A16

To decrypt this password, pass it to the application as a command line argument, or through stdin:

>     $ ./unseven 0822455D0A16
>     0822455D0A16    cisco

Also included is an encryption tool, in case you need it, which works the same way:

>     $ ./enseven cisco class
>     cisco   05080F1C2243
>     class   13061B13181F

This implementation is significant for being the only one I'm aware of that includes the full forty-character lookup table.

## Algorithm

The type 7 algorithm is as follows:

1. Truncate the input to 25 characters.
2. Choose a key. This is a number between 0 and 15, and will make up the first two characters of the ciphertext.
3. XOR each character of the plaintext with an entry in a look-up table. If your key is 12, for example, the first character will be XORed with the entry at index 12, the second with the one at index 13, &c.
4. Convert each resulting number to a pair of hexadecimal digits. This is the rest of the ciphertext.

Decryption surprisingly is the same thing in reverse.

## Caveats

If it's not obvious: type 7 is not a strong cipher. Cisco's official line is that it's only to foil shoulder surfers, not to provide actual security; don't use it yourself. The encryptor is only provided as a curiosity.

As per Cisco's implementation, trying to encrypt a password longer than 25 characters will truncate it.

In case you're wondering: type 5 encryption is just a `passwd`-style MD5 hash with a four-character salt. To create one, use [OpenSSL](http://www.openssl.org/):

>     openssl passwd -1 -salt `openssl rand -base64 3` PASSWORD

There is no easy way to crack these; try a tool like [John the Ripper](http://www.openwall.com/john/).
