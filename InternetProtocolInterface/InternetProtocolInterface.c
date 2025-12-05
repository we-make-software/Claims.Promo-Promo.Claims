#include "../.h"
Void DefaultExit(struct GatewayDevice*gd){
    Lock(&gd->lock.Servers);
    struct InternetProtocolFrame *entry, *tmp;
    list_for_each_entry_safe(entry,tmp,&gd->list.Servers, list.this){
        switch (entry->Version)
        {
            case 4:
                Unlock(&gd->lock.Servers);
                InternetProtocolVersion4 Memory.I.Free(
                    (struct InternetProtocolVersion4Frame*)entry
                );
                Lock(&gd->lock.Servers);
                break;

            case 6:
                Unlock(&gd->lock.Servers);
                InternetProtocolVersion6 Memory.I.Free(
                    (struct InternetProtocolVersion6Frame*)entry
                );
                Lock(&gd->lock.Servers);
                break;
        }
    }
    Unlock(&gd->lock.Servers);
}
LibraryBody(InternetProtocolInterface,{DefaultExit})