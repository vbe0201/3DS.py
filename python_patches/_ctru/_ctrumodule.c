#include <3ds.h>

#include "Python.h"

#define _CTRU_ASSERT_IPC_OK(result) \
    if (R_FAILED(result)) { \
        PyErr_SetString(PyExc_OSError, "IPC call failed"); \
        return NULL; \
    }

PyDoc_STRVAR(_ctru__doc__,
"Low-level Python bindings to smea's ctrulib.\n");

/* ac */

PyDoc_STRVAR(_ctru_ac_init__doc__,
"Initializes AC.");

static PyObject *
_ctru_ac_init(PyObject *self, PyObject *args)
{
    Result result;

    result = acInit();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_ac_exit__doc__,
"Exits AC.");

static PyObject *
_ctru_ac_exit(PyObject *self, PyObject *args)
{
    acExit();

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_ac_wait_internet_connection__doc__,
"Waits for the system to connect to the internet.");

static PyObject *
_ctru_ac_wait_internet_connection(PyObject *self, PyObject *args)
{
    Result result;

    result = acWaitInternetConnection();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_acu_get_wifi_status__doc__,
"Gets the connected Wifi status.");

static PyObject *
_ctru_acu_get_wifi_status(PyObject *self, PyObject *args)
{
    Result result;
    u32 out;

    result = ACU_GetWifiStatus(&out);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(out);
}

PyDoc_STRVAR(_ctru_acu_get_status__doc__,
"Gets the connected Wifi status.");

static PyObject *
_ctru_acu_get_status(PyObject *self, PyObject *args)
{
    Result result;
    u32 out;

    result = ACU_GetStatus(&out);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(out);
}

PyDoc_STRVAR(_ctru_acu_get_security_mode__doc__,
"Gets the connected Wifi security mode.");

static PyObject *
_ctru_acu_get_security_mode(PyObject *self, PyObject *args)
{
    Result result;
    acSecurityMode mode;

    result = ACU_GetSecurityMode(&mode);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("i", (int)mode);
}

PyDoc_STRVAR(_ctru_acu_get_ssid__doc__,
"Gets the connected Wifi SSID.");

static PyObject *
_ctru_acu_get_ssid(PyObject *self, PyObject *args)
{
    Result result;
    const char *ssid;

    result = ACU_GetSSID(ssid);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("s#", ssid);
}

PyDoc_STRVAR(_ctru_acu_get_ssid_length__doc__,
"Gets the connected Wifi SSID length.");

static PyObject *
_ctru_acu_get_ssid_length(PyObject *self, PyObject *args)
{
    Result result;
    u32 out;

    result = ACU_GetSSIDLength(&out);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(out);
}

PyDoc_STRVAR(_ctru_acu_get_proxy_enable__doc__,
"Determines whether proxy is enabled for the connected network.");

static PyObject *
_ctru_acu_get_proxy_enable(PyObject *self, PyObject *args)
{
    Result result;
    bool enable;

    result = ACU_GetProxyEnable(&enable);
    _CTRU_ASSERT_IPC_OK(result);

    return PyBool_FromLong(enable);
}

PyDoc_STRVAR(_ctru_acu_get_proxy_port__doc__,
"Gets the connected network's proxy port.");

static PyObject *
_ctru_acu_get_proxy_port(PyObject *self, PyObject *args)
{
    Result result;
    u32 out;

    result = ACU_GetProxyPort(&out);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(out);
}

PyDoc_STRVAR(_ctru_acu_get_proxy_user_name__doc__,
"Gets the connected network's proxy username.");

static PyObject *
_ctru_acu_get_proxy_user_name(PyObject *self, PyObject *args)
{
    Result result;
    char *username;

    result = ACU_GetProxyUserName(username);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("s", username);
}

PyDoc_STRVAR(_ctru_acu_get_proxy_password__doc__,
"Gets the connected network's proxy password.");

static PyObject *
_ctru_acu_get_proxy_password(PyObject *self, PyObject *args)
{
    Result result;
    char *password;

    result = ACU_GetProxyPassword(password);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("s", password);
}

PyDoc_STRVAR(_ctru_acu_get_last_error_code__doc__,
"Gets the last error to occur during a connection.");

static PyObject *
_ctru_acu_get_last_error_code(PyObject *self, PyObject *args)
{
    Result result;
    u32 error_code;

    result = ACU_GetLastErrorCode(&error_code);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(error_code);
}

PyDoc_STRVAR(_ctru_acu_get_last_detail_error_code__doc__,
"Gets the last detailed error to occur during a connection.");

static PyObject *
_ctru_acu_get_last_detail_error_code(PyObject *self, PyObject *args)
{
    Result result;
    u32 error_code;

    result = ACU_GetLastDetailErrorCode(&error_code);
    _CTRU_ASSERT_IPC_OK(result);

    return PyLong_FromUnsignedLong(error_code);
}

/* hid */

PyDoc_STRVAR(_ctru_hid_init__doc__,
"Initializes HID.");

static PyObject *
_ctru_hid_init(PyObject *self, PyObject *args)
{
    Result result;

    result = hidInit();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hid_exit__doc__,
"Exits HID.");

static PyObject *
_ctru_hid_exit(PyObject *self, PyObject *args)
{
    hidExit();

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hid_scan_input__doc__,
"Scans HID for input data.");

static PyObject *
_ctru_hid_scan_input(PyObject *self, PyObject *args)
{
    hidScanInput();

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hid_keys_held__doc__,
"Returns a bitmask of held buttons.");

static PyObject *
_ctru_hid_keys_held(PyObject *self, PyObject *args)
{
    u32 result;

    result = hidKeysHeld();

    return PyLong_FromUnsignedLong(result);
}

PyDoc_STRVAR(_ctru_hid_keys_down__doc__,
"Returns a bitmask of newly pressed buttons, this frame.");

static PyObject *
_ctru_hid_keys_down(PyObject *self, PyObject *args)
{
    u32 result;

    result = hidKeysDown();

    return PyLong_FromUnsignedLong(result);
}

PyDoc_STRVAR(__ctru_hid_keys_up__doc__,
"Returns a bitmask of newly released buttons, this frame.");

static PyObject *
_ctru_hid_keys_up(PyObject *self, PyObject *args)
{
    u32 result;

    result = hidKeysUp();

    return PyLong_FromUnsignedLong(result);
}

PyDoc_STRVAR(_ctru_hid_touch_read__doc__,
"Reads the current touch position.");

static PyObject *
_ctru_hid_touch_read(PyObject *self, PyObject *args)
{
    touchPosition pos;

    hidTouchRead(&pos);

    return Py_BuildValue("(II)", pos.px, pos.py);;
}

PyDoc_STRVAR(_ctru_hid_circle_read__doc__,
"Reads the current circle pad position.");

static PyObject *
_ctru_hid_circle_read(PyObject *self, PyObject *args)
{
    circlePosition pos;

    hidCircleRead(&pos);

    return Py_BuildValue("(II)", pos.dx, pos.dy);
}

PyDoc_STRVAR(_ctru_hid_accel_read__doc__,
"Reads the current accelerometer data.");

static PyObject *
_ctru_hid_accel_read(PyObject *self, PyObject *args)
{
    accelVector vector;

    hidAccelRead(&vector);

    return Py_BuildValue("(III)", vector.x, vector.y, vector.z);
}

PyDoc_STRVAR(_ctru_hid_gyro_read__doc__,
"Reads the current gyroscope data.");

static PyObject *
_ctru_hid_gyro_read(PyObject *self, PyObject *args)
{
    angularRate rate;

    hidGyroRead(&rate);

    return Py_BuildValue("(III)", rate.x, rate.y, rate.z);
}

PyDoc_STRVAR(_ctru_hid_wait_for_event__doc__,
"Waits for an HID event.");

static PyObject *
_ctru_hid_wait_for_event(PyObject *self, PyObject *args)
{
    int tmp_id;
    HID_Event id;
    bool next_event;

    if (!PyArg_ParseTuple(args, "ip", &tmp_id, &next_event))
        return NULL;
    id = (HID_Event)tmp_id;

    hidWaitForEvent(id, next_event);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hiduser_get_handles__doc__,
"Gets the handles for HID operation.");

static PyObject *
_ctru_hiduser_get_handles(PyObject *self, PyObject *args)
{
    Result result;
    Handle out_mem_handle, eventpad0, eventpad1, eventaccel, eventgyro, eventdebugpad;

    result = HIDUSER_GetHandles(&out_mem_handle, &eventpad0, &eventpad1,
                                &eventaccel, &eventgyro, &eventdebugpad);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("(IIIIII)", out_mem_handle, eventpad0, eventpad1,
                         eventaccel, eventgyro, eventdebugpad);
}

PyDoc_STRVAR(_ctru_hiduser_enable_accelerometer__doc__,
"Enables the accelerometer.");

static PyObject *
_ctru_hiduser_enable_accelerometer(PyObject *self, PyObject *args)
{
    Result result;

    result = HIDUSER_EnableAccelerometer();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hiduser_disable_accelerometer__doc__,
"Disables the accelerometer.");

static PyObject *
_ctru_hiduser_disable_accelerometer(PyObject *self, PyObject *args)
{
    Result result;

    result = HIDUSER_DisableAccelerometer();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hiduser_enable_gyroscope__doc__,
"Enables the gyroscope.");

static PyObject *
_ctru_hiduser_enable_gyroscope(PyObject *self, PyObject *args)
{
    Result result;

    result = HIDUSER_EnableGyroscope();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hiduser_disable_gyroscope__doc__,
"Disables the gyroscope.");

static PyObject *
_ctru_hiduser_disable_gyroscope(PyObject *self, PyObject *args)
{
    Result result;

    result = HIDUSER_DisableGyroscope();
    _CTRU_ASSERT_IPC_OK(result);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(_ctru_hiduser_get_gyroscope_raw_to_dps_coefficient__doc__,
"Gets the gyroscope raw to dps coefficient.");

static PyObject *
_ctru_hiduser_get_gyroscope_raw_to_dps_coefficient(PyObject *self, PyObject *args)
{
    Result result;
    float coeff;

    result = HIDUSER_GetGyroscopeRawToDpsCoefficient(&coeff);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("f", coeff);
}

PyDoc_STRVAR(_ctru_hiduser_get_sound_volume__doc__,
"Gets the current volume slider value.");

static PyObject *
_ctru_hiduser_get_sound_volume(PyObject *self, PyObject *args)
{
    Result result;
    u8 volume;

    result = HIDUSER_GetSoundVolume(&volume);
    _CTRU_ASSERT_IPC_OK(result);

    return Py_BuildValue("B", volume);
}

static PyMethodDef _ctru_methods[] = {
    /* ac */
    {"ac_init", _ctru_ac_init, METH_NOARGS, _ctru_ac_init__doc__},
    {"ac_exit", _ctru_ac_exit, METH_NOARGS, _ctru_ac_exit__doc__},
    {"ac_wait_internet_connection", _ctru_ac_wait_internet_connection, METH_NOARGS, _ctru_ac_wait_internet_connection__doc__},
    {"acu_get_wifi_status", _ctru_acu_get_wifi_status, METH_NOARGS, _ctru_acu_get_wifi_status__doc__},
    {"acu_get_status", _ctru_acu_get_status, METH_NOARGS, _ctru_acu_get_status__doc__},
    {"acu_get_security_mode", _ctru_acu_get_security_mode, METH_NOARGS, _ctru_acu_get_security_mode__doc__},
    {"acu_get_ssid", _ctru_acu_get_ssid, METH_NOARGS, _ctru_acu_get_ssid__doc__},
    {"acu_get_ssid_length", _ctru_acu_get_ssid_length, METH_NOARGS, _ctru_acu_get_ssid_length__doc__},
    {"acu_get_proxy_enable", _ctru_acu_get_proxy_enable, METH_NOARGS, _ctru_acu_get_proxy_enable__doc__},
    {"acu_get_proxy_port", _ctru_acu_get_proxy_port, METH_NOARGS, _ctru_acu_get_proxy_port__doc__},
    {"acu_get_proxy_user_name", _ctru_acu_get_proxy_user_name, METH_NOARGS, _ctru_acu_get_proxy_user_name__doc__},
    {"acu_get_proxy_password", _ctru_acu_get_proxy_password, METH_NOARGS, _ctru_acu_get_proxy_password__doc__},
    {"acu_get_last_error_code", _ctru_acu_get_last_error_code, METH_NOARGS, _ctru_acu_get_proxy_password__doc__},
    {"acu_get_last_detail_error_code", _ctru_acu_get_last_detail_error_code, METH_NOARGS, _ctru_acu_get_last_detail_error_code__doc__},
    /* hid */
    {"hid_init", _ctru_hid_init, METH_NOARGS, _ctru_hid_init__doc__},
    {"hid_exit", _ctru_hid_exit, METH_NOARGS, _ctru_hid_exit__doc__},
    {"hid_scan_input", _ctru_hid_scan_input, METH_NOARGS, _ctru_hid_scan_input__doc__},
    {"hid_keys_held", _ctru_hid_keys_held, METH_NOARGS, _ctru_hid_keys_held__doc__},
    {"hid_keys_down", _ctru_hid_keys_down, METH_NOARGS, _ctru_hid_keys_down__doc__},
    {"hid_keys_up", _ctru_hid_keys_up, METH_NOARGS, __ctru_hid_keys_up__doc__},
    {"hid_touch_read", _ctru_hid_touch_read, METH_NOARGS, _ctru_hid_touch_read__doc__},
    {"hid_circle_read", _ctru_hid_circle_read, METH_NOARGS, _ctru_hid_circle_read__doc__},
    {"hid_accel_read", _ctru_hid_accel_read, METH_NOARGS, _ctru_hid_accel_read__doc__},
    {"hid_gyro_read", _ctru_hid_gyro_read, METH_NOARGS, _ctru_hid_gyro_read__doc__},
    {"hid_wait_for_event", _ctru_hid_wait_for_event, METH_VARARGS, _ctru_hid_wait_for_event__doc__},
    {"hiduser_get_handles", _ctru_hiduser_get_handles, METH_NOARGS, _ctru_hiduser_get_handles__doc__},
    {"hiduser_enable_accelerometer", _ctru_hiduser_enable_accelerometer, METH_NOARGS, _ctru_hiduser_enable_accelerometer__doc__},
    {"hiduser_disable_accelerometer", _ctru_hiduser_disable_accelerometer, METH_NOARGS, _ctru_hiduser_disable_accelerometer__doc__},
    {"hiduser_enable_gyroscope", _ctru_hiduser_enable_gyroscope, METH_NOARGS, _ctru_hiduser_enable_gyroscope__doc__},
    {"hiduser_disable_gyroscope", _ctru_hiduser_disable_gyroscope, METH_NOARGS, _ctru_hiduser_disable_gyroscope__doc__},
    {"hiduser_get_gyroscope_raw_to_dps_coefficient", _ctru_hiduser_get_gyroscope_raw_to_dps_coefficient, METH_NOARGS, _ctru_hiduser_get_gyroscope_raw_to_dps_coefficient__doc__},
    {"hiduser_get_sound_volume", _ctru_hiduser_get_sound_volume, METH_NOARGS, _ctru_hiduser_get_sound_volume__doc__},
    {NULL, NULL, 0, NULL}, /* Sentinel */
};

static struct PyModuleDef _ctru_module = {
    PyModuleDef_HEAD_INIT,
    "_ctru",
    _ctru__doc__,
    -1,
    _ctru_methods,
    NULL,
    NULL,
    NULL,
    NULL,
};

PyMODINIT_FUNC
PyInit__ctru(void)
{
    return PyModule_Create(&_ctru_module);
}
