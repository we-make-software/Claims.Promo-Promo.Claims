#include "../.h"
WorkBackgroundTask(NetworkAdapterInterfaceReceiver,worker){
    Lock(&this->NAD->lock.this);
    if(this->NAD->Status==Overloaded||Now>Atomic64Value(&this->start)){
        Unlock(&this->NAD->lock.this);
        goto Cancel;
    }
    Unlock(&this->NAD->lock.this);
    atomic64_set(&this->start,Now+120000000ULL); 
    struct SKBEthernetII*eII=(struct SKBEthernetII *)skb_mac_header(this->skb);
    switch (eII->type) {
        case ETH_ARP:
            AddressResolutionProtocol NAI.RX0(this,eII);
            break;
        case ETH_IPv4:
            InternetProtocolVersion4 NAI.RX0(this, eII);
            break;
        case ETH_IPv6:
            InternetProtocolVersion6 NAI.RX0(this, eII);
            break;
    }
    Cancel:
    NetworkAdapter Memory.NAIR.Free(this);
}
MemoryCacheBody(NetworkAdapterInterfaceReceiver,{
    if(this->skb)
        kfree_skb(this->skb); 
}){ 
    InitWorkNetworkAdapterInterfaceReceiverworker(this);
    atomic64_set(&this->start, MillisecondsAdd(Now, 40));
    ListInit(&this->list.data);
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

static int NAIPF(struct sk_buff*skb,struct net_device*,struct packet_type*pt,struct net_device*){
    if(!skb||!skb->dev||skb->len<34||skb->pkt_type==PACKET_OUTGOING||!ApplicationProgramming Default.Status)return NET_RX_SUCCESS;
    struct SKBEthernetII*eII=(struct SKBEthernetII*)skb_mac_header(skb);
    if(!eII)return NET_RX_SUCCESS;
    if(eII->type!=ETH_IPv4&&eII->type!=ETH_IPv6&&eII->type!=ETH_ARP)return NET_RX_SUCCESS;
    #ifdef MODULE
        static const __be16 SSP=__constant_htons(22);
        if(eII->type!=ETH_ARP&&(((eII->type==ETH_IPv4)?((struct SKBIPv4*)skb_network_header(skb))->nexthdr:((struct SKBIPv6*)skb_network_header(skb))->nexthdr)==SSP))return NET_RX_SUCCESS;
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
    queue_work(system_highpri_wq,&NAIR->bt.worker);
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
    list_for_each_entry(NAD,&NetworkAdapter Default.this,list.this){
        DynamicHostConfigurationProtocolVersion4Client NAI.Init(NAD);
        DynamicHostConfigurationProtocolVersion6Client NAI.Init(NAD);  
    }
}

Struct sk_buff*TX0(struct NetworkAdapterDevice*NAD,struct SKBEthernetII**skbeII){
    if(!NAD)return NULL;
    struct sk_buff*skb=alloc_skb(1518, GFP_KERNEL);
    if(!skb)return NULL;
    skb->priority=0;
    skb->mark=0;  
    skb_reset_mac_header(skb);
    *skbeII=(struct SKBEthernetII*)skb_put(skb,12);
    memcpy((*skbeII)->src,NAD->packet.dev->dev_addr,6);
    skb->dev=NAD->packet.dev;
    skb->ip_summed=CHECKSUM_NONE;
    atomic64_set((atomic64_t*)skb->cb,Now+120000000ULL); 
    skb->network_header=skb->tail;
    return skb;
}

LibraryBody(NetworkAdapterInterface,
    BootstrapLibraryBody,
    TX0,
    {MemoryCacheBodyFunction(NetworkAdapterInterfaceReceiver),MemoryCacheBodyFunction(NetworkAdapterDevice)})