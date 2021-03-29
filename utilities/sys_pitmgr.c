#include <sys_pitmgr.h>

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

/*!
 * @addtogroup pitmgr
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


status_t PITMGR_Init(pitmgr_t *_inst, uint32_t _period_us)
{
    assert(_inst);
    assert(_period_us);

    HAL_EnterCritical();

    _inst->tickCounter = 0UL;
    _inst->tickPeriod_us = _period_us;

    pitmgr_isrList_init(_inst->isrList);

    HAL_ExitCritical();

    return kStatus_Success;
}

void PITMGR_Deinit(pitmgr_t *_inst)
{
    assert(_inst);

    HAL_EnterCritical();
    _inst->tickCounter = 0UL;
    _inst->tickPeriod_us = 0U;
    pitmgr_isrList_clear(_inst->isrList);
    HAL_ExitCritical();
}


void PITMGR_Isr(pitmgr_t *_inst)
{
    assert(_inst);

    pitmgr_isrList_it_t it;
    for (pitmgr_isrList_it(it, _inst->isrList); !pitmgr_isrList_end_p(it); pitmgr_isrList_next(it))
    {
        pitmgr_handle_t *handle = *pitmgr_isrList_ref(it);
        if (handle->pptFlag & pitmgr_pptEnable && _inst->tickCounter % (uint64_t)handle->tickInterval == (uint64_t)handle->tickOffset)
        {
            (*(handle->handler))(handle->userData);
            if (handle->pptFlag & pitmgr_pptRunOnce)
            {
                handle->pptFlag ^= pitmgr_pptEnable;
            }
        }
    }
    ++(_inst->tickCounter);
}


status_t PITMGR_HandleInsert(pitmgr_t *_inst, pitmgr_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    pitmgr_isrList_push_back(_inst->isrList, _handle);
    HAL_ExitCritical();

    return kStatus_Success;
}


status_t PITMGR_HandleRemove(pitmgr_t *_inst, pitmgr_handle_t *_handle)
{
    HAL_EnterCritical();
    pitmgr_isrList_it_t it;
    for (pitmgr_isrList_it(it, _inst->isrList); !pitmgr_isrList_end_p(it); pitmgr_isrList_next(it))
    {
        if (*pitmgr_isrList_cref(it) == _handle)
        {
            pitmgr_isrList_remove(_inst->isrList, it);
            HAL_ExitCritical();
            return kStatus_Success;
        }
    }
    HAL_ExitCritical();
    return kStatus_Fail;
}


#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_PITMGR
