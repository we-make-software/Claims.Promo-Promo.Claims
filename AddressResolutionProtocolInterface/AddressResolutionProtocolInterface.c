#include "../.h"
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    Print("AddressResolutionProtocol");
}
BootstrapBody({
}){
}
LibraryBody(AddressResolutionProtocolInterface, 
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(2054)})