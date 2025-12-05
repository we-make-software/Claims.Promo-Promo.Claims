#include "../.h"
MemoryCacheBody(InternetProtocolVersion4Frame,{}){}
struct InternetProtocolVersion4Frame*Client(struct GatewayDevice*gd,u8*address){}
struct InternetProtocolVersion4Frame*Server(struct GatewayDevice*gd,u8*address){}
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    Print("InternetProtocolVersion4");
}
BootstrapBody({
    InternetProtocolVersion4 Memory.I.Init();
}){
    InternetProtocolVersion4 Memory.I.Exit();
}
LibraryBody(InternetProtocolVersion4Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {MemoryCacheBodyFunction(InternetProtocolVersion4Frame)},
    {htons(2048)})