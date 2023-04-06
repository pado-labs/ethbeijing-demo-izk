#pragma once

#include <demo-izk/utils.h>
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include <iostream>
#include <stdio.h>
using namespace std;

class DemoEcdsa {
 public:
  int generate_sk(string& sk) {
    unsigned char seckey[32];
    unsigned char randomize[32];
    int return_val;
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if (!fill_random(randomize, sizeof(randomize))) {
      printf("Failed to generate randomness\n");
      return 1;
    }

    return_val = secp256k1_context_randomize(ctx, randomize);
    assert(return_val);

    while (1) {
      if (!fill_random(seckey, sizeof(seckey))) {
        printf("Failed to generate randomness\n");
        return 1;
      }
      if (secp256k1_ec_seckey_verify(ctx, seckey)) { break; }
    }

    sk = to_hex(seckey, sizeof(seckey));

    secp256k1_context_destroy(ctx);

    return 0;
  }

  int from_sk(const string& sk, string& pk, string& un_pk) {
    unsigned char seckey[32];
    unsigned char randomize[32];
    size_t len;
    int return_val;
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if (!fill_random(randomize, sizeof(randomize))) {
      printf("Failed to generate randomness\n");
      return 1;
    }
    return_val = secp256k1_context_randomize(ctx, randomize);
    assert(return_val);

    from_hex(sk.c_str(), 64, seckey);

    return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, seckey);
    assert(return_val);

    {
      unsigned char compressed_pubkey[33];
      len = sizeof(compressed_pubkey);
      return_val = secp256k1_ec_pubkey_serialize(
        ctx, compressed_pubkey, &len, &pubkey, SECP256K1_EC_COMPRESSED);
      assert(return_val);
      assert(len == sizeof(compressed_pubkey));
      pk = to_hex(compressed_pubkey, sizeof(compressed_pubkey));
    }

    {
      unsigned char un_compressed_pubkey[65];
      len = sizeof(un_compressed_pubkey);
      return_val = secp256k1_ec_pubkey_serialize(
        ctx, un_compressed_pubkey, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED);
      assert(return_val);
      assert(len == sizeof(un_compressed_pubkey));
      un_pk = to_hex(un_compressed_pubkey, sizeof(un_compressed_pubkey));
    }

    secp256k1_context_destroy(ctx);

    return 0;
  }
  int sign(const string& sk, const string& hash, string& signature) {
    unsigned char msg_hash[32];
    unsigned char seckey[32];
    unsigned char randomize[32];
    unsigned char serialized_signature[64];
    int return_val;
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if (!fill_random(randomize, sizeof(randomize))) {
      printf("Failed to generate randomness\n");
      return 1;
    }
    return_val = secp256k1_context_randomize(ctx, randomize);
    assert(return_val);

    from_hex(sk.c_str(), 64, seckey);
    from_hex(hash.c_str(), 64, msg_hash);

    int recid;
    {
      secp256k1_ecdsa_recoverable_signature recoverable_signature;
      return_val =
        secp256k1_ecdsa_sign_recoverable(ctx, &recoverable_signature, msg_hash, seckey, NULL, NULL);
      assert(return_val);

      return_val = secp256k1_ecdsa_recoverable_signature_serialize_compact(
        ctx, serialized_signature, &recid, &recoverable_signature);
      assert(return_val);
    }

    signature = to_hex(serialized_signature, sizeof(serialized_signature));
    {
      char buf[4] = {0};
      snprintf(buf, sizeof(buf), "%02x", (recid & 0xFF));
      signature += buf;
    }

    secp256k1_context_destroy(ctx);

    return 0;
  }

  int verify() { return 0; }
  int recovery() { return 0; }
};
