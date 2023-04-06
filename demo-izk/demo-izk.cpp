#include "demo-izk.h"

int start_http_server() { return 0; }
int stop_http_server() { return 0; }

int init_zk_network() { return 0; }
int uninit_zk_network() { return 0; }

void test_izk(int argc, char* argv[]) {
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
}

int main(int argc, char* argv[]) {
  test_izk(argc, argv);

  return 0;
}
