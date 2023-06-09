// modify from secp256k1

#if defined(_WIN32)
#include <windows.h>
#include <ntstatus.h>
#include <bcrypt.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/random.h>
#elif defined(__OpenBSD__)
#include <unistd.h>
#else
#error "Couldn't identify the OS"
#endif

#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <string>
using namespace std;

/* Returns 1 on success, and 0 on failure. */
static int fill_random(unsigned char* data, size_t size) {
#if defined(_WIN32)
  NTSTATUS res = BCryptGenRandom(NULL, data, size, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
  if (res != STATUS_SUCCESS || size > ULONG_MAX) {
    return 0;
  } else {
    return 1;
  }
#elif defined(__linux__) || defined(__FreeBSD__)
  /* If `getrandom(2)` is not available you should fallback to /dev/urandom */
  ssize_t res = getrandom(data, size, 0);
  if (res < 0 || (size_t)res != size) {
    return 0;
  } else {
    return 1;
  }
#elif defined(__APPLE__) || defined(__OpenBSD__)
  /* If `getentropy(2)` is not available you should fallback to either
     * `SecRandomCopyBytes` or /dev/urandom */
  int res = getentropy(data, size);
  if (res == 0) {
    return 1;
  } else {
    return 0;
  }
#endif
  return 0;
}

void from_hex(const char* s, int l, unsigned char* d) {
  while (l--) {
    const char* p = s + l + 2; // + 0x
    const char* p2 = p - 1;
    *(d + l / 2) = ((*p > '9' ? *p + 9 : *p) & 0x0f) | ((*p2 > '9' ? *p2 + 9 : *p2) << 4);
    l--;
  }
}

static string to_hex(unsigned char* data, size_t size) {
  string s = "0x";
  for (size_t i = 0; i < size; i++) {
    char buf[4] = {0};
    snprintf(buf, sizeof(buf), "%02x", data[i]);
    s += buf;
  }
  return s;
}
