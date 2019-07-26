#include <3ds.h>

#include "Python.h"

PyDoc_STRVAR(_ctru__doc__,
"Low-level Python bindings to smea's ctrulib.\n");

/* hid */

PyDoc_STRVAR(_ctru_hid_keys_down__doc__, "");

static PyObject *
_ctru_hid_keys_down(PyObject *self, PyObject *args)
{
    u32 result;

    result = hidKeysDown();

    return PyLong_FromUnsignedLong(result);
}

PyDoc_STRVAR(_ctru_hid_keys_held__doc__, "");

static PyObject *
_ctru_hid_keys_held(PyObject *self, PyObject *args)
{
    u32 result;

    result = hidKeysHeld();

    return PyLong_FromUnsignedLong(result);
}


static PyMethodDef _ctru_methods[] = {
    /* hid */
    {"hid_keys_down", _ctru_hid_keys_down, METH_NOARGS, _ctru_hid_keys_down__doc__},
    {"hid_keys_held", _ctru_hid_keys_held, METH_NOARGS, _ctru_hid_keys_held__doc__},
    {NULL, NULL, 0, NULL}, // Sentinel
};

static struct PyModuleDef _ctru_module[] = {
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
