#include "page.h"

extern uint32_t tos_usr;
extern uint32_t tos_user_seg;
extern uint32_t tos_kernel;
extern uint32_t tos_shared;

void set_domain_range() {
    //Set doamin access
    mmu_set_dom( 0, 0x3 ); // set domain 0 to 11_{(2)} => manager (i.e., not checked)
    mmu_set_dom( 1, 0x1 ); // set domain 1 to 01_{(2)} => client  (i.e.,     checked)
}

void enable_MMU(pte_t * pt) {
    //Set translation table.
    mmu_set_ptr0( pt );
    
    set_domain_range();
    
    //Enable MMU
    mmu_enable();
}

void kernel_page(pte_t * pt, uint32_t pid) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt[i] &= DOMAIN_MASK;
        if (i == 0x730 ) {
            pt[i] = ((pte_t) (0x740 - 1 - pid)) << 20 | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
        else if (i == 0x731) {
            pt[i] = ((pte_t ) ( 0x740))  << 20 | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
        else {
            pt[i] = ((pte_t) (i) << 20) | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
    }

}

void user_page(pte_t * pt, uint32_t pid) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt[i] &= DOMAIN_MASK;
        if ( i <= 0x600) {
            pt[i] = ((pte_t) i << 20) | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
        /*
         *  KERNEL SEGMENT
         */
        else if (i < 0x722  && i >= 0x700) {
            pt[i] = ((pte_t) (i) << 20) | 0x00000002;
            pt[i] |= DOMAIN_CLIENT;
            pt[i] &= ACCESS_MASK;
            pt[i] |= AP_PRW_URO;
        }
        /*
         * USER SEGEMENT
         */
        else if (i < 0x730  && i >= 0x722) {
            pt[i] = ((pte_t) (i) << 20) | 0x00000002;
            pt[i] |= DOMAIN_CLIENT;
            pt[i] &= ACCESS_MASK;
            pt[i] |= AP_PRW_URW;
        }
        /*
         *  SPECIFIC PROCESS STACK
         */
        else if (i == 0x730 ) {
            pt[i] = ((pte_t) (0x740 - 1 - pid)) << 20 | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
        /*
         *  SHARED MEMORY
         */
        else if (i == 0x731) {
            pt[i] = ((pte_t ) ( 0x740 ))  << 20 | 0x00000002;
            pt[i] |= DOMAIN_MANAGER;
        }
    }
}

void enable_page(pte_t * pt, pte_t * T) {
    mmu_flush();
    memcpy(T, pt, sizeof(uint32_t) * 4096);
}

void enable_kl_pg(pte_t * pt, pte_t * T, uint32_t pid) {
    kernel_page(pt, pid);
    enable_page(pt, T);
}

