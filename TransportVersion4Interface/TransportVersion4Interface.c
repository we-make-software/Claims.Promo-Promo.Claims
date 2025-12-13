#include "../.h"
Void IPV4I_RX0(struct NetworkAdapterInterfaceReceiver*NAIR,struct SKBEthernetII*,struct SKBIPv4*){
    struct SKBTransport*skbtransport=(struct SKBTransport*)skb_transport_header(NAIR->skb);
    if(skbtransport->type.use32.value==275635){ 

    }
}

LibraryBody(TransportVersion4Interface,{IPV4I_RX0})
