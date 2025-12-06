#ifndef InternetProtocolVersion6Interface_H
#define InternetProtocolVersion6Interface_H
    #include "../AddressResolutionProtocolInterface/.h"
    
    struct InternetProtocolVersion6Frame{
        struct InternetProtocolFrame IPF;
        u64 LowAddress,HighAddress;
    };
    LibraryHeader(InternetProtocolVersion6Interface){
        BootstrapLibraryHeader;
        TXLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*,struct NetworkAdapterInterfaceReceiver*);
        SKBTXLibraryHeader(struct InternetProtocolFrame*,u8*);
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