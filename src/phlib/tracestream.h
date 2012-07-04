/*
 * tracestream.h --
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

#ifndef	__TRACESTREAM_H_98823872318747612341235412354751234513627
#define	__TRACESTREAM_H_98823872318747612341235412354751234513627

#include	<sys/stat.h>
#include	<string>
#include	<fstream>

namespace phlib {

	class FileChecker {

		bool	fileExists;
		unsigned long long fileSize;

	protected:

		static std::string getFileName(const std::string& fileName) {
			if (fileName.begin() != fileName.end() && *fileName.begin() == '+') {
				std::string ret = fileName;
				ret.erase(ret.begin(), ret.begin() + 1);
				return ret;
			}
			else
				return fileName;
		}

		static std::ios_base::openmode getFileOpenMode(const std::string& fileName) {
			if (fileName.begin() != fileName.end() && *fileName.begin() == '+') {
				if (fileName.size() > 1)
					return std::ios_base::app;
			}
			else if (fileName.size() > 0)
				return std::ios_base::out;

			// do not really open file
			return static_cast<std::ios_base::openmode>(0);
		}

	public:

		FileChecker(const std::string& fileName) {
			fileExists = false;
			fileSize = 0;

			if (getFileOpenMode(fileName) == std::ios_base::app) {
				struct stat st;
				if (0 == ::stat(getFileName(fileName).c_str(), &st)) {
					fileExists = true;
					fileSize = st.st_size;
				}
			}
		}

		inline bool exists() const {
			return fileExists;
		}

		inline bool isNew() const {
			return !fileExists;
		}

		inline unsigned long long getInitialFileSize() const {
			return fileSize;
		}
	};

	class TraceStream : protected FileChecker, public std::ofstream {

		void checkAndOpen(const std::string& fileName, bool do_open) {
			if (do_open) {
				std::ios_base::openmode mode = getFileOpenMode(fileName);
				if (mode != 0) {
					std::string name = getFileName(fileName);
					if (name.size() > 0)
						open(name.c_str(), mode);
				}
			}
		}

	public:

		TraceStream(const std::string& fileName) : FileChecker(fileName) {
			checkAndOpen(fileName, true);
		}

		TraceStream(const std::string& fileName, bool do_open) : FileChecker(fileName) {
			checkAndOpen(fileName, do_open);
		}

		inline bool isHeaderRequired() {
			return isNew() || getInitialFileSize() == 0;
		}
	};

}

#endif	//	__TRACESTREAM_H_98823872318747612341235412354751234513627
