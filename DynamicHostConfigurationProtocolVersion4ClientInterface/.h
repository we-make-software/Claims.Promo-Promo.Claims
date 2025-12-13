#ifndef DynamicHostConfigurationProtocolVersion4ClientInterface_H
#define DynamicHostConfigurationProtocolVersion4ClientInterface_H
    #include "../TransportVersion6Interface/.h"
    struct SKBDHCPv4 {
        u8 op;
        u8 htype;
        u8 hlen;
        u8 hops;
        __be32 xid;
        __be16 secs;
        __be16 flags;
        __be32 ciaddr;
        __be32 yiaddr;
        __be32 siaddr;
        __be32 giaddr;
        u8 chaddr[16];
        u8 sname[64];
        u8 file[128];
        u8 options[312];
    }__attribute__((packed));

    LibraryHeader(DynamicHostConfigurationProtocolVersion4ClientInterface){
        struct{
            void(*Init)(struct NetworkAdapterDevice*);
        }NAI;
    };

    #define DynamicHostConfigurationProtocolVersion4Client\
        GetDynamicHostConfigurationProtocolVersion4ClientInterface()->
#endif