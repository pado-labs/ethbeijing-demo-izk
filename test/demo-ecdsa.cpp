
#include <string.h>

#include <demo-izk/demo-ecdsa.h>

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  string sk, pk, un_pk;
  string hash = "0x57fc0a0f2d0fd768be76019da544405c267749679fff47c3e810e7f1381fdaa1";
  string signature;

  DemoEcdsa ecdsa;
  {
    cout << "--------------------------" << endl;
    ecdsa.generate_sk(sk);
    cout << "sk:" << sk << endl;

    ecdsa.from_sk(sk, pk, un_pk);
    cout << "pk:" << pk << endl;
    cout << "un_pk:" << un_pk << endl;

    ecdsa.sign(sk, hash, signature);
    cout << "signature:" << signature << endl;
  }
  {
    cout << "--------------------------" << endl;
    sk = "0x8c924d7693ed51e462f1f895d92669d79055ea607a9f3b8658b8794bb2f849ba";
    cout << "sk:" << sk << endl;

    ecdsa.from_sk(sk, pk, un_pk);
    cout << "pk:" << pk << endl;
    cout << "un_pk:" << un_pk << endl;

    ecdsa.sign(sk, hash, signature);
    cout << "signature:" << signature << endl;
  }

  return 0;
}
