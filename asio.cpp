//g++ -std=c++14 -Wall -o chat chat.cpp
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <type_traits>
#include <unistd.h>

using namespace boost;
using boost::asio::ip::udp;
enum class MODE{CLIENT, SERVER};

struct connection:public std::enable_shared_from_this<connection>{
	asio::io_service& service;
	std::array<char, 1024> buff;
	udp::socket s;
	~connection()
	{
		std::cout << __LINE__ << " ~connection\n";
	}
	connection(asio::io_service& ios, const udp::endpoint& bindpoint, const udp::endpoint& peerpoint):service(ios), s(ios)
	{
		s.open(udp::v4());
		s.set_option(asio::socket_base::reuse_address(true));
		s.bind(bindpoint);
		s.connect(peerpoint);
	}
	void start()
	{
		auto strptr = std::make_shared<std::string>("connected");
		s.async_send(asio::buffer(*strptr, strptr->size()),
				[strptr, self=shared_from_this()](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " init connection failed:" << ec.message() << std::endl;
			}else{
				self->recvdata();
			}
		});
	}
	void senddata(const char* buf, size_t len)
	{
		system::error_code ec;
		auto bufptr = std::make_shared<std::string>(buf, len);
		s.async_send(asio::buffer(*bufptr, bufptr->size()),
				[self=shared_from_this(), bufptr](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " senddata failed:" << ec.message() << std::endl;
			}
		});
	}
	void recvdata()
	{
		s.async_receive(asio::buffer(buff.data(), buff.size()),
				[self=shared_from_this()](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " recvdata failed:" << ec.message() << std::endl;
				return;
			}
			self->senddata(self->buff.data(), len);
			self->recvdata();
		});
	}
};

void start_service(udp::socket& s)
{
	auto dataptr = std::make_shared<std::array<char, 1024>>();
	auto endptr = std::make_shared<udp::endpoint>();
	s.async_receive_from(asio::buffer(*dataptr, dataptr->size()), *endptr,
				[&s, dataptr, endptr](const system::error_code& ec, size_t len){
		if( ec ){
			std::cerr << __LINE__ << " error:" << ec << std::endl;
			return;
		}
		std::cout << __LINE__ << " receive:" << std::string(&(*dataptr)[0], len) << std::endl;
		system::error_code error;
		auto connptr = std::make_shared<connection>(s.get_io_service(), s.local_endpoint(error), *endptr);
		connptr->start();
		start_service(s);
	});
}
void RunServer(int port)
{
	asio::io_service io_service;
	udp::socket listensocket(io_service, udp::endpoint(udp::v4(), port));
	listensocket.set_option(asio::socket_base::reuse_address(true));
	start_service(listensocket);
	io_service.run();
}
void RunClient(const std::string& host, int port)
{
	asio::io_service io_service;
	udp::socket clientsocket(io_service, udp::v4());
	clientsocket.set_option(asio::socket_base::reuse_address(true));
	system::error_code ec;
	udp::endpoint remotepoint(asio::ip::address::from_string("host"), port);
	//std::cout << "remotepoint:" << remotepoint.address().to_string() << ":" << remotepoint.port() << std::endl;
	clientsocket.connect(remotepoint, ec);
	if( ec ){
		std::cerr << __LINE__ << " conncet error:" << ec.message() << std::endl;
		return;
	}
	std::string s = "hello world";
	size_t len = clientsocket.send(asio::buffer(&s[0], s.size()), 0, ec);
	if( ec ){
		std::cerr << __LINE__ << "  send error:" << ec.message() << std::endl;
		return;
	}
	std::array<char, 1024> buf;
	len =  clientsocket.receive(asio::buffer(buf), 0, ec);
	if( ec ){
		std::cerr << __LINE__ << " receive error:" << ec.message() << std::endl;
		return;
	}
	std::cout << "receive first data from server:" << buf.data() << std::endl;
	while(1){
		std::cout << "input:";
		buf.fill(0);
		std::cin.getline(buf.data(), buf.size());
		clientsocket.send(asio::buffer(buf.data(), strlen(buf.data())), 0, ec);
		if( ec ){
			std::cerr << __LINE__ << " send error:" << ec.message() << std::endl;
			return;
		}
		buf.fill(0);
		clientsocket.receive(asio::buffer(buf), 0, ec);
		if( ec ){
			std::cerr << __LINE__ << " receive error:" << ec.message() << std::endl;
			return;
		}
		std::cout << "receive:" << buf.data() << std::endl;
	}
}

int main(int argc, char** argv)
{
	char c;
	MODE RunMode = MODE::CLIENT;
	int port = 0;
	std::string host = "127.0.0.1";
	while( (c = getopt(argc, argv, "csp:h:")) != -1 ){
		switch(c){
			case 's':
				RunMode = MODE::SERVER;
				break;
			case 'c':
				RunMode = MODE::CLIENT;
				break;
			case 'p':
				port = lexical_cast<int>(optarg);
				break;
			case 'h':
				host = lexical_cast<std::string>(optarg);
				break;
			case '?':
			default:
				std::cout << argv[0] << " -[sc] -p port\n";
				return 0;
		}
	}
	if( port <= 0 ){
		std::cerr << "error port " << port << std::endl;
		return 0;
	}
	std::cout << host << ":" << port << std::endl;
	if( RunMode == MODE::SERVER ){
		RunServer(port);
	}else{
		RunClient(host, port);
	}
	return 0;
}
