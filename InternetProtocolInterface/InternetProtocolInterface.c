#include "../.h"
Void DefaultExit(struct GatewayDevice*gd){
    Lock(&gd->lock.Servers);
    struct InternetProtocolFrame *entry, *tmp;
    list_for_each_entry_safe(entry,tmp,&gd->list.Servers, list.this){
        switch (entry->Version)
        {
            case 4:
                    Unlock(&gd->lock.Servers);
                    InternetProtocolVersion4 Memory.I.Free((struct InternetProtocolVersion4Frame*)entry);
                    Lock(&gd->lock.Servers);
                break;

            case 6:
                    Unlock(&gd->lock.Servers);
                    InternetProtocolVersion6 Memory.I.Free((struct InternetProtocolVersion6Frame*)entry);
                    Lock(&gd->lock.Servers);
                break;
        }
    }
    Unlock(&gd->lock.Servers);
}
Void DefaultDelete(struct InternetProtocolFrame*ipf){
    if(ipf->Client){
        Lock(&ipf->link.Server->lock.this);
            list_del(&ipf->list.this);
        Unlock(&ipf->link.Server->lock.this);
    }else{
        struct GatewayDevice*gd=ipf->link.Router;
        Lock(&gd->lock.Servers);
            list_del(&ipf->list.this);
        Unlock(&gd->lock.Servers);
    }
}
Void DefaultInit(struct InternetProtocolFrame*ipf){
    ipf->Block=false;
    ipf->Client=false;
    ipf->Verified=false;
    ListInit(&ipf->list.this,&ipf->list.Clients);
    LockInit(&ipf->lock.this);
    AtomicInit(&ipf->status.request,&ipf->status.response);
}
RX(u8*nextHeader,struct InternetProtocolFrame*ipf,struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    //handle in version specific interface
}
LibraryBody(InternetProtocolInterface,{DefaultDelete,DefaultExit,DefaultInit})