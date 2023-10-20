//// mirion_modbus.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include <iostream>
//#include <modbus.h>
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

