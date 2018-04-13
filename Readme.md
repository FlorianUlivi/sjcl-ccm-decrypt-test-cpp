# SJCL ccm decrypt test

This is a test program for learning the OpenSSL API to decrypt a message
encrypted with [sjcl](http://bitwiseshiftleft.github.io/sjcl/).



- password: "password"
- salt: 3B889F6A 5CE3FC4B
- key: 7DED638F DD30ACDB 4A34D153 48F12D95 BB388A7E 8CB9C35B 8D9C9009 89454E1E
- iv: ECCFB278 E13D4DA6 695BBD75 77C61F7C
- message: "Hallo Welt!"

The json data was created with this
[demo](https://bitwiseshiftleft.github.io/sjcl/demo/).

``` json
{"iv":"7M+yeOE9TaZpW711d8YffA==",
"v":1,
"iter":1000,
"ks":256,
"ts":64,
"mode":"ccm",
"adata":"",
"cipher":"aes",
"salt":"O4ifalzj/Es=",
"ct":"Lsbc3V06Fruulq83CRSiD2dohA=="}
```

[This](https://stackoverflow.com/questions/23074176/crypto-is-sjcl-javascript-encryption-compatible-with-openssl) SO post was a great help.
