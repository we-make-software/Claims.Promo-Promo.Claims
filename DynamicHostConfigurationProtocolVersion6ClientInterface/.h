#ifndef DynamicHostConfigurationProtocolVersion6ClientInterface_H
#define DynamicHostConfigurationProtocolVersion6ClientInterface_H
    #include "../DynamicHostConfigurationProtocolVersion4ClientInterface/.h"
    LibraryHeader(DynamicHostConfigurationProtocolVersion6ClientInterface){
        struct{
            void(*Init)(struct NetworkAdapterDevice*);
        }NAI;
    };
    
    #define DynamicHostConfigurationProtocolVersion6Client\
        GetDynamicHostConfigurationProtocolVersion6ClientInterface()->
    
    static const u8 DHCPV6_MULTICAST[6]={51,51,0,1,0,2};
#endif