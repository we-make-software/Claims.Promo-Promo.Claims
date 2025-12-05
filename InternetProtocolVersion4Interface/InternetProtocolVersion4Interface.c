#include "../.h"
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    Print("InternetProtocolVersion4");
}
BootstrapBody({
}){
}
LibraryBody(InternetProtocolVersion4Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(2048)})