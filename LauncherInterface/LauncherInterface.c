#include "../.h"
#ifdef MODULE
    static void __exit DefaultLauncherInterfaceExit(void){
        ApplicationProgramming 
            Default.Exit();
    }
    module_exit(DefaultLauncherInterfaceExit);
#else
    static int DefaultLauncherInterfaceExit(struct notifier_block*,unsigned long, void*){
        ApplicationProgramming 
            Exit();
        return NOTIFY_OK;
    }
    static struct notifier_block LN= {.notifier_call = DefaultLauncherInterfaceExit};
#endif
LauncherInterface_Setup(Launcher){
    #ifndef MODULE
        register_reboot_notifier(&LN);
    #endif
    ApplicationProgramming 
        Default.Init();
}