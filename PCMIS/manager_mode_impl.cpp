#include "manager_mode_impl.h"
#include "emis.h"

void ManagerModeImpl::load(vector<Manager>& mgrArr)
{
	ifstream ifs(DATA_MGR_PATH,ios::binary);
	if(!ifs)	return;
	
	while(true)
	{
		Manager mgr;
		ifs.read(reinterpret_cast<char*>(&mgr),sizeof(mgr));
		if(ifs.fail()) break;
		mgrArr.push_back(mgr);
	}
	
	ifs.close();
}

void ManagerModeImpl::save(vector<Manager>& mgrArr)
{
	ofstream ofs(DATA_MGR_PATH,ios::binary);
	if(!ofs) return;
	
	for(unsigned int i=0; i<mgrArr.size(); i++)
	{
		ofs.write(reinterpret_cast<const char*>(&mgrArr[i]),sizeof(mgrArr[i]));	
	}
	ofs.close();
}













