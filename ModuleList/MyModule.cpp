#include "MyModule.h"

MyModule::MyModule(Server *s, const char *osc) : Module(s,osc) {
    
	addMethodToServer("/Stream", "b", stream, this);
    addMethodToServer("/Data", "ii", data, this);
    tID = 0;
}

int MyModule::stream(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data) {

	MyModule *o = (MyModule *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    unsigned char *dp = (unsigned char *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b);
    
	if (o->tID != *dp) {
		o->tID = *dp;
		printf("tID:%d\n", o->tID);
	}
	
	return 0;
}

int MyModule::data(const char   *path, 
                   const char   *types, 
                   lo_arg       **argv, 
                   int          argc,
                   void         *data, 
                   void         *user_data) {

	return 0;
}

MyModule::~MyModule() {
	deleteMethodFromServer("/Data", "ii");
	deleteMethodFromServer("/Data", "ii");
}

