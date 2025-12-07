#include "../.h"
MemoryCacheBody(InternetProtocolVersion6Frame,{
    InternetProtocol Default.Delete(&this->IPF);
}){
    this->IPF.Version=6;
    InternetProtocol Default.Init(&this->IPF);
}
TX{}
static u32 SKBNLH=htonl(6);
SKBTX(struct InternetProtocolFrame*ipf,u8*nextHeader){
    SKBTXGet(Gateway,ipf->link.Router,InternetProtocolVersion6 Default.Type);
    u8*data=skb_put(skb,40);
    *(u32*)(data)=SKBNLH;
    data[6]=*nextHeader;
    data[7]=64;
    u64*LowDestinationAddress=(u64*)(data+24);
    *(u64*)(data+8)=((struct InternetProtocolVersion6Frame*)ipf->link.Server)->LowAddress;
    *(u64*)(data+16)=((struct InternetProtocolVersion6Frame*)ipf->link.Server)->HighAddress;
    *(u64*)(data+24)=((struct InternetProtocolVersion6Frame*)ipf)->LowAddress;
    *(u64*)(data+32)=((struct InternetProtocolVersion6Frame*)ipf)->HighAddress;
    skb->transport_header=skb->tail;
    SKBTXReturn;
}
Static struct InternetProtocolFrame*DualClient(struct InternetProtocolFrame*ipf,u8*address){
    if(!list_empty(&ipf->list.Clients)){
        struct InternetProtocolVersion6Frame*Head=list_first_entry(&ipf->list.Clients,struct InternetProtocolVersion6Frame,IPF.list.this),
                                            *Tail=list_last_entry(&ipf->list.Clients,struct InternetProtocolVersion6Frame,IPF.list.this);
        u64*LowAddress=(u64*)address;
        u64*HighAddress=(u64*)(address+8);
        while(true){
            if(Head==Tail)
                return *LowAddress==Head->LowAddress&&*HighAddress==Head->HighAddress?&Head->IPF:NULL;
            if(*LowAddress==Head->LowAddress&&*HighAddress==Head->HighAddress)
                return &Head->IPF;
            if(*LowAddress==Tail->LowAddress&&*HighAddress==Tail->HighAddress)
                return &Tail->IPF;
            if(Head->IPF.list.this.next==&Tail->IPF.list.this)
                return NULL;
            Head=list_entry(Head->IPF.list.this.next,struct InternetProtocolVersion6Frame, IPF.list.this);
            Tail=list_entry(Tail->IPF.list.this.prev,struct InternetProtocolVersion6Frame, IPF.list.this);
        }   
    }
    return NULL;
}
Static struct InternetProtocolFrame*Client(struct InternetProtocolFrame*ipf,u8*address){
    struct InternetProtocolFrame*client=DualClient(ipf,address);
    if(client)
        return client;
    Lock(&ipf->lock.this);
        client=DualClient(ipf,address);
        if(client){
            Unlock(&ipf->lock.this);
            return client;
        }   
        struct InternetProtocolVersion6Frame*ipv6f=InternetProtocolVersion6 Memory.I.Create();
        if(!ipv6f){
            Unlock(&ipf->lock.this);
            return NULL;
        }
        ipv6f->LowAddress=*(u64*)(address);
        ipv6f->HighAddress=*(u64*)(address+8);
        ipv6f->IPF.link.Server=ipf;
        ipv6f->IPF.Client=true;
        list_add(&ipv6f->IPF.list.this,&ipf->list.Clients);
    Unlock(&ipf->lock.this);
    return &ipv6f->IPF;
}
Static struct InternetProtocolFrame*DualServer(struct GatewayDevice*gd,u8*address){
    if(!list_empty(&gd->list.Servers)){
        struct InternetProtocolVersion6Frame*Head=list_first_entry(&gd->list.Servers,struct InternetProtocolVersion6Frame,IPF.list.this),
                                            *Tail=list_last_entry(&gd->list.Servers,struct InternetProtocolVersion6Frame,IPF.list.this);
        u64*LowAddress=(u64*)address;
        u64*HighAddress=(u64*)(address+8);
        while(true){
            if(Head==Tail)
                return *LowAddress==Head->LowAddress&&*HighAddress==Head->HighAddress?&Head->IPF:NULL;
            if(*LowAddress==Head->LowAddress&&*HighAddress==Head->HighAddress)
                return &Head->IPF;
            if(*LowAddress==Tail->LowAddress&&*HighAddress==Tail->HighAddress)
                return &Tail->IPF;
            if(Head->IPF.list.this.next==&Tail->IPF.list.this)
                return NULL;
            Head=list_entry(Head->IPF.list.this.next,struct InternetProtocolVersion6Frame, IPF.list.this);
            Tail=list_entry(Tail->IPF.list.this.prev,struct InternetProtocolVersion6Frame, IPF.list.this);
        }   
    }
    return NULL;
}
Static struct InternetProtocolFrame*Server(struct GatewayDevice*gd,u8*address){
    struct InternetProtocolFrame*ipf=DualServer(gd,address);
    if(ipf)
        return ipf;
    Lock(&gd->lock.Servers);
        ipf=DualServer(gd,address);
        if(ipf){
            Unlock(&gd->lock.Servers);
            return ipf;
        }   
        struct InternetProtocolVersion6Frame*ipv6f=InternetProtocolVersion6 Memory.I.Create();
        if(!ipv6f){
            Unlock(&gd->lock.Servers);
            return NULL;
        }
        ipv6f->LowAddress=*(u64*)(address);
        ipv6f->HighAddress=*(u64*)(address+8);
        ipv6f->IPF.link.Router=gd;
        list_add(&ipv6f->IPF.list.this,&gd->list.Servers);  
    Unlock(&gd->lock.Servers);
    return &ipv6f->IPF;
}
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    struct InternetProtocolFrame*server=Server(gd,nair->data+24);
    if(!server){
        Gateway Default.RXError(gd);
        return;
    }
    struct InternetProtocolFrame*client=Client(server,nair->data+8);
    if(!client){
        if(!AtomicValue(&server->status.request)&&!AtomicValue(&server->status.response))
            InternetProtocolVersion6 Memory.I.Free((struct InternetProtocolVersion6Frame*)server);
        Gateway Default.RXError(gd);
        return;
    }
    u8*nextHeader=nair->data+6;
    RXMove(40);
    RXCall(InternetProtocol,nextHeader,client,nair);
    
}
BootstrapBody({
    InternetProtocolVersion6 Memory.I.Exit();  
}){
   
    InternetProtocolVersion6 Memory.I.Init();
}
LibraryBody(InternetProtocolVersion6Interface,
    BootstrapLibraryBody,
    TXLibraryBody,
    RXLibraryBody,
    SKBTXLibraryBody,
    {MemoryCacheBodyFunction(InternetProtocolVersion6Frame)},
    {htons(34525)})