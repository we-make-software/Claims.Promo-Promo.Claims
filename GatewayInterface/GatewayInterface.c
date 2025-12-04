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
    //send it to project that needs to know about it

}){ 
    InitDelayedWorkGatewayDeviceworker(this);
    ListInit(&this->list.this);
    AtomicInit(&this->status.request,&this->status.response);
    Atomic64Init(&this->status.expiry,&this->status.worker);
    LockInit(&this->lock.this);
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
        gd->Default.TXSpeed = (delta_ns < 125000000ULL);
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



Void DoEthertypeRX(u16*value,struct GatewayDevice* gd, struct NetworkAdapterInterfaceReceiver* nair){
 
             /*     RXMove(2);

            printk(KERN_INFO "DoEthertypeRX called\n");  // simple debug

            if(*value==InternetProtocolVersion6 Default.Type){
                printk(KERN_INFO "IPv6 branch\n");
                return;
            }
            if(*value==InternetProtocolVersion4 Default.Type){
                printk(KERN_INFO "IPv4 branch\n");
                return;
            }

            printk(KERN_INFO "ARP or other type\n");*/
}

Void DoRX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){

    AtomicIncrements(&gd->status.request);
    RXMove(6);
    s64 delta_ms = ktime_to_ms(ktime_sub(Atomic64Value(&nair->start), Now));
printk(KERN_INFO "DoEthertypeRX: delta_ms = %lld\n", delta_ms);

  // DoEthertypeRX((u16*)(nair->data),gd,nair);
   
    Lock(&gd->lock.this);
       gd->Default.RXSpeed = (Atomic64Value(&nair->start) - Now < 125000000ULL);


    Unlock(&gd->lock.this);
    AtomicDecrements(&gd->status.request);
    DefualtDelaySet(gd);
    Print("DORX");
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
    Lock(&nair->NAD->lock.GatewayDevices);
    struct GatewayDevice*GD;
    list_for_each_entry(GD,&nair->NAD->list.GatewayDevices, list.this) {
        if(memcmp(nair->data,GD->Address,6)==0&&cancel_delayed_work_sync(&GD->BackgroundTask.worker)){
            Unlock(&nair->NAD->lock.GatewayDevices);
            DoRX(GD,nair);
            return;
        }
    }
    GD=Gateway Memory.GatewayDevice.Create();
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