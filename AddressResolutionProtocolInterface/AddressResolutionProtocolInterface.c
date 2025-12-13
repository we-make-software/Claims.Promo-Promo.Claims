#include "../.h"
Void NAI_RX0(struct NetworkAdapterInterfaceReceiver*,struct SKBEthernetII*){

}

Void TX0(struct NetworkAdapterDevice*NAD,struct SKBARP**skbarp){
    (*skbarp)->htype_ptype=ARP_HTYPE_PTYPE;
    (*skbarp)->hlen_plen=ARP_HLEN_PLEN;
    memcpy((*skbarp)->sha,NAD->packet.dev->dev_addr, 6);
}
Struct sk_buff*TX1(struct NetworkAdapterDevice*NAD,struct SKBARP**skbarp){
   /*
    struct SKBEthernetII*skbeII;
    struct sk_buff*skb=Gateway ARPI_IPV4I_DHCPV4I.TX0(NAD,&skbeII);
    if(!skb)return NULL;
    skbeII->type=ETH_ARP;
    *skbarp=(struct SKBARP*)skb_put(skb, sizeof(struct SKBARP));
    TX0(NAD,skbarp);
    memset((*skbarp)->tha,255,6);
    return skb;*/
    return NULL;
}
LibraryBody(AddressResolutionProtocolInterface,{NAI_RX0})
