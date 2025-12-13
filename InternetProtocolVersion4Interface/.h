#ifndef InternetProtocolVersion4Interface_H
#define InternetProtocolVersion4Interface_H
    #include "../GatewayInterface/.h"
    struct SKBIPv4{
        __be16 version_ihl_tos,tot_len;
        __be32 id_frag;
        u8 hop_limit,nexthdr;
        __be16 check;
        __be32 saddr,daddr;
    }__attribute__((packed));

    LibraryHeader(InternetProtocolVersion4Interface){
        struct{
            void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*);
        }NAI; 
        struct{
            struct sk_buff*(*TX0)(struct NetworkAdapterDevice*)
        }TV4I;
    };

    #define InternetProtocolVersion4\
        GetInternetProtocolVersion4Interface()->

    static const __be16 ETH_IPv4=__constant_htons(2048);      
    static const __be16 IPv4_VERSION_IHL_TOS = __constant_htons(17664); 
#endif