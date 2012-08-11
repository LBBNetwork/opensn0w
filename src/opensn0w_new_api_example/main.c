/*
 * New opensn0w API example.
 */
 
#include "sn0wcore_public.h"

//
// You should declare the class global.
//

POPENSN0W_CORE_CLASS CoreClass;
LPSTR ConfigurationFile = DATADIR "/opensn0w.conf";

int main(int argc, char* argv[]) {
    SN0W_RETURN Status;
    
    printf("opensn0w New API Sample Jailbreak Tool\n");

    //
    // First thing that must be done is initializing the class.
    //
    
    CoreClass = SnInitializeCore(API_MAJOR_LATEST, API_MINOR_LATEST);
    
    //
    // Always check to make sure the class isn't null.
    //
    
    if(!CoreClass) {
        printf("There was an error during initialization of the Base API\n");
        return -1;
    }
    
    //
    // Load the configuration file. It contains all that is required for
    // sn0wcore to function.
    //
    
    Status = CoreClass->LoadConfigurationFile(CoreClass, ConfigurationFile);
    if(IS_ERROR(Status)) {
        printf("Error during configuration file load. 0x%08x\n", Status);
        return -1;
    }
    
    //
    // Set the bundle to the requested one.
    //
    
    Status = CoreClass->SetBundlePath(CoreClass, argv[1]);
    if(IS_ERROR(Status)) {
        printf("Error during bundle file load. 0x%08x\n", Status);
        return -1;
    }
    
    //
    // That's all there is to it!
    //
    
    Status = CoreClass->StartJailbreak(CoreClass);
    if(IS_ERROR(Status)) {
        printf("Error during bundle file load. 0x%08x\n", Status);
        return -1;
    }
    
    return 0;
}