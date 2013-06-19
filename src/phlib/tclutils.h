/*
 * tclutils.h --
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

#ifndef	__nettcl_wrapper_utils_h
#define	__nettcl_wrapper_utils_h

extern "C" {
#include	<tcl.h>
}

#include	<exception>
#include	<sstream>
#include	<vector>
#include	<string>

#define TCL_FALSE 0
#define TCL_TRUE 1

namespace phlib {

	struct TclUtils {

		typedef void (StaticHandler)(Tcl_Interp * interp, int objc, Tcl_Obj * const objv[]);

		class tcl_error : public std::exception {

			std::string message;

		public:

			tcl_error() {}

			explicit tcl_error(const char *message) :
				message(message)
			{}

			explicit tcl_error(const std::string& message) :
				message(message)
			{}

			virtual ~tcl_error() throw() {}

			virtual const char* what() const throw() {
				return message.c_str();
			}
		};

		class wrong_num_args_exception : public tcl_error {
			Tcl_Interp* interp;
			int objc;
			Tcl_Obj * const * objv;

		public:

			wrong_num_args_exception(Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[], const char *message) :
					tcl_error(message) {
				this->interp = interp;
				this->objc = objc;
				this->objv = objv;
			}

			wrong_num_args_exception(Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[]) {
				this->interp = interp;
				this->objc = objc;
				this->objv = objv;
			}

			virtual ~wrong_num_args_exception() throw() {}

			void inform(void) const {
				(void) Tcl_WrongNumArgs(interp, objc, objv, what());
			}

			void inform(const char* msg) const {
				(void) Tcl_WrongNumArgs(interp, objc, objv, msg);
			}
		};

		class wrong_args_value_exception : public tcl_error {

		public:
			wrong_args_value_exception(const char* msg) : tcl_error(msg) {}
			wrong_args_value_exception() {}

			virtual ~wrong_args_value_exception() throw() {}

		};

		template <class t>
		static t getArg(const char* argv, const char* error_message) {
			t out;
			if (0 != argv) {
				std::stringstream s(argv);
				s >> out;
				if (s.fail())
					throw wrong_args_value_exception(error_message);
			}
			return out;
		}

		template <class t>
		static t getArg(const char* argv, t& out) {
			if (0 != argv) {
				std::stringstream s(argv);
				s >> out;
				if (s.fail())
					throw wrong_args_value_exception();
			}
			return out;
		}

		static bool getBool(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static int getInt(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static unsigned getUInt(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static long getLong(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static double getDouble(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static std::vector<unsigned> getUIntVector(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static std::vector<double> getDoubleVector(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static std::vector<std::string> getStringVector(Tcl_Interp *interp, Tcl_Obj *objPtr);
		static std::vector<Tcl_Obj*> getObjectVector(Tcl_Interp *interp, Tcl_Obj *objPtr);

		static Tcl_Obj* toListOfDouble(Tcl_Interp *interp, const std::vector<double>& v);

		static void notifyProcError(Tcl_Interp *interp, const std::exception& ex, const char* default_message);

		static void registerCommand(Tcl_Interp * interp, const char *commandName, StaticHandler handler) {
			(void) ::Tcl_CreateObjCommand(
				interp,
				commandName,
				reinterpret_cast<Tcl_ObjCmdProc*>(commandHandler),
				(ClientData) handler,
				NULL);
		}

	private:

		static int commandHandler(ClientData clientData, Tcl_Interp *interp, int objc, struct Tcl_Obj * const objv[]) {
			StaticHandler *handler = (StaticHandler*) clientData;
			if (!handler) return TCL_ERROR;
			return process(
					interp,
					objc,
					objv,
					*handler);
		}

		static int process(Tcl_Interp * interp, int objc, Tcl_Obj * const objv[], StaticHandler handler) {
			int ret_code = TCL_ERROR;

			try {
				handler(interp, objc, objv);
				ret_code = TCL_OK;
			} catch (TclUtils::wrong_num_args_exception& ex) {
				ex.inform();
			} catch (std::exception& ex) {
				TclUtils::notifyProcError(interp, ex, 0);
			} catch (...) {
				::Tcl_SetResult(
					interp,
					const_cast<char*>("Unknown exception"),
					TCL_STATIC);
			}

			return ret_code;
		}

	};

	template<char** type_name>
	class TclObject {
	public:

		virtual ~TclObject() {}

		static const char* typeName() {
		 	return *type_name;
		}

		static void free(Tcl_Obj * const objPtr) {
			delete reinterpret_cast<TclObject*>(objPtr->internalRep.otherValuePtr);
		}

		static void dup(Tcl_Obj * const srcPtr, Tcl_Obj * const dupPtr) {
			const TclObject* src = reinterpret_cast<TclObject*>(srcPtr->internalRep.otherValuePtr);
			dupPtr->internalRep.otherValuePtr = src->clone();
		}

		static Tcl_ObjType* type(void) {
			Tcl_ObjType* type = Tcl_GetObjType(*type_name);
			if (!type) {
				std::string msg("Type is not registered: ");
				msg += *type_name;
				throw TclUtils::tcl_error(msg);
			}
			return type;
		}

		static bool isInstanceOf(const Tcl_Obj* arg) {
			return arg->typePtr == type();
		}

		static TclObject* validateArg(Tcl_Interp * /*interp*/, const Tcl_Obj* arg) {
			if (!isInstanceOf(arg)) {
				std::string msg("Argument passed is not a valid instance of ");
				msg += *type_name;
				throw TclUtils::wrong_args_value_exception(msg.c_str());
			}

			return reinterpret_cast<TclObject*>(arg->internalRep.otherValuePtr);
		}

		static void registerType(void) {
			static Tcl_ObjType typedesc;

			typedesc.name = *type_name;
			typedesc.freeIntRepProc = free;
			typedesc.dupIntRepProc = dup;
			typedesc.updateStringProc = NULL;
			typedesc.setFromAnyProc = NULL;

			::Tcl_RegisterObjType(&typedesc);
		}

		Tcl_Obj* createTclObject() {
			Tcl_Obj* result = Tcl_NewObj();
			result->typePtr = type();
			result->internalRep.otherValuePtr = this;
			return result;
		}

		static Tcl_Obj* createTclObject(TclObject * src) {
			return src->createTclObject();
		}

	protected:

		TclObject() {};
		TclObject(const TclObject&) {};

	private:

		TclObject& operator=(const TclObject&);

		virtual TclObject* clone() const = 0;

	};

}

#endif	//	__nettcl_wrapper_utils_h

