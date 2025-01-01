#include <ServerHTTP.h>

int main() {
  maziogra_http::ServerHTTP s(8080);
  s.start();
  return 0;
}
