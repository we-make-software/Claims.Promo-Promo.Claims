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

    struct NetworkAdapterInterfaceReceiver{
        struct NetworkAdapterDevice*NAD;
        struct sk_buff*skb;
        u8*data;
        atomic64_t start;
        struct{
            struct list_head tasks,pointers;
        }list;
        struct{
            struct work_struct worker;
        }BackgroundTask;
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
        SKBTXLibraryHeader(struct NetworkAdapterDevice*);
        struct{
            MemoryCacheHeaderFunction(NetworkAdapterInterfaceReceiver)NAIR;
            MemoryCacheHeaderFunction(NetworkAdapterDevice)NAD;
        }Memory;
        struct{
            struct list_head this;
        }Default;
    };

   

 
    #define RXLibraryBody\
            {RC}


    #define SKBTX(...)\
        Struct sk_buff*DTXC(__VA_ARGS__)

    #define SKBTXGet(name,...)\
        struct sk_buff*skb=name TXLH.C(__VA_ARGS__);\
        if(!skb)return NULL

    #define SKBTXReturn\
        return skb    

    #define NetworkAdapter\
            GetNetworkAdapterInterface()->

   
            
        
#endif