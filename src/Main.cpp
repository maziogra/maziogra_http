#include <ServerHTTP.h>

int main() {
  maziogra_http::ServerHTTP s(8080);
  s.addRoute("/hello", "GET", [](const maziogra_http::HttpRequest &request) {
    return "<img src=\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQi_ZID5maG5NwNY7MlQojiunQKhZTdkkBj2-D0fl5xwFrQUNXCYjrN5qmOwORNY5SKlzf8QK6fh6r2gUn5xWQ_SQ\">";
  });
  s.start();
  return 0;
}
