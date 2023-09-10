#ifndef EMIS_H
#define EMIS_H

#include "manager_view_impl.h"

#define DATA_ID_PATH "./data/id.dat"
#define DATA_MGR_PATH "./data/managers.dat"


class Emis
{
	ManagerView* mgrView;
	static Emis emis;
	
	Emis(void);
	Emis(const Emis& that){}
public:
	~Emis(void);
	static Emis& get_emis(void);
	void start_emis(void);
};

#endif//EMIS_H

