#include "../.h"
Void NAI_RX0(struct NetworkAdapterInterfaceReceiver* NAIR,struct SKBEthernetII*skbethernetii){
    struct SKBIPv4*skbipv4=(struct SKBIPv4*)skb_network_header(NAIR->skb);
    u8*saddr=(u8*)&skbipv4->saddr;
    if(skbipv4->nexthdr==17&&(saddr[0]==10||(saddr[0]==172&&saddr[1]>=16&&saddr[1]<=31)||(saddr[0]==192&&saddr[1]==168))){
        TransportVersion4 IPV4I.RX0(NAIR,skbethernetii,skbipv4);
        return;
    }
}

Struct sk_buff*TV4I_TX0(struct NetworkAdapterDevice*NAD){
    struct sk_buff*skb=Gateway IPV4I.TX0(NAD);
    if(!skb)return NULL;
    struct SKBIPv4*skbipv4=(struct SKBIPv4*)skb_put(skb, sizeof(struct SKBIPv4));
    skbipv4->version_ihl_tos=IPv4_VERSION_IHL_TOS;
    skbipv4->id_frag=0;
    skbipv4->hop_limit=64;
    skbipv4->saddr=0;
    skbipv4->daddr=~0;
    skbipv4->nexthdr=17;
    skb->transport_header=skb->tail;
    return skb;
}




LibraryBody(InternetProtocolVersion4Interface,{NAI_RX0})
