#include "service_mode_impl.h"

void ServiceModeImpl::load(vector<Department>& deptArr)
{
	ifstream ifs(DATA_SER_PATH, ios::binary);
    if (!ifs) return;

    while (true) {
        Department dep;
        ifs.read(reinterpret_cast<char*>(&dep), sizeof(dep));
        if (ifs.fail()) break;
        deptArr.push_back(dep);
    }

    ifs.close();

}

void ServiceModeImpl::load_emp(vector<Employee>& empArr)
{
	ifstream ifs("./data/employee.dat", ios::binary);
    if (!ifs) return;

    while (true) {
        Employee emp;
        ifs.read(reinterpret_cast<char*>(&emp), sizeof(emp));
        if (ifs.fail()) break;
        empArr.push_back(emp);
    }

    ifs.close();
}
void ServiceModeImpl::save_emp(vector<Employee>& empArr)
{
	ofstream ofs("./data/employee.dat", ios::binary);
    if (!ofs) return;

    for (unsigned int i = 0; i < empArr.size(); i++) {
        ofs.write(reinterpret_cast<const char*>(&empArr[i]), sizeof(empArr[i]));
    }

    ofs.close();
}

void ServiceModeImpl::save(vector<Department>& deptArr)
{
	ofstream ofs(DATA_SER_PATH, ios::binary);
    if (!ofs) return;

    for (unsigned int i = 0; i < deptArr.size(); i++) {
        ofs.write(reinterpret_cast<const char*>(&deptArr[i]), sizeof(deptArr[i]));
    }

    ofs.close();
}





