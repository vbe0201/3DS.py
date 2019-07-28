#include <3ds.h>

#include "Python.h"

#define _CTRU_ASSERT_IPC_OK(result) \
    if (R_FAILED(result)) { \
        PyErr_SetString(PyExc_OSError, "IPC call failed"); \
        return NULL; \
    }

PyDoc_STRVAR(_ctru__doc__,
"Low-level Python bindings to smea's ctrulib.\n");

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
