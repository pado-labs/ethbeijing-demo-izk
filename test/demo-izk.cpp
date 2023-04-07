#include <demo-izk/demo-izk.h>

int main(int argc, char* argv[]) {
  int port, party;
  parse_party_and_port(argv, &party, &port);

  DemoIZK izk;
  izk.init(party, port, "127.0.0.1");

  size_t public_value = izk.is_prover() ? 100 : 0;
  izk.sync_data((char*)&public_value, sizeof(size_t));

  auto f_geq = [&](size_t in) {
    size_t private_value = izk.is_prover() ? in : 0;
    cout << "private:" << private_value << ", public:" << public_value
         << ", res:" << izk.geq(private_value, public_value) << endl;
  };
  f_geq(101);
  f_geq(100);
  f_geq(99);

  izk.uninit();

  return 0;
}
