#pragma once

#include <httplib.h>
using namespace httplib;

#include <chrono>
#include <cstdio>
#include <iostream>
#include <map>
using namespace std;

class DemoIZK;
typedef void (*func_do_task)(
  DemoIZK* izk, const multimap<string, string>& req, multimap<string, string>& res);

std::string make_string(const multimap<string, string>& m) {
  string s;
  for (auto it = m.begin(); it != m.end(); ++it) {
    s += it->first;
    s += "=";
    s += it->second;
    s += "&";
  }
  if (s.length() > 0) { s = s.substr(0, s.length() - 1); }
  return s;
}

class DemoHTTP {
  Server svr;

 public:
  void stop() {
    cout << "stop http server" << endl;
    svr.stop();
  }

  bool start(int port, DemoIZK* izk, func_do_task do_task) {
    if (!svr.is_valid()) {
      cerr << "server error" << endl;
      return false;
    }

    // e.g.: /get_izk?basevalue=100&balance=101&truehash=0x12345678abcdef&falsehash=0x01234567abcdef
    svr.Get("/get_izk", [&](const Request& req, Response& res) {
      cout << "/get_izk query:" << make_string(req.params) << endl;

      string result = "";
      if (izk != nullptr && do_task != nullptr) {
        multimap<string, string> _res;
        do_task(izk, req.params, _res);
        result = make_string(_res);
      }
      res.set_content(result, "text/plain");
    });

    cout << "HTTP Server listen on: 0.0.0.0:" << port << endl;
    svr.listen("0.0.0.0", port);

    return true;
  }
  bool start(int port) { return start(port, nullptr, nullptr); }
};
