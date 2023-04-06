#include <demo-izk/demo-izk.h>

int main(int argc, char* argv[]) {
  int port, party;
  parse_party_and_port(argv, &party, &port);

  DemoIZK izk;

  izk.init(party, port, "127.0.0.1");
  {
    size_t public_value = 0;
    size_t private_value = 0;

    if (izk.is_prover()) {
      private_value = 101;
      public_value = 100;
    }
    izk.geq(private_value, public_value);
  }
  {
    size_t public_value = 0;
    size_t private_value = 0;

    if (izk.is_prover()) {
      private_value = 99;
      public_value = 100;
    }
    izk.geq(private_value, public_value);
  }
  izk.uninit();

  return 0;
}
