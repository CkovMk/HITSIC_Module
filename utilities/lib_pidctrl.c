#include <lib_pidctrl.h>


//#ifdef __cplusplus
//extern "C"{
//#endif

	/**
	 * @brief 设置PID结构体参数
	 * 
	 * @param _pid  
	 * @param _kp  
	 * @param _ki  
	 * @param _kd  
	 */
	void PIDCTRL_Setup(pidCtrl_t *_pid, float _kp, float _ki, float _kd)
	{
		_pid->kp = _kp;
		_pid->ki = _ki;
		_pid->kd = _kd;
		_pid->errCurr = _pid->errIntg = _pid->errIntg = _pid->errPrev = 0.0f;
	}

	/**
	 * @brief 构造并返回一个PID结构体指针
	 * 
	 * @param _kp  
	 * @param _ki  
	 * @param _kd  
	 * @return pidCtrl_t*
	 */
	pidCtrl_t *PIDCTRL_Construct(float _kp, float _ki, float _kd)
	{
		pidCtrl_t *inst = (pidCtrl_t *)malloc(sizeof(pidCtrl_t));
		assert(inst);
		if (inst == NULL)
		{
			return NULL;
		}
		PIDCTRL_Setup(inst, _kp, _ki, _kd);
		return inst;
	}

	/**
	 * @brief 更新PID偏差，已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 */
	void PIDCTRL_ErrUpdate(pidCtrl_t *_pid, float _err)
	{
		_pid->errPrev = _pid->errCurr;
		_pid->errCurr = _err;
		_pid->errIntg += _pid->errCurr;
		_pid->errDiff = _pid->errCurr - _pid->errPrev;
	}

	/**
	 * @brief 更新PID偏差，已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 */
	void PIDCTRL_ErrUpdateByDiff(pidCtrl_t *_pid, float _diff)
	{
		_pid->errPrev = _pid->errCurr;
		_pid->errCurr = _pid->errPrev + _diff;
		_pid->errIntg += _pid->errCurr;
		_pid->errDiff = _diff;
	}

		/**
	 * @brief 更新PID偏差，已知偏差项和微分项
	 * 
	 * @param _pid  
	 * @param _err
	 * @param _diff  
	 */
	void PIDCTRL_ErrUpdateByErrAndDiff(pidCtrl_t *_pid, float _err, float _diff)
	{
		_pid->errPrev = _pid->errCurr;
		_pid->errCurr = _err;
		_pid->errIntg += _pid->errCurr;
		_pid->errDiff = _diff;
	}

	/**
	 * @brief 计算比例输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcPGain(pidCtrl_t *_pid)
	{
		return _pid->errCurr * _pid->kp;
	}

	/**
	 * @brief 计算积分输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcIGain(pidCtrl_t *_pid)
	{
		return _pid->errIntg * _pid->ki;
	}

	/**
	 * @brief 计算微分输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcDGain(pidCtrl_t *_pid)
	{
		return _pid->errDiff * _pid->kd;
	}

	/**
	 * @brief 计算PID总输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcPIDGain(pidCtrl_t *_pid)
	{
		return PIDCTRL_CalcPGain(_pid) + PIDCTRL_CalcIGain(_pid) + PIDCTRL_CalcDGain(_pid);
	}

	/**
	 * @brief 更新偏差，并计算PID输出。已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 * @return float 
	 */
	float PIDCTRL_UpdateAndCalcPID(pidCtrl_t *_pid, float _err)
	{
		PIDCTRL_ErrUpdate(_pid, _err);
		return PIDCTRL_CalcPIDGain(_pid);
	}

	/**
	 * @brief 更新偏差，并计算PID输出。已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 * @return float 
	 */
	float PIDCTRL_UpdateByDiffAndCalcPID(pidCtrl_t *_pid, float _diff)
	{
		PIDCTRL_ErrUpdateByDiff(_pid, _diff);
		return PIDCTRL_CalcPIDGain(_pid);
	}

	/**
	 * @brief 设置串级PID结构体参数。
	 * 
	 * @param _pid  
	 * @param _okp  
	 * @param _oki  
	 * @param _okd  
	 * @param _ikp  
	 * @param _iki  
	 * @param _ikd  
	 */
	void PID2CTRL_Setup(pid2Ctrl_t *_pid, float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd)
	{
		PIDCTRL_Setup(&_pid->outer, _okp, _oki, _okd);
		PIDCTRL_Setup(&_pid->inner, _ikp, _iki, _ikd);
	}

	/**
	 * @brief 构造并返回一个串级PID结构体指针
	 * 
	 * @param _okp  
	 * @param _oki  
	 * @param _okd  
	 * @param _ikp  
	 * @param _iki  
	 * @param _ikd  
	 * @return pid2Ctrl_t*
	 */
	pid2Ctrl_t *PID2CTRL_Construct(float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd)
	{
		pid2Ctrl_t *inst = (pid2Ctrl_t *)malloc(sizeof(pid2Ctrl_t));
		assert(inst);
		if (inst == NULL)
		{
			return NULL;
		}
		PID2CTRL_Setup(inst, _okp, _oki, _okd, _ikp, _iki, _ikd);
		return inst;
	}
	/**
	 * @brief 更新偏差，并计算串级PID输出。已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 * @return float 
	 */
	float PID2CTRL_UpdateAndCalcPID(pid2Ctrl_t *_pid, float _err)
	{
		return PIDCTRL_UpdateAndCalcPID(&_pid->inner, PIDCTRL_UpdateAndCalcPID(&_pid->outer, _err));
	}

	/**
	 * @brief 更新偏差，并计算串级PID输出。已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 * @return float 
	 */
	float PID2CTRL_UpdateByDiffeAndCalcPID(pid2Ctrl_t *_pid, float _diff)
	{
		return PIDCTRL_UpdateAndCalcPID(&_pid->inner, PIDCTRL_UpdateByDiffAndCalcPID(&_pid->outer, _diff));
	}

//#ifdef __cplusplus
//}
//#endif







