#include <algorithm>
#include <array>
#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string>
#include <vector>

std::vector<uint8_t> decode_b64(const std::string &in) {

  BIO *b64f = BIO_new(BIO_f_base64());
  BIO *buff = BIO_new_mem_buf(in.data(), in.size());
  buff = BIO_push(b64f, buff);

  BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
  BIO_set_close(buff, BIO_CLOSE);
  std::vector<uint8_t> out(in.size());
  size_t out_len = BIO_read(buff, out.data(), in.size());
  out.resize(out_len);
  BIO_free_all(buff);
  return out;
}

int main(void) {
  const std::string pw = "password";
  const std::string b64_ct = R"(Lsbc3V06Fruulq83CRSiD2dohA==)";
  const std::string b64_iv = R"(7M+yeOE9TaZpW711d8YffA==)";
  const std::string b64_salt = R"(O4ifalzj/Es=)";
  const size_t ts = 64; // bit

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_ccm(), nullptr, nullptr, nullptr);
  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_IVLEN, 15 - 2, nullptr);

  std::vector<uint8_t> tag;
  std::vector<uint8_t> ct;
  {
    const auto tmp = decode_b64(b64_ct);
    const size_t tag_len = ts / 8;
    const size_t ct_len = tmp.size() - tag_len;
    tag.resize(tag_len);
    std::copy_n(begin(tmp) + ct_len, tag_len, begin(tag));
    ct.resize(ct_len);
    std::copy_n(begin(tmp), ct_len, begin(ct));
  }

  EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, tag.size(),
                      (void *)tag.data());

  auto salt = decode_b64(b64_salt);
  std::array<uint8_t, 32> key;
  PKCS5_PBKDF2_HMAC(pw.data(), pw.size(), salt.data(), salt.size(), 1000,
                    EVP_sha256(), key.size(), key.data());
  const auto iv = decode_b64(b64_iv);
  EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());
  int len;
  std::array<uint8_t, 128> plaintext{{0}};
  EVP_DecryptUpdate(ctx, plaintext.data(), &len, ct.data(), ct.size());

  if (EVP_DecryptFinal(ctx, plaintext.data() + len, &len) <= 0) {
    std::cerr << "Decoding error\n";
  } else {
    std::cout << "Message: ";
    for (uint8_t c : plaintext) {
      std::cout << c;
    }
    std::cout << "'\n";
  }

  EVP_CIPHER_CTX_free(ctx);
  return 0;
}