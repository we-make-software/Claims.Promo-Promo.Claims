#ifndef InternetProtocolVersion4Interface_H
#define InternetProtocolVersion4Interface_H
    #include "../GatewayInterface/.h"
    LibraryHeader(InternetProtocolVersion4Interface){
        BootstrapLibraryHeader;
        void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct EthernetII*);
    };

    #define InternetProtocolVersion4\
        GetInternetProtocolVersion4Interface()->

#endif