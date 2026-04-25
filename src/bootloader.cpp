#include <bootloader.hpp>

static volatile struct limine_paging_mode_request liminePagingreq = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL};

__attribute__((used, section(".requests")))
static struct limine_kernel_address_request limineKrnreq = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request limineHHDMreq = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0};

static volatile struct limine_memmap_request limineMMreq = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static volatile struct limine_smp_request limineSmpReq = {
    .id = LIMINE_SMP_REQUEST, .revision = 0};

static volatile struct limine_rsdp_request limineRsdpReq = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};

typedef struct Bootloader
{
    uint64_t hhdm_offset;
    uint64_t kernelVirtBase;
    uint64_t kernelPhysBase;
    uint64_t entry_count;
    uint64_t cpu_count;
    uint64_t smpBspIndex;
    uint64_t rsdp;
    uint64_t total_memory ;
    LIMINE_PTR(struct limine_memmap_entry **) entries;
}Bootloader;

Bootloader bootloader;

void parse_bootloader(){    
    struct limine_paging_mode_response *liminePagingres = liminePagingreq.response;
    if(liminePagingres->mode != LIMINE_PAGING_MODE_X86_64_4LVL){
        //dobra moze jak bedzie debug albo panic to cos tutaj bedzie 
    }
    
    //HHDM
    struct limine_hhdm_response *limineHHDM = limineHHDMreq.response;
    bootloader.hhdm_offset = limineHHDM->offset;
    
    //kernel address
    if (limineKrnreq.response == NULL) while(1);
    struct limine_kernel_address_response *kernel_addr = limineKrnreq.response;
    uint64_t virt_base = kernel_addr->virtual_base;
    uint64_t phys_base = kernel_addr->physical_base;
    bootloader.kernelPhysBase = phys_base;
   // bootloader.kernelVirtBase = virt_base;
    //memory map
    
    struct limine_memmap_response *limine_memmap = limineMMreq.response;
    bootloader.entry_count = limine_memmap->entry_count;
    bootloader.entries = limine_memmap->entries;

    for (size_t i = 0; i < bootloader.entry_count; i++)
    {
        
        struct limine_memmap_entry *entry = limine_memmap->entries[i];
        if(entry->type != LIMINE_MEMMAP_RESERVED){
            bootloader.total_memory += 1;
        }
    }
    

    //SMP
    
    
    struct limine_smp_response * liminesmp = limineSmpReq.response;
    bootloader.cpu_count = liminesmp->cpu_count;
    bootloader.smpBspIndex = (uint64_t)(-1);
    for (int i = 0; i < bootloader.cpu_count; i++)
    {
       
        struct limine_smp_info *info = liminesmp->cpus[i];
        if(info->lapic_id == liminesmp->bsp_lapic_id){
            bootloader.smpBspIndex = i;
        }
    }

    if(bootloader.smpBspIndex == (uint64_t)(-1)){
        //tutej tez by sie debug przydal
    }
    

    //rsdp
    struct limine_rsdp_response* rsdp = limineRsdpReq.response;
    bootloader.rsdp = (size_t)rsdp->address - bootloader.hhdm_offset;
}