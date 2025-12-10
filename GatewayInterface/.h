#ifndef GatewayInterface_H
#define GatewayInterface_H
    #include "../NetworkAdapterInterface/.h"

    LibraryHeader(GatewayInterface){
        BootstrapLibraryHeader;
        void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct EthernetII*);
    };

    #define Gateway\
        GetGatewayInterface()->

#endif
