#ifndef DynamicHostConfigurationProtocolVersion4ClientInterface_H
#define DynamicHostConfigurationProtocolVersion4ClientInterface_H
    #include "../TransportVersion6Interface/.h"
    LibraryHeader(DynamicHostConfigurationProtocolVersion4ClientInterface){
        struct{
            void(*Init)(struct NetworkAdapterDevice*);
        }NAI;
    };

    #define DynamicHostConfigurationProtocolVersion4Client\
        GetDynamicHostConfigurationProtocolVersion4ClientInterface()->
#endif