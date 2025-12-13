#include "../.h"
Void NAI_RX0(struct NetworkAdapterInterfaceReceiver*NAIR,struct SKBEthernetII*skbethernetii){
    struct SKBIPv6*skbipv6=(struct SKBIPv6*)skb_network_header(NAIR->skb);
    u8*saddr=(u8*)&skbipv6->saddr;
    if(skbipv6->nexthdr==17&&saddr[0]==254&&(saddr[1]>=128&&saddr[1]<=191)){
        TransportVersion6 IPV6I.RX0(NAIR,skbethernetii,skbipv6);
        return;
    }
}
Struct sk_buff*TRV6I_TX0(struct NetworkAdapterDevice*NAD){
    struct sk_buff*skb=Gateway IPV6I.TX0(NAD);
    if(!skb)return NULL;
    struct SKBIPv6*skbipv6=(struct SKBIPv6*)skb_put(skb, sizeof(struct SKBIPv6));
    skbipv6->version_tc_flow=IPv6_VERSION_TC_FLOW;
    skbipv6->hop_limit=64;
    skbipv6->saddr[0]=4261412864;
    skbipv6->saddr[1]=0;
    skbipv6->saddr[2]=0;
    skbipv6->saddr[3]=1;
    skbipv6->daddr[0]=4278190336;
    skbipv6->daddr[1]=0;
    skbipv6->daddr[2]=0;
    skbipv6->daddr[3]=2;
    skbipv6->nexthdr=17;
    skb->transport_header=skb->tail;
    return skb;
}
LibraryBody(InternetProtocolVersion6Interface,{NAI_RX0},{TRV6I_TX0})
