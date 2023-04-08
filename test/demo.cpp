
#include <demo-izk/demo-izk.h>
#include <demo-izk/demo-ecdsa.h>
#include <demo-izk/demo-http.h>

#include <unistd.h>
#include <thread>
void f_init_zk(DemoIZK* izk) { izk->init_zk(); }

class Demo {
  int party;
  int mpc_port = 12345;
  string mpc_host = string("127.0.0.1");
  int http_port = 8000;

  DemoHTTP http;
  DemoEcdsa ecdsa;
  DemoIZK izk;

 public:
  bool parse(int argc, char* argv[]) {
    // default: [1,2] 12345 127.0.0.1 8000
    if (argc < 2) return false;
    if (argc > 1) party = atoi(argv[1]);
    if (argc > 2) mpc_port = atoi(argv[2]);
    if (argc > 3) mpc_host = string(argv[3]);
    if (argc > 4) http_port = atoi(argv[4]);
    return true;
  }

  bool start() {
    if (party == 1) {
      cout << "izk Prover will connect to " << mpc_host << ":" << mpc_port << endl;
    } else {
      cout << "izk Verifier listen on " << mpc_host << ":" << mpc_port << endl;
    }
    izk.init(party, mpc_port, mpc_host.c_str());

    if (party == ALICE) {
      http.start(
        http_port, &izk,
        [](DemoIZK* izk, const multimap<string, string>& req, multimap<string, string>& res) {
          {
            while (izk->in_processing_init_zk) { usleep(100000); }
            int sync_code = 1;
            izk->sync_data((char*)&sync_code, sizeof(sync_code));

            size_t private_value = 0;
            size_t public_value = 0;
            string truehash(66, '0'), falsehash(66, '0');
            {
              for (auto it = req.begin(); it != req.end(); ++it) {
                if (it->first == "balance")
                  private_value = atof(it->second.c_str()) * 100;
                else if (it->first == "basevalue")
                  public_value = atof(it->second.c_str()) * 100;
                else if (it->first == "truehash")
                  truehash = it->second;
                else if (it->first == "falsehash")
                  falsehash = it->second;
              }
            }
            izk->sync_data((char*)&public_value, sizeof(public_value));
            izk->sync_data((char*)truehash.data(), truehash.length());
            izk->sync_data((char*)falsehash.data(), falsehash.length());
            cout << "base_value:" << public_value / 100.0 << endl;

            izk->geq(private_value, public_value);

            int ret = 0;
            izk->sync_data((char*)&ret, sizeof(ret), BOB, ALICE);
            if (ret == -1) return;

            string signature(132, '0');
            izk->sync_data((char*)signature.data(), signature.length(), BOB, ALICE);
            res.insert({string("res"), to_string(ret)});
            res.insert({string("signature"), signature});
            cout << "izk Prover sent bytes:" << izk->sent_bytes() << endl;
            cout << "-----------------------------------\n\n";
            {
              izk->in_processing_init_zk = true;
              std::thread t(f_init_zk, izk);
              t.detach();
            }
          }
        });
    } else {
      while (true) {
        int sync_code = -1;
        izk.sync_data((char*)&sync_code, sizeof(sync_code));
        if (sync_code == 0) break;
        if (sync_code == 1) {
          size_t public_value = 0;
          string truehash(66, '0'), falsehash(66, '0');
          izk.sync_data((char*)&public_value, sizeof(public_value));
          izk.sync_data((char*)truehash.data(), truehash.length());
          izk.sync_data((char*)falsehash.data(), falsehash.length());
          cout << "base_value:" << public_value / 100.0 << endl;
          // cout << "truehash:" << truehash << endl;
          // cout << "falsehash:" << falsehash << endl;

          int ret = izk.geq(0, public_value);
          izk.sync_data((char*)&ret, sizeof(ret), BOB, ALICE);
          if (ret == -1) {
            cout << "izk failed" << endl;
            continue;
          }

          string sk = "0x8c924d7693ed51e462f1f895d92669d79055ea607a9f3b8658b8794bb2f849ba";
          string hash = (ret == 1) ? truehash : falsehash;
          string signature;
          cout << "hash:" << hash << endl;
          ecdsa.sign(sk, hash, signature);
          cout << "signature:" << signature << endl;
          izk.sync_data((char*)signature.data(), signature.length(), BOB, ALICE);
          cout << "izk Verifier sent bytes:" << izk.sent_bytes() << endl;
          cout << "-----------------------------------\n\n";
          {
            izk.in_processing_init_zk = true;
            izk.init_zk();
          }
        }
      }
    }

    return true;
  }
};

/*
http://127.0.0.1:8000/get_izk?balance=152.576&basevalue=100&falsehash=0x48ae24527e9e0c0bfe198053df982b7eb50b538d991f3880d934d75025f16497&truehash=0x414df7ad939da480ee52193ec8153b05a5d3fb1fc8b7a5253f6196a4fe595de3
*/
int main(int argc, char* argv[]) {
  Demo demo;

  if (!demo.parse(argc, argv)) {
    cerr << "Usage:\n\t" << argv[0] << " <party> <mpc_port> <mpc_host> <http_port>" << endl;
    // default: [1,2] 12345 127.0.0.1 8000
    return -1;
  }

  if (!demo.start()) {
    cerr << "Start failed" << endl;
    return -1;
  }

  return 0;
}
