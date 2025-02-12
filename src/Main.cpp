#include <HttpResponse.h>
#include <ServerHTTP.h>

int main() {
  maziogra_http::ServerHTTP s(8080);
  s.addRoute("/hello", "GET", [](const maziogra_http::HttpRequest &request) {
      return maziogra_http::HttpResponse(200, "Hi " + request.getUrlParams().find("nome")->second);
  });
  s.start();
  return 0;
}
