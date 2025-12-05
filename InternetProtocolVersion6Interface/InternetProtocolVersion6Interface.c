#include "../.h"

MemoryCacheBody(InternetProtocolVersion6Frame,{

}){

}
Static struct InternetProtocolVersion6Frame*Client(struct GatewayDevice*gd,u8*address){
    return NULL;
}
Static struct InternetProtocolVersion6Frame*Server(struct GatewayDevice*gd,u8*address){
    return NULL;
}

RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
   // Print("InternetProtocolVersion6");
}
BootstrapBody({
    InternetProtocolVersion6 Memory.I.Init();
}){
    InternetProtocolVersion6 Memory.I.Exit();
}
LibraryBody(InternetProtocolVersion6Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {MemoryCacheBodyFunction(InternetProtocolVersion6Frame)},
    {htons(34525)})