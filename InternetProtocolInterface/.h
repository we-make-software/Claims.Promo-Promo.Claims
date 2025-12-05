#ifndef InternetProtocolInterface_H
#define InternetProtocolInterface_H
    #include "../GatewayInterface/.h"

    struct InternetProtocolFrame{
        u8 Version:7,Client:1;
        struct{
            struct list_head this,Clients;
        }list;
        union{
            struct InternetProtocol*Server;
            struct GatewayDevice*Router;
        }link;
    };

    LibraryHeader(InternetProtocolInterface){
        struct{
            void(*Exit)(struct GatewayDevice*gd)
        }Default;
    };

    #define InternetProtocol\
        GetInternetProtocolInterface()->
#endif