// ChargingSystem.cpp : Defines the entry point for the console application.
//
#include "core/user_interface.h"

int main()
{
	cs_user_interface app;
	app.service_run();
	getchar();
    return 0;
}

