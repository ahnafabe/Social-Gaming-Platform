#include "GameServer.h"

int main(int args, char *argv[])
{
  if (args < 3)
  {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  unsigned short port = std::stoi(argv[1]);

  try
  {
    GameServer gameServer{port, argv[2]};
    gameServer.start();
  }
catch (const std::exception& e)
{
    std::cerr << "Exception: " << typeid(e).name() << " - " << e.what() << std::endl;
    std::exit(-1);
}

  // End of main
  return 0;
}