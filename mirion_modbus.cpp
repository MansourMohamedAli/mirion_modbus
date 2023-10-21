//// mirion_modbus.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
#include <iostream>
#include <modbus.h>
#include <ws2tcpip.h>
#include <winsock2.h>

#define NB_CONNECTION (10)

int main()
{
	modbus_t* ctx;
	ctx = modbus_new_tcp("127.0.0.1", 502);
	if (ctx == NULL)
		std::cout << "Unable to allocate libmodbus context\n";
	else
		std::cout << "libmodbus context allocated\n";

	/*std::cout << modbus_connect(ctx);*/
	
	

	modbus_mapping_t * mb_mapping;
	mb_mapping = modbus_mapping_new(0, MODBUS_MAX_READ_BITS, 0, MODBUS_MAX_READ_REGISTERS);
	mb_mapping->tab_input_registers[1] = 500;


	//modbus_write_register(ctx, 1, 50);

	//modbus_free(ctx);

	int server_socket{ -1 };
	server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
	if (server_socket == -1)
	{
		fprintf(stderr, "Unable to listen TCP connection\n");
		modbus_free(ctx);
		return -1;
	}
	int fdmax;
	int rc{};
	fd_set refset;
	fd_set rdset;
	fdmax = server_socket;


	socklen_t addrlen;
	struct sockaddr_in clientaddr;
	int newfd;
	uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
	addrlen = sizeof(clientaddr);
	memset(&clientaddr, 0, sizeof(clientaddr));
	newfd = accept(server_socket, (struct sockaddr*) &clientaddr, &addrlen);
	if (newfd == -1)
	{
		std::cout << "Server accept() error";
	}

	modbus_set_socket(ctx, server_socket);
	rc = modbus_receive(ctx, query);

	if (rc > 0)
	{
		modbus_reply(ctx, query, rc, mb_mapping);
		std::cout << "Reply sent";
	}
	else if (rc == -1)
	{
		std::cout << "Connection is closed on socket" << server_socket << "\n";
	}


	return 0;
}























//
//int main()
//{
//    modbus_t* ctx;
//
//    ctx = modbus_new_tcp("127.0.0.1", 1502);
//    if (ctx == NULL) {
//        fprintf(stderr, "Unable to allocate libmodbus context\n");
//        return -1;
//    }
//
//    if (modbus_connect(ctx) == -1) {
//        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
//        modbus_free(ctx);
//        return -1;
//    }
//    // Write the value to the Modbus register
//    if (modbus_write_register(ctx, 1, 5) == -1) {
//        std::cerr << "Failed to write to the Modbus register" << std::endl;
//    }
//    else {
//        std::cout << "Successfully wrote " << value_to_write << " to register " << register_address << std::endl;
//    }
//
//}
//

