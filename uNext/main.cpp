#include "header.h"
#include "Config.h"
#include "Core.h"

Config GLOBAL_CONFIG = Config("config.json");

int main(int argc, char *argv[])
{
	CCore oCore;
	
	oCore.mainLoop();

	return 0;
}
