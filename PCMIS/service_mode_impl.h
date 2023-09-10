#ifndef SERVICE_MODE_IMPL_H
#define SERVICE_MODE_IMPL_H

#include "service_mode.h"

class ServiceModeImpl : public ServiceMode
{
public:
	void load(vector<Department>& deptArr);
	void save(vector<Department>& depArr);
	void load_emp(vector<Employee>& empArr);
	void save_emp(vector<Employee>& empArr);
};

#endif
