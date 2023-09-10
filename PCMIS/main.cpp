#include "emis.h"

int main(int argc,const char* argv[])
{
	Emis& emis = Emis::get_emis();
	emis.start_emis();
}

