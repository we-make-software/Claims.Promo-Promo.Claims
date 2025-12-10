#ifndef InternetProtocolVersion6Interface_H
#define InternetProtocolVersion6Interface_H
    #include "../AddressResolutionProtocolInterface/.h"

    LibraryHeader(InternetProtocolVersion6Interface) {
        BootstrapLibraryHeader;
        void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct EthernetII*);
    };

    #define InternetProtocolVersion6\
        GetInternetProtocolVersion6Interface()->

#endif
