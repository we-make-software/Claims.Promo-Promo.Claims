#include "../.h"
Void IPV6I_RX0(struct NetworkAdapterInterfaceReceiver*NAIR,struct SKBEthernetII*,struct SKBIPv4*){
    struct SKBTransport*skbtransport=(struct SKBTransport*)skb_transport_header(NAIR->skb);
    if(skbtransport->type.use32.value==35848738){ 
        
    }
}
Struct sk_buff*TX0(struct NetworkAdapterDevice*NAD){
    struct sk_buff*skb=InternetProtocolVersion6 TV6I.TX0(NAD);
    if(!skb)return NULL;
    ((struct SKBTransport*)skb_put(skb,sizeof(struct SKBTransport)))->type.use32.value=0b00000010001000100000001000100011;
    return skb;
}
LibraryBody(TransportVersion6Interface,{IPV6I_RX0})
