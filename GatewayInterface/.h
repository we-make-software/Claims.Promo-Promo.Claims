#ifndef GatewayInterface_H
#define GatewayInterface_H
    #include "../NetworkAdapterInterface/.h"

    LibraryHeader(GatewayInterface){
        struct{
            struct sk_buff*(*TX0)(struct NetworkAdapterDevice*);
        }IPV4I;
        struct{
            struct sk_buff*(*TX0)(struct NetworkAdapterDevice*);  
        }IPV6I;
    };

    #define Gateway\
        GetGatewayInterface()->


#endif
