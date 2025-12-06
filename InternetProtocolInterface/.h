#ifndef InternetProtocolInterface_H
#define InternetProtocolInterface_H
    #include "../GatewayInterface/.h"
    
    #define TXLibraryBody\
        {DTXS}  

    #define TXLibraryHeader\
        struct{void(*TW)(u8*);}NALO

    #define TX\
        Void DTXS(u8*data)

    #define TXGet(name)\
        name NALO.TW(data)

    struct InternetProtocolFrame{
        u8 Version:5,Client:1,Verified:1,Block:1;
        struct{
            struct list_head this,Clients;
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
        union{
            struct InternetProtocolFrame*Server;
            struct GatewayDevice*Router;
        }link;
    };
    LibraryHeader(InternetProtocolInterface){
        RXLibraryHeader(u8*,struct InternetProtocolFrame*,struct NetworkAdapterInterfaceReceiver*);
        SKBTXLibraryHeader(struct InternetProtocolFrame*);
        struct{
            void(*Delete)(struct InternetProtocolFrame*);
            void(*Exit)(struct GatewayDevice*);
            void(*Init)(struct InternetProtocolFrame*);
        }Default;
    };

     #define InternetProtocolFrameExpiry(m)\
        {\
            Atomic64AddMinutes(&ipf->status.expiry,m);\
            if(ipf->Client)\
                Atomic64AddMinutes(&ipf->link.Server->status.expiry,m);\
            struct GatewayDevice*gd=ipf->Client?ipf->link.Server->link.Router:ipf->link.Router;\
            GatewayDeviceExpiry(m);\
        }
    #ifdef OnlyForInternetProtocolInterface
    
        #define SKBTXChoice(name)\
        {\
            SKBTXGet(name,ipf,nextHeader);\
            AtomicIncrements(&ipf->link.Server->status.response);\
            AtomicIncrements(&ipf->status.response);\
            SKBTXReturn;\
        }     

        #define TXGetChoice(name)\
            name NALO.TW((u8*)skb->network_header);break\
    
    #endif

  

    #define TXInternetProtocolCancel{\
        AtomicDecrements(&ipf->link.Server->status.response);\
        AtomicDecrements(&ipf->status.response);\
        TXGatewayCancel;\
    }

    #define InternetProtocol\
        GetInternetProtocolInterface()->

#endif