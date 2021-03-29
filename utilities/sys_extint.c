#include <sys_extint.h>

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)

/*!
 * @addtogroup extint
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

status_t EXTINT_Init(extint_t *_inst)
{
    assert(_inst);

    HAL_EnterCritical();
    extint_isrList_init(_inst->isrList);

    HAL_ExitCritical();

}

void EXTINT_Deinit(extint_t *_inst)
{
    assert(_inst);

    HAL_EnterCritical();
    extint_isrList_clear(_inst->isrList);
    HAL_ExitCritical();
}


void EXTINT_Isr(extint_t *_inst, uint32_t flag)
{
    assert(_inst);

    extint_isrList_it_t it;
    for (extint_isrList_it(it, _inst->isrList); !extint_isrList_end_p(it); extint_isrList_next(it))
    {
        extint_handle_t *handle = *extint_isrList_ref(it);
        if (flag & (1 << handle->index))
        {
            (*(handle->handler))(handle->userData);
        }
    }
}


status_t EXTINT_HandleInsert(extint_t *_inst, extint_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    extint_isrList_push_back(_inst->isrList, _handle);
    HAL_ExitCritical();

    return kStatus_Success;
}


status_t EXTINT_HandleRemove(extint_t *_inst, extint_handle_t *_handle)
{
    HAL_EnterCritical();
    extint_isrList_it_t it;
    for (extint_isrList_it(it, _inst->isrList); !extint_isrList_end_p(it); extint_isrList_next(it))
    {
        if (*extint_isrList_cref(it) == _handle)
        {
            extint_isrList_remove(_inst->isrList, it);
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

#endif // ! HITSIC_USE_EXTINT
