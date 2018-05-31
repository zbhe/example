//g++ -std=c++14 -Wall -o chat chat.cpp
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <type_traits>
#include <unistd.h>
#include <set>
#include <thread>

using namespace boost;
using boost::asio::ip::udp;
enum class MODE{CLIENT, SERVER};

struct connection;
void login(const std::shared_ptr<connection>& con);
void logout(const std::shared_ptr<connection>& con);
void broadcast(const char* data, size_t len);

struct connection:public std::enable_shared_from_this<connection>{
	std::string name;
	asio::io_service& service;
	udp::socket s;
	udp::endpoint local_endpoint;
	udp::endpoint remote_endpoint;
	~connection()
	{
		std::cout << __LINE__ << " ~connection\n";
	}
	connection(const std::string& cname, asio::io_service& ios,
			const udp::endpoint& bindpoint, const udp::endpoint& peerpoint)
		:name(cname), service(ios), s(ios), local_endpoint(bindpoint), remote_endpoint(peerpoint)
	{
	}
	bool start()
	{
#define CHECK_ERROR(msg, ec)\
	do{\
		if( (ec) ){\
			std::cerr << __LINE__ << " " << msg << " error:" << ec.message() << std::endl;\
			return false;\
		}\
	}while(0)
		system::error_code ec;
		s.open(udp::v4(), ec);
		CHECK_ERROR("open", ec);
		s.set_option(asio::socket_base::reuse_address(true), ec);
		CHECK_ERROR("set_option", ec);
		s.bind(local_endpoint, ec);
		CHECK_ERROR("bind", ec);
		s.connect(remote_endpoint, ec);
		CHECK_ERROR("connect", ec);
		auto strptr = std::make_shared<std::string>("connected");
		s.async_send(asio::buffer(*strptr, strptr->size()),
				[strptr, self=shared_from_this()](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " init connection failed:" << ec.message() << std::endl;
				logout(self);
			}else{
				self->recvdata();
			}
		});
		return true;
	}
	void senddata(const char* buf, size_t len)
	{
		system::error_code ec;
		auto bufptr = std::make_shared<std::string>(buf, len);
		s.async_send(asio::buffer(*bufptr, bufptr->size()),
				[self=shared_from_this(), bufptr](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " senddata failed:" << ec.message() << std::endl;
				logout(self);
			}
		});
	}
	void recvdata()
	{
		auto bufptr = std::make_shared<std::array<char, 1024> >();
		s.async_receive(asio::buffer(bufptr->data(), bufptr->size()),
				[self=shared_from_this(), bufptr](const system::error_code& ec, size_t len){
			if( ec ){
				std::cerr << __LINE__ << " recvdata failed:" << ec.message() << std::endl;
				logout(self);
				return;
			}
			std::string msg{self->name};
			msg.append(":");
			msg.append(bufptr->data(), len);
			broadcast(msg.data(), msg.size());
			self->recvdata();
		});
	}
};

std::set<std::shared_ptr<connection>> allconnections;
void login(const std::shared_ptr<connection>& con)
{
	allconnections.insert(con);
}
void logout(const std::shared_ptr<connection>& con)
{
	allconnections.erase(con);
}

void broadcast(const char* data, size_t len)
{
	for(auto& con : allconnections){
		con->senddata(data, len);
	}
}

void start_service(udp::socket& s)
{
	auto nameptr = std::make_shared<std::string>(250, 0);
	auto endptr = std::make_shared<udp::endpoint>();
	s.async_receive_from(asio::buffer(const_cast<char*>(nameptr->data()), nameptr->size()), *endptr,
				[&s, nameptr, endptr](const system::error_code& ec, size_t len){
		start_service(s);
		if( ec ){
			std::cerr << __LINE__ << " error:" << ec << std::endl;
			return;
		}
		std::cout << __LINE__ << " hello:" << *nameptr << std::endl;
		system::error_code error;
		auto local_endpoint(s.local_endpoint(error));
		if( error ){
			std::cerr << __LINE__ << " error:" << error.message() << std::endl;
			return;
		}
		auto connptr = std::make_shared<connection>(nameptr->data(), s.get_io_service(), local_endpoint, *endptr);
		if( connptr->start() ){
			login(connptr);
		}
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

void client_recv(udp::socket& clientsocket)
{
	std::array<char, 1024> buf;
	system::error_code ec;
	while(1)
	{
		buf.fill(0);
		clientsocket.receive(asio::buffer(buf), 0, ec);
		if( ec ){
			std::cerr << __LINE__ << " receive error:" << ec.message() << std::endl;
			return;
		}
		std::cout << buf.data() << std::endl;
	}
}

void RunClient(const std::string& host, int port)
{
	asio::io_service io_service;
	udp::socket clientsocket(io_service, udp::v4());
	clientsocket.set_option(asio::socket_base::reuse_address(true));
	system::error_code ec;
	udp::endpoint serverpoint(asio::ip::address::from_string(host), port);
	clientsocket.connect(serverpoint, ec);
	if( ec ){
		std::cerr << __LINE__ << " conncet error:" << ec.message() << std::endl;
		return;
	}
	std::array<char, 1024> buf;
	std::cout << "input a name:\n";
	std::cin.getline(buf.data(), buf.size());
	size_t len = clientsocket.send(asio::buffer(buf.data(), strlen(buf.data())), 0, ec);
	if( ec ){
		std::cerr << __LINE__ << "  send error:" << ec.message() << std::endl;
		return;
	}
	buf.fill(0);
	len =  clientsocket.receive(asio::buffer(buf), 0, ec);
	if( ec ){
		std::cerr << __LINE__ << " receive error:" << ec.message() << std::endl;
		return;
	}
	std::cout << "receive first data from server:" << buf.data() << std::endl;
	std::thread recvthread(client_recv, std::ref(clientsocket));
	std::cout << "input:\n";
	while(1){
		buf.fill(0);
		std::cin.getline(buf.data(), buf.size());
		clientsocket.send(asio::buffer(buf.data(), strlen(buf.data())), 0, ec);
		if( ec ){
			std::cerr << __LINE__ << " send error:" << ec.message() << std::endl;
			return;
		}
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
				std::cout << argv[0] << " -[sc] -h host -p port\n";
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
