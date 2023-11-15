#define PY_SSIZE_T_CLEAN
#include <Python.h>

/*
 * Implementation copied from Python 3.7.2 implementation of tuple hash. The only modification is that in
 * case of a tuple inside the tuple, it uses the same hash recursively.
 * This implementation has been changed because it contains collisions:
 * https://github.com/python/cpython/pull/9471
 * This is reimplemented here in case matching tuples to hashes that were created in an old Python version
 * is needed.
 */
static Py_hash_t
tuplehashImplementation(PyTupleObject *v)
{
    Py_uhash_t x;  /* Unsigned for defined overflow behavior. */
    Py_hash_t y;
    Py_ssize_t len = Py_SIZE(v);
    PyObject **p;
    Py_uhash_t mult = _PyHASH_MULTIPLIER;
    x = 0x345678UL;
    p = v->ob_item;
    while (--len >= 0) {
        if (PyTuple_Check(*p)) {
            y = tuplehashImplementation((PyTupleObject *) *p);
        } else {
            y = PyObject_Hash(*p);
        }
        p++;
        if (y == -1)
            return -1;
        x = (x ^ y) * mult;
        /* the cast might truncate len; that doesn't change hash stability */
        mult += (Py_hash_t)(82520UL + len + len);
    }
    x += 97531UL;
    if (x == (Py_uhash_t)-1)
        x = -2;
    return x;
}

static PyObject *tuplehash(PyObject *self, PyObject *args) {
    PyTupleObject *tuple;
    if (!PyArg_ParseTuple(args, "O", &tuple)) {
        return NULL;
    }

    if (!PyTuple_Check(tuple)) {
        PyErr_SetString(PyExc_TypeError, "Must be a tuple");
        return NULL;
    }

    return PyLong_FromSsize_t(tuplehashImplementation(tuple));
}


static PyMethodDef functions[] = {
  {"tuplehash", tuplehash, METH_VARARGS, "Get the old hash of a tuple"},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
  PyModuleDef_HEAD_INIT,
  "legacy_hash",
  NULL,
  -1,
  functions
};

PyMODINIT_FUNC PyInit_legacy_hash(void){
  return PyModule_Create(&module);
}
