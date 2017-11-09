//g++ -std=c++14 -Wall -o chat chat.cpp
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <functional>
#include <type_traits>
using namespace boost;
using asio::ip::tcp;
tcp::acceptor::non_blocking_io noblockcmd(true);
void do_accept(tcp::acceptor& acceptor)
{
	auto socket = std::make_shared<tcp::socket>(acceptor.get_io_service());
	system::error_code errcode;
	socket->io_control(noblockcmd, errcode);
	acceptor.async_accept(*socket, [&acceptor, socket](std::error_code err){
				std::cout << err << " new connection\n";
				do_accept(acceptor);
			});
}
int main()
{
	std::function<int(int)> nf = nullptr;
	std::function<int(int)> f = [](int){return 1;};
	//std::cout << std::is_function<decltype(f)>::value << std::endl;
	//std::cout << std::is_object<decltype(nf)>::value << std::endl;
	asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2358));
	system::error_code errcode;
	acceptor.io_control(noblockcmd, errcode);
	if( errcode ){
		//todo error msg
	}
	do_accept(acceptor);
	io_service.run();
	return 0;
}
