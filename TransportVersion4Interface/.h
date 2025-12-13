#ifndef TransportVersion4Interface_H
#define TransportVersion4Interface_H
    #include "../InternetProtocolVersion6Interface/.h"
    
    struct SKBTransport{
        struct{
            struct{__be32 value; }use32;
            struct{__be16 value[2]; }use16;
        }type;
    }__attribute__((packed));
    
    LibraryHeader(TransportVersion4Interface){
        struct{
            void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*,struct SKBIPv4*)
        }IPV4I;
    };

    #define TransportVersion4\
        GetTransportVersion4Interface()->

#endif