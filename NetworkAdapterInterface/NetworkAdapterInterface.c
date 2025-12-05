#include "../.h"
WorkBackgroundTask(NetworkAdapterInterfaceReceiver,worker){
    Lock(&this->NAD->lock.this);
    if(this->NAD->Status==Overloaded||Now>Atomic64Value(&this->start)){
        Unlock(&this->NAD->lock.this);
        goto Cancel;
    }
    Unlock(&this->NAD->lock.this);
    atomic64_set(&this->start, Now + 120000000ULL); 
    this->data+=6;
    RXCall(Gateway,this);
    Cancel:
    NetworkAdapter Memory.NAIR.Free(this);
}
MemoryCacheBody(NetworkAdapterInterfaceReceiver,{
    if(this->skb)
        kfree_skb(this->skb); 
}){ 
    InitWorkNetworkAdapterInterfaceReceiverworker(this);
    atomic64_set(&this->start, MillisecondsAdd(Now, 40));
}
MemoryCacheBody(NetworkAdapterDevice,{
    list_del(&this->list.this);
}){ 
    ListInit(&this->list.this,&this->list.GatewayDevices);  
    list_add(&this->list.this, &NetworkAdapter Default.this);
    LockInit(&this->lock.this,&this->lock.GatewayDevices);
    this->Status=Processed;
    atomic64_set(&this->time.Status, Now);
}
SKBTX(struct NetworkAdapterDevice*nad){
    if(!nad)return NULL;
    struct sk_buff*skb=alloc_skb(1518, GFP_KERNEL);
    if(!skb)return NULL;
    skb->priority=0;
    skb->mark=0;  
    skb_reset_mac_header(skb);
    memcpy(skb_mac_header(skb)+6,nad->packet.dev->dev_addr,6);
    skb->dev=nad->packet.dev;
    skb->ip_summed=CHECKSUM_NONE;
    atomic64_set((atomic64_t*)skb->cb, Now + 120000000ULL); 
    return skb;
}
static int NAIPF(struct sk_buff*skb,struct net_device*,struct packet_type*pt,struct net_device*){
    if(!skb||!skb->dev||skb->len<34||skb->pkt_type==PACKET_OUTGOING||!ApplicationProgramming Default.Status)return NET_RX_SUCCESS;
    u16*header=(u16*)skb_mac_header(skb);
    if(!header)return NET_RX_SUCCESS;
    header+=6;
    if(*header!=InternetProtocolVersion4 Default.Type&&*header!=InternetProtocolVersion6 Default.Type&&*header!=AddressResolutionProtocol Default.Type)return NET_RX_SUCCESS;
    #ifdef MODULE
        if(*header==InternetProtocolVersion4 Default.Type||*header==InternetProtocolVersion6 Default.Type){  
            header+=(*header==InternetProtocolVersion4 Default.Type)?12:22;   
            if(*header==SecureShell Default.Port)return NET_RX_SUCCESS;
        }
    #endif  
    struct NetworkAdapterDevice*NAD=container_of(pt,struct NetworkAdapterDevice, packet); 
    Lock(&NAD->lock.this);
    if(NAD->Status==Overloaded&&Now<Atomic64Value(&NAD->time.Status)){
        Unlock(&NAD->lock.this);
        kfree_skb(skb);
        return NET_RX_DROP;
    }
    Unlock(&NAD->lock.this);
    struct NetworkAdapterInterfaceReceiver*NAIR=NetworkAdapter Memory.NAIR.Create();
    if(!NAIR){
        kfree_skb(skb);
        Lock(&NAD->lock.this);
        NAD->Status=Overloaded;
        Unlock(&NAD->lock.this);
        atomic64_set(&NAD->time.Status, MillisecondsAdd(Now, 20));
        return NET_RX_DROP;
    }
    Lock(&NAD->lock.this);
    NAD->Status=Processed;
    Unlock(&NAD->lock.this);
    NAIR->NAD=NAD;
    NAIR->skb=skb_get(skb);
    NAIR->data=skb_mac_header(skb);
    queue_work(system_highpri_wq,&NAIR->BackgroundTask.worker);
    return NET_RX_DROP;
}
Static u8 Exists(struct net_device*n)
{
    struct NetworkAdapterDevice*NAD;
    list_for_each_entry(NAD,&NetworkAdapter Default.this,list.this)
        if(memcmp(NAD->packet.dev->dev_addr,n->dev_addr,6)==0)
                return 1; 
    return 0;
}
BootstrapBody({
    struct NetworkAdapterDevice*NAD,*tmp_NAD;
    list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
        Gateway Default.Exit(NAD);
        
    NAD=NULL;   
    list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
        dev_remove_pack(&NAD->packet);
    synchronize_net();
    NAD=NULL;
    list_for_each_entry_safe(NAD,tmp_NAD,&NetworkAdapter Default.this, list.this)
        NetworkAdapter Memory.NAD.Free(NAD);
    NetworkAdapter Memory.NAIR.Exit();
    NetworkAdapter Memory.NAD.Exit();
}){
    ListInit(&NetworkAdapter Default.this);
    NetworkAdapter Memory.NAIR.Init();
    NetworkAdapter Memory.NAD.Init();
    struct net_device*n;
    for_each_netdev(&init_net,n){
        if((n->flags&IFF_LOOPBACK)||Exists(n))continue;
        struct NetworkAdapterDevice*NAD=NetworkAdapter Memory.NAD.Create();
        NAD->packet=(struct packet_type){
            .dev=n,
            .type=htons(ETH_P_ALL),
            .func=NAIPF
        };
        dev_add_pack(&NAD->packet);
    }
    synchronize_net();
    struct NetworkAdapterDevice*NAD;
    list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
        Gateway Default.Init(NAD);   
}
LibraryBody(NetworkAdapterInterface,
    BootstrapLibraryBody,
    SKBTXLibraryBody,
    {MemoryCacheBodyFunction(NetworkAdapterInterfaceReceiver),MemoryCacheBodyFunction(NetworkAdapterDevice)})