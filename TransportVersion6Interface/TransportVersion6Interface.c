#include "../.h"
Void IPV6I_RX0(struct NetworkAdapterInterfaceReceiver*NAIR,struct SKBEthernetII*,struct SKBIPv4*){
    struct SKBTransport*skbtransport=(struct SKBTransport*)skb_transport_header(NAIR->skb);
    if(skbtransport->type.use32.value==35848738){ 
        
    }
}

LibraryBody(TransportVersion6Interface,{IPV6I_RX0})
