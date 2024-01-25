#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>

using boost::asio::ip::tcp;

// Coroutine that echoes data back to the client
boost::asio::awaitable<void> echo_client_data(tcp::socket client_socket) {
  try {
    char buffer[1024];
    while (true) {  // Run until an exception occurs
      // Read data from the client
      std::size_t data_length = co_await client_socket.async_read_some(boost::asio::buffer(buffer), boost::asio::use_awaitable);

      // Write the same data back to the client
      co_await boost::asio::async_write(client_socket, boost::asio::buffer(buffer, data_length), boost::asio::use_awaitable);
    }
  } catch (std::exception &e) {
    std::cerr << "Exception in coroutine: " << e.what() << std::endl;
  }
}

void run_server() {
  try {
    boost::asio::io_context io_context;

    // Set up acceptor to listen for new connections on port 12345
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));

    auto accept_connections = [&]() -> boost::asio::awaitable<void> {
      while (true) {  // Run indefinitely
        // Accept a new client connection
        tcp::socket client_socket = co_await acceptor.async_accept(boost::asio::use_awaitable);

        // Spawn a new coroutine to handle the client connection
        boost::asio::co_spawn(io_context.get_executor(), echo_client_data(std::move(client_socket)), boost::asio::detached);
      }
    };

    // Spawn the acceptor coroutine
    boost::asio::co_spawn(io_context.get_executor(), accept_connections, boost::asio::detached);

    // Run the I/O context
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

int main() {
  run_server();
  return 0;
}
