#include "../.h"
SKBTX(struct GatewayDevice*GD,u16*type){
    if(!GD||GD->Default.block||!HasEnoughSpaceBytes(sizeof(struct sk_buff)+1514))
        return NULL;
    SKBTXGet(NetworkAdapter,GD->NAD);
    AtomicIncrements(&GD->status.response);
    memcpy(skb_put(skb,12),GD->Address,6);
    *(u16*)skb_put(skb,2)=*type;
    skb->protocol=*type;
    skb->network_header=skb->tail;
    SKBTXReturn;
}
DelayedBackgroundTask(GatewayDevice,worker){
    Lock(&this->lock.this);
    this->Default.block=true;
    Unlock(&this->lock.this);
    Gateway Memory.GatewayDevice.Free(this);
}
MemoryCacheBody(GatewayDevice,{
    Lock(&this->NAD->lock.GatewayDevices);
        list_del(&this->list.this);
    Unlock(&this->NAD->lock.GatewayDevices);
    Lock(&this->lock.this);
    CancelDelayedWorkGatewayDeviceworker(this);
    Unlock(&this->lock.this);
    InternetProtocol Default.Exit(this);
}){ 
    InitDelayedWorkGatewayDeviceworker(this);
    ListInit(&this->list.this,&this->list.Servers);
    AtomicInit(&this->status.request,&this->status.response);
    Atomic64Init(&this->status.expiry,&this->status.worker);
    LockInit(&this->lock.this,&this->lock.Servers);
    this->Default.block=false;
    this->Default.RXSpeed=true;
    this->Default.TXSpeed=true;
}
Void DefualtDelaySet(struct GatewayDevice*gd){
    if(!ApplicationProgramming Default.Status)return;
    Lock(&gd->lock.this);
        if(Atomic64Value(&gd->status.worker) != Atomic64Value(&gd->status.expiry)){
            if(Atomic64Value(&gd->status.expiry) == 0)
                GatewayDeviceExpiry(5);
            ScheduleDelayedWorkGatewayDeviceworker(gd,(Atomic64Value(&gd->status.expiry)>Now?(Atomic64Value(&gd->status.expiry)-Now):0)/1000000ULL);
            Atomic64Set(&gd->status.worker,&gd->status.expiry);
        }
    Unlock(&gd->lock.this);
}
Void DefaultCancel(struct GatewayDevice*gd,struct sk_buff*skb){
    AtomicDecrements(&gd->status.response);
    kfree_skb(skb);
    DefualtDelaySet(gd);
}

Void DefaultSend(struct GatewayDevice* gd, struct sk_buff* skb) {
    s64 delta_ns = Now - Atomic64Value((atomic64_t*)skb->cb);
    dev_queue_xmit(skb);
    Lock(&gd->lock.this);
        gd->Default.TXSpeed=(delta_ns<125000000ULL);
    Unlock(&gd->lock.this);
    AtomicDecrements(&gd->status.response);
    DefualtDelaySet(gd);
}


Void DefaultExit(struct NetworkAdapterDevice*nda){
    if(list_empty(&nda->list.GatewayDevices))
        return;
    struct GatewayDevice *GD, *tmp;
    list_for_each_entry_safe(GD,tmp,&nda->list.GatewayDevices,list.this)
      Gateway Memory.GatewayDevice.Free(GD);
}


Void DefaultInit(struct NetworkAdapterDevice*nad){

       /*
    Lock(&nad->lock.GatewayDevices);
        struct GatewayDevice*gd=Gateway Memory.GatewayDevice.Create();
        if(!gd){
            Unlock(&nad->lock.GatewayDevices);
            return;
        }
        memcpy(gd->Address,Gateway Default.Broadcast,6);
        gd->NAD=nad;
        list_add(&gd->list.this,&nad->list.GatewayDevices);
    Unlock(&nad->lock.GatewayDevices);
    GatewayDeviceExpiry(5);
    Print("Gateway DefaultInit");
    //init to something new we know the packet wee need to make DHCP 
    */
}
Void DoEthertypeRX(u16*value,struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    RXMove(2);
    if(*value==InternetProtocolVersion6 Default.Type){
        RXCall(InternetProtocolVersion6,gd,nair);
        return;
    }
    if(*value==InternetProtocolVersion4 Default.Type){
        RXCall(InternetProtocolVersion4,gd,nair);
        return;
    }
    RXCall(AddressResolutionProtocol,gd,nair);
}
Void DoRX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    AtomicIncrements(&gd->status.request);
    RXMove(6);
    DoEthertypeRX((u16*)(nair->data),gd,nair);
    Lock(&gd->lock.this);
       gd->Default.RXSpeed=(Atomic64Value(&nair->start)-Now<125000000ULL);
    Unlock(&gd->lock.this);
    AtomicDecrements(&gd->status.request);
    DefualtDelaySet(gd);
}
static bool DefaultTXSpeed(struct GatewayDevice *gd) {
    bool value;
    Lock(&gd->lock.this);
    value=gd->Default.TXSpeed;
    Unlock(&gd->lock.this);
    return value;
}
static bool DefaultRXSpeed(struct GatewayDevice *gd) {
    bool value;
    Lock(&gd->lock.this);
    value=gd->Default.RXSpeed;
    Unlock(&gd->lock.this);
    return value;
}
RX(struct NetworkAdapterInterfaceReceiver*nair){
    if(!list_empty(&nair->NAD->list.GatewayDevices)){
         struct GatewayDevice*GDHead=list_first_entry(&nair->NAD->list.GatewayDevices,struct GatewayDevice,list.this),
                            *GDTail=list_last_entry(&nair->NAD->list.GatewayDevices,struct GatewayDevice,list.this);
        while(true){
            if(GDHead==GDTail){
                if(memcmp(nair->data,GDHead->Address,6)==0){
                    DoRX(GDHead,nair);
                    return;
                }
                break;
            }
            if(memcmp(nair->data,GDHead->Address,6)==0){
                DoRX(GDHead,nair);
                return;
            }
            if(memcmp(nair->data,GDTail->Address,6)==0){
                DoRX(GDTail,nair);
                return;
            }
            if(GDHead->list.this.next==&GDTail->list.this)break;
            GDHead=list_entry(GDHead->list.this.next,struct GatewayDevice, list.this);
            GDTail=list_entry(GDTail->list.this.prev,struct GatewayDevice, list.this);
        }
    }
    Lock(&nair->NAD->lock.GatewayDevices);
    if(!list_empty(&nair->NAD->list.GatewayDevices)){
        struct GatewayDevice*GDHead=list_first_entry(&nair->NAD->list.GatewayDevices,struct GatewayDevice,list.this),
                            *GDTail=list_last_entry(&nair->NAD->list.GatewayDevices,struct GatewayDevice,list.this);
        while(true){
            if(GDHead==GDTail){
                if(memcmp(nair->data,GDHead->Address,6)==0){
                    Unlock(&nair->NAD->lock.GatewayDevices);
                    DoRX(GDHead,nair);
                    return;
                }
                break;
            }
            if(memcmp(nair->data,GDHead->Address,6)==0){
                Unlock(&nair->NAD->lock.GatewayDevices);
                DoRX(GDHead,nair);
                return;
            }
            if(memcmp(nair->data,GDTail->Address,6)==0){
                Unlock(&nair->NAD->lock.GatewayDevices);
                DoRX(GDTail,nair);
                return;
            }
            if(GDHead->list.this.next==&GDTail->list.this)break;
            GDHead=list_entry(GDHead->list.this.next,struct GatewayDevice, list.this);
            GDTail=list_entry(GDTail->list.this.prev,struct GatewayDevice, list.this);
        }
    }
    struct GatewayDevice*GD=Gateway Memory.GatewayDevice.Create();
    if(!GD){
        Unlock(&nair->NAD->lock.GatewayDevices);
        return;
    }
    memcpy(GD->Address, nair->data,6);
    GD->NAD=nair->NAD;
    list_add(&GD->list.this,&nair->NAD->list.GatewayDevices);
    Unlock(&nair->NAD->lock.GatewayDevices);
    DoRX(GD,nair);
}
BootstrapBody({
    Gateway Memory.GatewayDevice.Exit();
}){
    Gateway Memory.GatewayDevice.Init();
}
LibraryBody(GatewayInterface,
    BootstrapLibraryBody,
    RXLibraryBody,
    SKBTXLibraryBody,
    {MemoryCacheBodyFunction(GatewayDevice)},
    {DefaultInit,DefaultExit,DefaultSend,DefaultCancel,DefaultTXSpeed,DefaultRXSpeed,{255,255,255,255,255,255}}
)