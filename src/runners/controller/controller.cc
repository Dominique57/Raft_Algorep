#include <strings.h>

namespace Controller
{
        void Controller::GetRequest()
        {
            std::string request;
            std::cin >> request;
            parse(request);
        }

        void pasre(std::string request)
        {
            if (strcasecmp(request, "client") == 0)
            {
                sendClientRequest();
            }
        }

        void Controller::sendClientRequest()
        {
            int clientId;
            std::cin >> clientId;

            if (clientId > nb_client || clientId < nb_node)
            {
                std::cout << "Id is not a client Id\nPlease chose a client Id"\
                    << std::endl;
                return;
            }

            std::string request_message;
            std::cin >> request_message;

        }
}
