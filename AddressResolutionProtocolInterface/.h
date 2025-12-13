#ifndef AddressResolutionProtocolInterface_H
#define AddressResolutionProtocolInterface_H
    #include "../InternetProtocolVersion4Interface/.h"

    struct SKBARP {
        __be32 htype_ptype;
        __be16 hlen_plen;
        __be16 oper;
        u8 sha[6];
        __be32 spa;
        u8 tha[6];
        __be32 tpa;
    }__attribute__((packed));


    LibraryHeader(AddressResolutionProtocolInterface) {
        struct{
            void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*);
        }NAI; 
    };

    #define AddressResolutionProtocol\
        GetAddressResolutionProtocolInterface()->

    static const __be16 ETH_ARP=__constant_htons(2054);    
    static const __be16 ARP_HLEN_PLEN =__constant_htons(1540); 
    static const __be32 ARP_HTYPE_PTYPE = __constant_htonl(67584);

#endif
