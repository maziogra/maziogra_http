#include "CoreSocket.h"
#include <sys/socket.h>

int main() {
  maziogra_http::CoreSocket c(PF_INET, SOCK_STREAM, 0, 8080);
}
