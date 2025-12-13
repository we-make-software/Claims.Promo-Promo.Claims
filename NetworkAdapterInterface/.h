#ifndef NetworkAdapterInterface_H
#define NetworkAdapterInterface_H
    #include "../BootstrapInterface/.h"
    #include <linux/ethtool.h>
    #include <linux/netdevice.h>
    #include <linux/skbuff.h>
    #include <linux/etherdevice.h>
    #include <linux/if_packet.h>
    #include <linux/ktime.h>
    #include <linux/slab.h>
    #include <linux/types.h>
    #include <linux/atomic.h>
    #ifdef __INTELLISENSE__
        typedef struct { s64 counter; } atomic64_t;
    #endif

    struct SKBEthernetII{
        u8 dst[6];
        u8 src[6];
        __be16 type; 
    }__attribute__((packed));

    struct NetworkAdapterInterfaceReceiver{
        struct NetworkAdapterDevice*NAD;
        struct sk_buff*skb;
        atomic64_t start;
        struct{
            struct list_head data;
        }list;
        struct{
            struct work_struct worker;
        }bt;
    };
    enum Process{Overloaded,Processed};
    struct NetworkAdapterDevice{
        enum Process Status;
        struct packet_type packet;
        struct{
            struct list_head this,GatewayDevices;
        }list;
        struct{
            atomic64_t Status;
        }time;
        struct{
            spinlock_t this,GatewayDevices;
        }lock;
    };
    #define SKBTXLibraryHeader(...)\
        struct{struct sk_buff*(*C)(__VA_ARGS__);}TXLH
   
    LibraryHeader(NetworkAdapterInterface){
        BootstrapLibraryHeader;
        struct sk_buff*(*TX0)(struct NetworkAdapterDevice*,struct SKBEthernetII**);
        struct{
            MemoryCacheHeaderFunction(NetworkAdapterInterfaceReceiver)NAIR;
            MemoryCacheHeaderFunction(NetworkAdapterDevice)NAD;
        }Memory;
        struct{
            struct list_head this;
        }Default;
    };

 
 

    #define NetworkAdapter\
            GetNetworkAdapterInterface()->


        
#endif