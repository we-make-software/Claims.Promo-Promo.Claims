#ifndef AddressResolutionProtocolInterface_H
#define AddressResolutionProtocolInterface_H
    #include "../InternetProtocolVersion4Interface/.h"

    LibraryHeader(AddressResolutionProtocolInterface) {
        BootstrapLibraryHeader;
        void(*RX0)(struct NetworkAdapterInterfaceReceiver*,struct EthernetII*);
    };

    #define AddressResolutionProtocol\
        GetAddressResolutionProtocolInterface()->

#endif
