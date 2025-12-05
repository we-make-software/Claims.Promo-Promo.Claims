#include "../.h"
MemoryCacheBody(InternetProtocolVersion4Frame,{}){}
Static struct InternetProtocolVersion4Frame*Client(struct GatewayDevice*gd,u8*address){
    return NULL;
}
Static struct InternetProtocolVersion4Frame*Server(struct GatewayDevice*gd,u8*address){
    return NULL;
}
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
   RXTestTime;
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