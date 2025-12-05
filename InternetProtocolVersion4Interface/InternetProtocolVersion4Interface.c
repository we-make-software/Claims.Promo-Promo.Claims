#include "../.h"
MemoryCacheBody(InternetProtocolVersion4Frame,{
    InternetProtocol Default.Delete(&this->IPF);
}){
    this->IPF.Version=4;
    InternetProtocol Default.Init(&this->IPF);
}
Static struct InternetProtocolFrame*DualClient(struct InternetProtocolFrame*ipf,u8*address){
    if(!list_empty(&ipf->list.Clients)){
        struct InternetProtocolVersion4Frame*Head=list_first_entry(&ipf->list.Clients,struct InternetProtocolVersion4Frame,IPF.list.this),
                                           *Tail=list_last_entry(&ipf->list.Clients,struct InternetProtocolVersion4Frame,IPF.list.this);
        u32*Address=(u32*)address;
        while(true){
            if(Head==Tail)
                return *Address==Head->Address?&Head->IPF:NULL;
            if(*Address==Head->Address)
                return &Head->IPF;
            if(*Address==Tail->Address)
                return &Tail->IPF;
            if(Head->IPF.list.this.next==&Tail->IPF.list.this)
                return NULL;
            Head=list_entry(Head->IPF.list.this.next,struct InternetProtocolVersion4Frame, IPF.list.this);
            Tail=list_entry(Tail->IPF.list.this.prev,struct InternetProtocolVersion4Frame, IPF.list.this);
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
        struct InternetProtocolVersion4Frame*ipv4f=InternetProtocolVersion4 Memory.I.Create();
        if(!ipv4f){
            Unlock(&ipf->lock.this);
            return NULL;
        }
        ipv4f->Address=*(u32*)address;
        ipv4f->IPF.link.Server=ipf;
        ipv4f->IPF.Client=true;
        list_add(&ipv4f->IPF.list.this,&ipf->list.Clients);
    Unlock(&ipf->lock.this);
    return &ipv4f->IPF;
}
Static struct InternetProtocolFrame*DualServer(struct GatewayDevice*gd,u8*address){
    if(!list_empty(&gd->list.Servers)){
        struct InternetProtocolVersion4Frame*Head=list_first_entry(&gd->list.Servers,struct InternetProtocolVersion4Frame,IPF.list.this),
                                           *Tail=list_last_entry(&gd->list.Servers,struct InternetProtocolVersion4Frame,IPF.list.this);
        u32*Address=(u32*)address;
        while(true){
            if(Head==Tail)
                return *Address==Head->Address?&Head->IPF:NULL;
            if(*Address==Head->Address)
                return &Head->IPF;
            if(*Address==Tail->Address)
                return &Tail->IPF;
            if(Head->IPF.list.this.next==&Tail->IPF.list.this)
                return NULL;
            Head=list_entry(Head->IPF.list.this.next,struct InternetProtocolVersion4Frame, IPF.list.this);
            Tail=list_entry(Tail->IPF.list.this.prev,struct InternetProtocolVersion4Frame, IPF.list.this);
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
        struct InternetProtocolVersion4Frame*ipv4f=InternetProtocolVersion4 Memory.I.Create();
        if(!ipv4f){
            Unlock(&gd->lock.Servers);
            return NULL;
        }
        ipv4f->Address=*(u32*)address;
        ipv4f->IPF.link.Router=gd;
        list_add(&ipv4f->IPF.list.this,&gd->list.Servers);
    Unlock(&gd->lock.Servers);
    return &ipv4f->IPF;
}
RX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    struct InternetProtocolFrame*server=Server(gd,nair->data+16);
    if(!server)
        return;
    struct InternetProtocolFrame*client=Client(server,nair->data+12);
    if(!client){
        if(!AtomicValue(&server->status.request)&&!AtomicValue(&server->status.response))
            InternetProtocolVersion4 Memory.I.Free((struct InternetProtocolVersion4Frame*)server);
        return;
    }
    u8*nextHeader=nair->data+9;
    RXMove(20);
    RXCall(InternetProtocol,nextHeader,client,nair);
}
BootstrapBody({
    InternetProtocolVersion4 Memory.I.Exit(); 
}){
    InternetProtocolVersion4 Memory.I.Init();
}
LibraryBody(InternetProtocolVersion4Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {MemoryCacheBodyFunction(InternetProtocolVersion4Frame)},
    {htons(2048)})