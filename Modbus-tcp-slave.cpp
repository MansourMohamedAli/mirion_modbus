// Following Brighterside Hacks tutorial. Link: https://www.youtube.com/watch?v=Q0fldVY3s6c

#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <iostream>
#include <Windows.h>



const int NB_BITS = 16;
const int NB_INPUT_BITS = 16;
const int NB_REGISTERS = 16;
const int NB_INPUT_REGISTERS = 16;

const uint8_t UT_INPUT_BITS_TAB[] = { 0xFF, 0xDB };

int main()
{
	int s = -1; // socket
	int rc = 0; // request status
	int i;
	uint8_t buf[MODBUS_TCP_MAX_ADU_LENGTH] = {};
	modbus_t* ctx; // Context, contains all information needed for the modbus connection
	modbus_mapping_t* map; //contains register mapping.

	// Create Context
	ctx = modbus_new_tcp("127.0.0.1", 502);
	//ctx = modbus_new_tcp(NULL, 502);

	//create mapping
	map = modbus_mapping_new_start_address(
		0, NB_BITS, 0, NB_INPUT_BITS, 0, NB_REGISTERS, 0, NB_INPUT_REGISTERS);

	if (map == NULL)
	{
		std::cout << fprintf(stderr, "Failed to allocate mapping: %s\n", modbus_strerror(errno));
		return -1;
	}
	else
	{
		printf("mapping created\n");
		printf("Size of holding registers is: %ld \n", sizeof(map->tab_input_registers));
	}

	// At some input register values
	map->tab_input_registers[0] = 69;
	map->tab_input_registers[1] = 47;
	map->tab_input_registers[2] = 8414;

	// At some holding register values
	map->tab_registers[0] = 24;
	map->tab_registers[1] = 32;
	map->tab_registers[2] = 33000;


	// set some discrete inputs
	//modbus_set_bits_from_bytes(
	//	map->tab_input_bits, 0, NB_INPUT_REGISTERS, UT_INPUT_BITS_TAB);

	for (i = 0; i < NB_INPUT_BITS; i++)
	{
		printf("Discrete Input: %d\n", map->tab_input_bits[i]);
	}

	for (i = 0; i < NB_INPUT_BITS; i++)
	{
		printf("Analog Input: %d\n", map->tab_input_registers[i]);
	}

	while (1) {
		s = modbus_tcp_listen(ctx, 1); // second argument is number of connections
		modbus_tcp_accept(ctx, &s);

		if (s < 0)
		{
			printf("Error listening for modbus, err %s\n", modbus_strerror(errno));
			Sleep(1000);
			continue;
		}
		else
		{
			break;
		}		
	}

	struct mb_tcp
	{
		uint16_t transact;
		uint16_t protocol;
		uint16_t length;
		uint8_t unit;
		uint8_t func;
		uint8_t data[];
	};

	for (;;)
	{
		do
		{
			rc = modbus_receive(ctx, buf);
		} while (rc == 0);

		if (rc < 0)
		{
			printf("%d\n", rc);
			printf("Error %s\n", modbus_strerror(errno));
			closesocket(s);
			break;
		}

		printf("Received poll from Master\n");
		struct mb_tcp* frame = (struct mb_tcp*)buf;
		printf("Function code is: %d\n", frame -> func);
		printf("Protocol is: %d\n", frame->protocol);
		printf("Transaction is: %d\n", frame->transact);
		printf("--------------------------------------------\n");

		rc = modbus_reply(ctx, buf, rc, map);
		if (rc < 0)
		{
			printf("%d%d\n", rc, rc);
			printf("Error %s\n", modbus_strerror(errno));
			closesocket(s);
			break;
		}
	}
	return 0;

	// Free resources
	closesocket(s);
	modbus_free(ctx);
	modbus_mapping_free(map);
}

