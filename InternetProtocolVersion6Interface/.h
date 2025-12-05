#ifndef InternetProtocolVersion6Interface_H
#define InternetProtocolVersion6Interface_H
    #include "../AddressResolutionProtocolInterface/.h"
    
    struct InternetProtocolVersion6Frame{
        struct InternetProtocolFrame IPF;
        u64 LowAddress,HighAddress;
    };
    LibraryHeader(InternetProtocolVersion6Interface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*,struct NetworkAdapterInterfaceReceiver*);
        struct{
            MemoryCacheHeaderFunction(InternetProtocolVersion6Frame)I;
        }Memory;
        struct{
            u16 Type;
        }Default;
    };

    #define InternetProtocolVersion6\
        GetInternetProtocolVersion6Interface()->

#endif