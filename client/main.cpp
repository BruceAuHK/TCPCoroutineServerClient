#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void run_client() {
  try {
    boost::asio::io_context io_context;

    // Set up resolver and socket
    tcp::resolver resolver(io_context);
    tcp::socket client_socket(io_context);

    // Resolve "localhost" and service "12345" into a list of endpoints
    auto endpoints = resolver.resolve("localhost", "12345");

    // Connect to the server
    boost::asio::connect(client_socket, endpoints);

    while (true) {  // Run indefinitely
      // Get a message from the user
      std::string message;
      std::cout << "Enter message: ";
      std::getline(std::cin, message);

      // Send the message to the server
      boost::asio::write(client_socket, boost::asio::buffer(message));

      // Read the server's reply
      char reply[1024];
      size_t reply_length = boost::asio::read(client_socket, boost::asio::buffer(reply, message.size()));

      // Print the server's reply
      std::cout << "Reply is: ";
      std::cout.write(reply, reply_length);
      std::cout << "\n";
    }
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

int main() {
  run_client();
  return 0;
}
