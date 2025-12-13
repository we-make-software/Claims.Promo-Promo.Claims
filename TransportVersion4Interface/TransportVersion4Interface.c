#include "../.h"
Void IPV4I_RX0(struct NetworkAdapterInterfaceReceiver*NAIR,struct SKBEthernetII*,struct SKBIPv4*){
    struct SKBTransport*skbtransport=(struct SKBTransport*)skb_transport_header(NAIR->skb);
    if(skbtransport->type.use32.value==275635){ 

    }
}
Struct sk_buff*TX0(struct NetworkAdapterDevice*NAD){
    struct sk_buff*skb=InternetProtocolVersion4 TV4I.TX0(NAD);
    if(!skb)return NULL;
    struct SKBTransport*skbtransport=(struct SKBTransport*)skb_put(skb, sizeof(struct SKBTransport));
    skbtransport->type.use32.value=4456515;
    return skb;
}
LibraryBody(TransportVersion4Interface,{IPV4I_RX0})
