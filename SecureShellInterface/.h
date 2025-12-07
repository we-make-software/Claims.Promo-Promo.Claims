#ifndef SecureShellInterface_H
#define SecureShellInterface_H
    #include "../TransportInterface/.h"

    LibraryHeader(SecureShellInterface){
        struct{
            u16 Port;
        }Default;
    };

    #define SecureShell\
        GetSecureShellInterface()->

#endif