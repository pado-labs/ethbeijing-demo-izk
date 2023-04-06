#pragma once

#include <httplib.h>
using namespace httplib;

#include <chrono>
#include <cstdio>
#include <iostream>
using namespace std;

class DemoHTTP {
  Server svr;

 public:
  void stop() {
    cout << "stop http server" << endl;
    svr.stop();
  }

  bool start(int port) {
    if (!svr.is_valid()) {
      cerr << "server error" << endl;
      return false;
    }

    // /get_izk?basevalue=100&balance=101&truehash=12345678abcdef&falsehash=01234567abcdef
    svr.Get("/get_izk", [](const Request& req, Response& res) {
      for (auto it = req.params.begin(); it != req.params.end(); ++it) {
        cout << "req param: " << it->first << "=" << it->second << endl;
      }
      res.set_content("res=1&signature=0x123456789", "text/plain");
    });

    cout << "HTTP Server listen on: 0.0.0.0:" << port << endl;
    svr.listen("0.0.0.0", port);

    return true;
  }
};
