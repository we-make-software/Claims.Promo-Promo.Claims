#ifndef InternetProtocolVersion6Interface_H
#define InternetProtocolVersion6Interface_H
    #include "../AddressResolutionProtocolInterface/.h"
    struct SKBIPv6{
        __be32 version_tc_flow;
        __be16 payload_len;
        u8 nexthdr,hop_limit;
        __be32 saddr[4],daddr[4];
    }__attribute__((packed));

    LibraryHeader(InternetProtocolVersion6Interface) {
        struct{
            void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*);
        }NAI;
    };

    #define InternetProtocolVersion6\
        GetInternetProtocolVersion6Interface()->

    static const __be16 ETH_IPv6=__constant_htons(34525);  
    static const __be32 IPv6_VERSION_TC_FLOW = __constant_htonl(100663296);

#endif
