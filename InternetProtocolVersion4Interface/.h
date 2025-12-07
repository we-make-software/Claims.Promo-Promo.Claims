#ifndef InternetProtocolVersion4Interface_H
#define InternetProtocolVersion4Interface_H
    #include "../InternetProtocolInterface/.h"
    
    struct InternetProtocolVersion4Frame{
        struct InternetProtocolFrame IPF;
        u32 Address;
    };
    LibraryHeader(InternetProtocolVersion4Interface){
        BootstrapLibraryHeader;
        TXLibraryHeader;
        TXCancelLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*,struct NetworkAdapterInterfaceReceiver*);
        SKBTXLibraryHeader(struct InternetProtocolFrame*,u8*);
        struct{
            MemoryCacheHeaderFunction(InternetProtocolVersion4Frame)I;
        }Memory;
        struct{
            u16 Type;
        }Default;
    };

    #define InternetProtocolVersion4\
        GetInternetProtocolVersion4Interface()->

#endif