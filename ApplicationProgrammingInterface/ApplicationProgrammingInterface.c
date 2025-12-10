#define OnlyForApplicationProgrammingInterface
#define OnlyForBootstrapInterfaceInterface
#include "../.h"
struct Content{
    u8*Name;
    void*Library;
    struct list_head List;
};
static LIST_HEAD(Contents);
static void DefaultRegister(u8*Name,void*Library){
    struct Content*this=kmalloc(sizeof(*this), GFP_KERNEL);
    *this=(struct Content){Name,Library};
    INIT_LIST_HEAD(&this->List);
    list_add(&this->List,&Contents);
}
static void RemoveAll(void){
    struct Content*this,*temporary;
    list_for_each_entry_safe(this,temporary,&Contents,List){
        list_del(&this->List);
        kfree(this);
    }
}
static void DefaultInit(void){
    if(Main.Default.Status)return;
    Main.Default.Status=true;
    Bootstrap Open;
}

static void DefaultExit(void){
    static bool ExitEvent=false;
    if(!Main.Default.Status||ExitEvent)return;
    ExitEvent=true;
    Main.Default.Status=false;
    Bootstrap Close;
    RemoveAll();
}
static void*DefaultGet(u8*name){
    struct Content*this;
    list_for_each_entry(this,&Contents,List)
        if(strcmp(this->Name,name)==0)
            return this->Library;
    return NULL;
}
static void DefaultRestart(void){
    DefaultExit();
    kernel_restart(NULL);
}
static u64 DefaultSpaces(void){
    //struct sysinfo info;
    //si_meminfo(&info);
    return 0;// ((u64)info.freeram+info.bufferram)*info.mem_unit;
}
struct ApplicationProgrammingInterface Main={{DefaultInit,DefaultExit,DefaultRestart,DefaultGet,DefaultRegister,DefaultSpaces,false}};
struct ApplicationProgrammingInterface*GetApplicationProgrammingInterface(void){
    return&Main;
}
EXPORT_SYMBOL(GetApplicationProgrammingInterface);
OverwriteLauncherInterface_Setup(ApplicationProgrammingInterface){}