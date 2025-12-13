#ifndef TransportVersion6Interface_H
#define TransportVersion6Interface_H
    #include "../TransportVersion4Interface/.h"
    LibraryHeader(TransportVersion6Interface){
        struct{
            void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*,struct SKBIPv6*);
        }IPV6I;
    };

    #define TransportVersion6\
        GetTransportVersion6Interface()->


#endif