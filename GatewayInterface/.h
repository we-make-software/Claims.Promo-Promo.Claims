#ifndef GatewayInterface_H
#define GatewayInterface_H
#define CONFIG_64BIT 1
    #include "../NetworkAdapterInterface/.h"
    #include <linux/kref.h>
    #include <linux/atomic.h>
    #include <linux/mm.h>

    struct GatewayDevice{
        struct NetworkAdapterDevice*NAD;
        u8 Address[6];
        struct{
            struct list_head this,Servers;
        }list;
        struct{
            spinlock_t this,Servers;
        }lock;
        struct{
            AtomicHeader(response,request);    
            Atomic64Header(expiry,worker);
        }status;
        struct{           
             struct delayed_work worker; 
        }BackgroundTask;
        struct{
            u8 block:1,RXSpeed:1,TXSpeed:6;
        }Default;
    };

    #define RXLibraryHeader(...)\
        struct{void(*RW)(__VA_ARGS__);}NALH
    
  

    LibraryHeader(GatewayInterface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct NetworkAdapterInterfaceReceiver*);
        SKBTXLibraryHeader(struct GatewayDevice*, u16 *);
        struct{
            MemoryCacheHeaderFunction(GatewayDevice)GatewayDevice;
        }Memory;
        struct{
            void(*Init)(struct NetworkAdapterDevice*);
            void(*Exit)(struct NetworkAdapterDevice*);
            void(*Send)(struct GatewayDevice*,struct sk_buff*);   
            void(*Cancel)(struct GatewayDevice*,struct sk_buff*); 
            bool(*TXSpeed)(struct GatewayDevice*);
            bool(*RXSpeed)(struct GatewayDevice*);
            void(*RXError)(struct GatewayDevice*gd);
            u8 Broadcast[6];
        }Default;
    };
    
    #define GatewayOverFlowControl(...)\
        if(gd->NAD->Status==Overloaded||Now>Atomic64Value(nair->start)||gd->Default.block)__VA_ARGS__\
        else

    #define SKBTXLibraryBody\
        {DTXC}    

    #define Gateway\
        GetGatewayInterface()->


    #define RXData\
        nair->data

    #define RXMove(length)\
        RXData+=length


    #define GatewayDeviceExpiry(m)\
        Atomic64AddMinutes(&gd->status.expiry,m);

    #define TXGatewayGetSpeed\
        Gateway Default.TXSpeed(gd)

    #define RXGatewayGetSpeed\
        Gateway Default.RXSpeed(gd)    
    
    #define RXCall(name, ...)\
        name NALH.RW(__VA_ARGS__)
        
    #define RX(...)\
        Void RC(__VA_ARGS__)
    
    #define HasEnoughSpaceBytes(bytes) (ApplicationProgramming Default.Spaces() >= (u64)(bytes))


    #define RXTestTime\
    {\
        s64 __delta_ms = ktime_to_ms(ktime_sub(Atomic64Value(&nair->start), Now));\
        printk(KERN_INFO "RXSped: delta_ms = %lld\n", __delta_ms);\
    }

#endif