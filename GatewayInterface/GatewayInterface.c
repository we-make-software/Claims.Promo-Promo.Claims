#include "../.h"
Struct sk_buff*IPV4I_TX0(struct NetworkAdapterDevice*NAD){
    struct SKBEthernetII*skbeII;
    struct sk_buff*skb=NetworkAdapter TX0(NAD,&skbeII);
    if(!skb)return NULL;
    skbeII->type=ETH_IPv4;
    memset(skbeII->dst,255,6);
    return skb;
}
Struct sk_buff*IPV6I_TX0(struct NetworkAdapterDevice*NAD){
    struct SKBEthernetII*skbeII;
    struct sk_buff*skb=NetworkAdapter TX0(NAD,&skbeII);
     if(!skb)return NULL;
    skbeII->type=ETH_IPv6;
    memcpy(skbeII->dst,DHCPV6_MULTICAST,6);
    return skb;
}

LibraryBody(GatewayInterface,{IPV4I_TX0},{IPV6I_TX0})
