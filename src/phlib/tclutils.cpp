/*
 * tclutils.cpp --
 *
 * This file is part of phlib library.
 *
 * Copyright (c) 2012 Andrey V. Nakin <andrey.nakin@gmail.com>
 * All rights reserved.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include	"tclutils.h"
#include	<string.h>
#include	<limits>
#include	<iostream>

namespace phlib {

	namespace error_message {
		static const char bad_int_argument[] = "argument passed is not a valid integer";
		static const char bad_double_argument[] = "argument passed is not a valid double";
		static const char bad_list_argument[] = "argument passed is not a valid list";
	}

	bool TclUtils::getBool(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int ret;

		if (TCL_OK != Tcl_GetBooleanFromObj(interp, objPtr, &ret))
			throw wrong_args_value_exception(error_message::bad_int_argument);

		return ret ? true : false;
	}

	int TclUtils::getInt(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int ret;

		if (TCL_OK != Tcl_GetIntFromObj(interp, objPtr, &ret))
			throw wrong_args_value_exception(error_message::bad_int_argument);

		return ret;
	}

	unsigned TclUtils::getUInt(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		long ret;

		if (
				TCL_OK != Tcl_GetLongFromObj(interp, objPtr, &ret)
				|| ret < std::numeric_limits<unsigned>::min()
				|| ret > std::numeric_limits<unsigned>::max()) {

			std::string msg("expected unsigned integer but got \"");
			msg += Tcl_GetStringFromObj(objPtr, NULL);
			msg += "\"";
			throw wrong_args_value_exception(msg.c_str());
		}

		return static_cast<unsigned>(ret);
	}

	long TclUtils::getLong(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		long ret;

		if (TCL_OK != Tcl_GetLongFromObj(interp, objPtr, &ret))
			throw wrong_args_value_exception(error_message::bad_int_argument);

		return ret;
	}

	double TclUtils::getDouble(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		double ret;

		if (TCL_OK != Tcl_GetDoubleFromObj(interp, objPtr, &ret))
			throw wrong_args_value_exception(error_message::bad_double_argument);

		return ret;
	}

	std::vector<unsigned> TclUtils::getUIntVector(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int length;
		int rc = Tcl_ListObjLength(interp, objPtr, &length);
		if (TCL_OK != rc) {
			throw wrong_args_value_exception(error_message::bad_list_argument);
		}

		std::vector<unsigned> ret;
		for (int i = 0; i < length; ++i) {
			Tcl_Obj* v;
			rc = Tcl_ListObjIndex(interp, objPtr, i, &v);
			if (TCL_OK != rc) {
				throw wrong_args_value_exception(error_message::bad_list_argument);
			}
			ret.push_back(getUInt(interp, v));
		}

		return ret;
	}

	std::vector<double> TclUtils::getDoubleVector(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int length;
		int rc = Tcl_ListObjLength(interp, objPtr, &length);
		if (TCL_OK != rc) {
			throw wrong_args_value_exception(error_message::bad_list_argument);
		}

		std::vector<double> ret;
		for (int i = 0; i < length; ++i) {
			Tcl_Obj* v;
			rc = Tcl_ListObjIndex(interp, objPtr, i, &v);
			if (TCL_OK != rc) {
				throw wrong_args_value_exception(error_message::bad_list_argument);
			}
			ret.push_back(getDouble(interp, v));
		}

		return ret;
	}

	std::vector<std::string> TclUtils::getStringVector(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int length;
		int rc = Tcl_ListObjLength(interp, objPtr, &length);
		if (TCL_OK != rc) {
			throw wrong_args_value_exception(error_message::bad_list_argument);
		}

		std::vector<std::string> ret;
		for (int i = 0; i < length; ++i) {
			Tcl_Obj* v;
			rc = Tcl_ListObjIndex(interp, objPtr, i, &v);
			if (TCL_OK != rc) {
				throw wrong_args_value_exception(error_message::bad_list_argument);
			}
			ret.push_back(std::string(Tcl_GetStringFromObj(v, NULL)));
		}

		return ret;
	}

	std::vector<Tcl_Obj*> TclUtils::getObjectVector(Tcl_Interp *interp, Tcl_Obj *objPtr) {
		int length;
		int rc = Tcl_ListObjLength(interp, objPtr, &length);
		if (TCL_OK != rc) {
			throw wrong_args_value_exception(error_message::bad_list_argument);
		}

		std::vector<Tcl_Obj*> ret;
		for (int i = 0; i < length; ++i) {
			Tcl_Obj* v;
			rc = Tcl_ListObjIndex(interp, objPtr, i, &v);
			if (TCL_OK != rc) {
				throw wrong_args_value_exception(error_message::bad_list_argument);
			}
			ret.push_back(v);
		}

		return ret;
	}

	Tcl_Obj* TclUtils::toListOfDouble(Tcl_Interp *interp, const std::vector<double>& v) {
		Tcl_Obj *ret = Tcl_NewListObj(0, NULL);

		for (std::vector<double>::const_iterator i = v.begin(), end = v.end(); i != end; ++i) {
			Tcl_ListObjAppendElement(interp, ret, Tcl_NewDoubleObj(*i));
		}

		return ret;
	}

	void TclUtils::notifyProcError(Tcl_Interp *interp, const std::exception& ex, const char* default_message) {
		const char *message = ex.what() && *ex.what() ? ex.what() : default_message;
		char*	buf = Tcl_Alloc(static_cast<int>(::strlen(message)) + 1);

		Tcl_SetResult(
			interp, 
			::strcpy(buf, message),
			TCL_DYNAMIC);
	}

}
