#include "../.h"

MemoryCacheBody(InternetProtocolVersion6Frame,{

}){

}
struct InternetProtocolVersion6Frame*Client(struct GatewayDevice*gd,u8*address){}
struct InternetProtocolVersion6Frame*Server(struct GatewayDevice*gd,u8*address){}

RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    Print("InternetProtocolVersion6");
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