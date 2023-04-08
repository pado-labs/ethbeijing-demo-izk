#pragma once

#include <emp-tool/emp-tool.h>
#include <emp-zk/emp-zk.h>
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
  bool is_setup = false;

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

    geq(0, 0);

    init_zk();
    return true;
  }

  // todo: here only one round
  size_t sent_bytes() { return io->counter; }

  bool in_processing_init_zk = false;
  void init_zk() {
    in_processing_init_zk = true;
    if (!is_setup) {
      io->counter = 0; // todo: set io->counter to one round
      setup_zk_bool<BoolIO<NetIO>>(ios, threads, party);
      is_setup = true;
    }
    in_processing_init_zk = false;
  }

  /**
   * return
   *   1 if in(private) >= value(public)
   *   0 if in < value
   *  -1 failed
   */
  int geq(size_t in, size_t value) {
    // setup_zk_bool<BoolIO<NetIO>>(ios, threads, party);
    init_zk();

    Integer _in(64, in, ALICE);
    bool res = compare(_in, value);
    // cout << "run_compare_izk res: " << res << endl;

    bool cheat = finalize_zk_bool<BoolIO<NetIO>>();
    is_setup = false;
    if (cheat) {
      error("cheat!\n");
      return -1;
    }

    return res ? 1 : 0;
  }

  void sync_data(char* data, size_t size, int from = ALICE, int to = BOB) {
    if (party == from) io->send_data((const char*)data, size);
    if (party == to) io->recv_data(data, size);
    io->flush();
  }

 private:
  bool compare(Integer& in, size_t value) {
    size_t len = (in.size() > sizeof(size_t) * 8) ? in.size() : sizeof(size_t) * 8;
    Integer ivalue(len, value, PUBLIC);

    Bit res = in.geq(ivalue);
    return res.reveal();
  }
};
