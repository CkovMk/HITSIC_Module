/*
 * sys_pitmgr_port.hpp
 *
 *  Created on: 2019年12月5日
 *      Author: CkovMk
 */

#ifndef D_KV10Z7_SYS_PITMGR_PORT_HPP_
#define D_KV10Z7_SYS_PITMGR_PORT_HPP_

#include "inc_fsl_kv10z7.h"


#define HITSIC_USE_PITMGR (1U)

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)





#define HITSIC_PITMGR_CNTFREQ	 	(2000000U)

#define HITSIC_PITMGR_INITLIZE		(0U)

#if defined(HITSIC_PITMGR_INITLIZE) && (HITSIC_PITMGR_INITLIZE > 0)
inline void PITMGR_PlatformInit(void)
{

}
#endif // ! HITSIC_PITMGR_INITLIZE

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)





#endif // ! HITSIC_USE_PITMGR

#endif /* D_KV10Z7_SYS_PITMGR_PORT_HPP_ */
