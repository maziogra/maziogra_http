#if _WIN32
#include <WinSock2.h>
#endif

class NetInit {
public:
    NetInit() {
#ifdef _WIN32
        WSADATA wsa;
        int result = WSAStartup(MAKEWORD(2, 2), &wsa);
	
        if (result != 0) {
            std::cout << "WSAStartup failed: " << result << std::endl;
        }
#endif
    }

    ~NetInit() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
};