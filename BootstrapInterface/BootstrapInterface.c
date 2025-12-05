#define OnlyForBootstrapInterfaceInterface
#include "../.h"
BootstrapBody({
    NetworkAdapter Close;
    Gateway Close;
    InternetProtocolVersion6 Close;
    InternetProtocolVersion4 Close;
}){
    InternetProtocolVersion6 Open;
    InternetProtocolVersion4 Open;
    Gateway Open;
    NetworkAdapter Open;
}
LibraryBody(BootstrapInterface,BootstrapLibraryBody)