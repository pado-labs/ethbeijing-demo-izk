#include <demo-izk/demo-http.h>

int main(int argc, char* argv[]) {
  DemoHTTP http;
  http.start(8000);

  return 0;
}
