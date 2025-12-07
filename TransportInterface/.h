#ifndef TransportInterface_H
#define TransportInterface_H
    #include "../InternetProtocolVersion6Interface/.h"
    struct Transport{
        struct InternetProtocolFrame*ipf;
        u8 Block;
        u16 Source,Destination;
        u8 NextHeader;
        struct{
            struct list_head this;
        }list;
        struct{
            spinlock_t this;
        }lock;
        struct{
            struct delayed_work worker;
        }BackgroundTask;
        struct{
            AtomicHeader(response,request);
            Atomic64Header(expiry,worker);
        }status;
    };
    LibraryHeader(TransportInterface){
        BootstrapLibraryHeader;
        RXLibraryHeader(u8*,struct InternetProtocolFrame*,struct NetworkAdapterInterfaceReceiver*);
    };

    #define Transport\
        GetTransportInterface()->

#endif