#include "scenes/GlobalContext.h"
#include <Jauntlet/Jauntlet.h>

int main(int argc, char** argv) {
	Jauntlet::init();
	GlobalContext::initContext();
	return 0;
}