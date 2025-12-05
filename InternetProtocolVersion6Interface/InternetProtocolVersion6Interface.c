#include "../.h"
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    Print("InternetProtocolVersion6");
}
BootstrapBody({
}){
}
LibraryBody(InternetProtocolVersion6Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(34525)})