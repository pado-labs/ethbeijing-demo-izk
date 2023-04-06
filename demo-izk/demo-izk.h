#pragma once

#include <emp-tool/emp-tool.h>
#include "emp-zk/emp-zk.h"
using namespace emp;

#include <iostream>
using namespace std;

/**
 * Prover: party == ALICE
 * MPC Server: BOB
*/
class DemoIZK {
 private:
  static const int threads = 1;
  bool _is_prover = false;
  int party = 0;
  NetIO* io = nullptr;
  BoolIO<NetIO>* ios[threads];

 public:
  inline bool is_prover() { return _is_prover; }
  void uninit() {
    delete io;
    for (int i = 0; i < threads; i++) { delete ios[i]; }
  }
  bool init(int party, int port, const char* host) {
    this->party = party;
    _is_prover = (party == ALICE);

    io = new NetIO(!_is_prover ? nullptr : host, port);
    for (int i = 0; i < threads; i++) ios[i] = new BoolIO<NetIO>(io, _is_prover);

    return true;
  }

  // return if in(private) >= value(public)
  bool geq(size_t in, size_t value) {
    cout << "before sync base value, is_prover:" << _is_prover << " in:" << in << " value:" << value
         << endl;
    sync_base_value(value);

    cout << "after sync base value, is_prover:" << _is_prover << " in:" << in << " value:" << value
         << endl;
    setup_zk_bool<BoolIO<NetIO>>(ios, threads, party);

    Integer _in(64, in, ALICE);

    bool res = compare(_in, value);
    cout << "run_compare_izk res: " << res << endl;

    bool cheat = finalize_zk_bool<BoolIO<NetIO>>();
    if (cheat) {
      error("cheat!\n");
      return false;
    }
    return true;
  }

 private:
  void sync_base_value(size_t& value) {
    if (_is_prover)
      io->send_data((const char*)&value, sizeof(value));
    else
      io->recv_data((char*)&value, sizeof(value));
  }

  bool compare(Integer& in, size_t value) {
    size_t len = (in.size() > sizeof(size_t) * 8) ? in.size() : sizeof(size_t) * 8;
    Integer ivalue(len, value, PUBLIC);

    Bit res = in.geq(ivalue);
    return res.reveal();
  }
};
